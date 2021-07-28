#include "m_iv.h"

// Dud functions
void ModuleIV::stop(){}

ModuleIV::ModuleIV(){}

ModuleIV::DetectorIV::PreventerIV::PreventerIV()
{
    this->module = "IV";
}

ModuleIV::DetectorIV::DetectorIV()
{
    this->module = "IV";
    preventer = PreventerIV();
}

int ModuleIV::DetectorIV::setup()
{
    // set threshold values
    sshWarnThreshold = pg_get_threshold(sshWarnName);
    sshBlockThreshold = pg_get_threshold(sshBlockName);
    webWarnThreshold = pg_get_threshold(webWarnName);
    webBlockThreshold = pg_get_threshold(webBlockName);

    // clear IP lists, they will be set again
    sshIps.clear();
    webIps.clear();
    sshIpsFailures.clear();
    webIpsFailures.clear();

    return 0;
}

int ModuleIV::DetectorIV::scan()
{
    /* SSH */
    // check journalctl
    FILE *jp = popen("/usr/bin/journalctl -u sshd | /usr/bin/grep 'authentication failure'", "r");
    char buf[BUFSIZ];
    char *tok;
    bool there = false;
    int aux;
    std::vector<int> newSshIpsFailures, newWebIpsFailures;

    // set up nome new counters
    for (int i = 0; i < sshIps.size(); i++)
        newSshIpsFailures.push_back(0);
    
    for (int i = 0; i < webIps.size(); i++)
        newWebIpsFailures.push_back(0);

    // sample lines: 
    // Jul 27 15:39:30 localhost.localdomain sshd[1704]: pam_unix(sshd:auth): authentication failure; logname= uid=0 euid=0 tty=ssh ruser= rhost=192.168.0.13  user=root
    // Jul 27 15:40:38 localhost.localdomain sshd[1710]: pam_unix(sshd:auth): authentication failure; logname= uid=0 euid=0 tty=ssh ruser= rhost=192.168.0.13
    // Jul 27 18:19:36 localhost.localdomain sshd[5037]: PAM 2 more authentication failures; logname= uid=0 euid=0 tty=ssh ruser= rhost=192.168.0.13  user=root

    // parse output
    while (fgets(buf, BUFSIZ, jp))
    {
        tok = strtok(buf, "]");
        tok = strtok(NULL, ": ");
        tok = strtok(NULL, ": ");
        if ((aux = atoi(tok)))
            aux--;
        else aux = 0;

        tok = strtok(NULL, ";");
        for (int i = 0; i < 6; i++)
        {
            tok = strtok(NULL, " \n");
        }
        
        // tok is the IP
        tok += 6;

        // check if the IP is known
        there = false;
        for (int j = 0; j < sshIps.size(); j++)
        {
            if (strcmp(tok, sshIps[j].c_str()) == 0)
            {
                there = true;
                newSshIpsFailures[j] += aux + 1;
                break;
            }
        }

        // store if not there
        if (!there)
        {
            sshIps.push_back(std::string(tok));
            newSshIpsFailures.push_back(aux + 1);
        }
    }
    pclose(jp);

    for (int i = 0; i < sshIps.size(); i++)
    {
        // ignore-list
        there = false;
        for (int j = 0; j < sshIgnoreIps.size(); j++)
        {
            if (strcmp(sshIgnoreIps[j].c_str(), sshIps[i].c_str()) == 0)
                there = true;
        }
        if (there) break;

        // if newly over block threshold log adn take action
        if (newSshIpsFailures[i] > sshBlockThreshold && sshIpsFailures.size() > i && newSshIpsFailures[i] > sshIpsFailures[i])
        {
            log(ALARM_IV_SSH_BLOCK, sshIps[i].c_str(), "[Action taken]");
            preventer.setIp(sshIps[i].c_str());
            preventer.act(0);
            sshIgnoreIps.push_back(sshIps[i]);
            break;
        }

        // if newly over warn threshold log warning
        if (newSshIpsFailures[i] > sshWarnThreshold)
        {
            // if ip is known and wasn't already warned
            if (sshIpsFailures.size() >= newSshIpsFailures.size())
            {
                if (newSshIpsFailures[i] > sshIpsFailures[i] && sshIpsFailures[i] <= sshWarnThreshold)
                {
                    char msg[32];
                    sprintf(msg, "Total: %d", newSshIpsFailures[i]);
                    log(ALARM_IV_SSH_WARN, sshIps[i].c_str(), msg);
                }
            } else
            {
                char msg[32];
                sprintf(msg, "Total: %d", newSshIpsFailures[i]);
                log(ALARM_IV_SSH_WARN, sshIps[i].c_str(), msg);
            }
        }

        sshIpsFailures = newSshIpsFailures;
    }

    return 0;
}

void ModuleIV::DetectorIV::PreventerIV::setIp(const char *ip)
{
    this->ip = ip;
}

int ModuleIV::DetectorIV::PreventerIV::act(int action)
{
    if (action == 0 && ip)
    {
        // block IP with an iptables rule
        char cmd[512];
        sprintf(cmd, "iptables -A INPUT -s %s -p tcp --destination-port 22 -j DROP", ip);
        system(cmd);

        // log the action
        char msg[1024];
        sprintf(msg, "Blocked \"%s\" from port 22 (SSH)", ip);
        log(msg);
        ip = NULL;
    }

    return 0;
}

void ModuleIV::setup()
{
    active = true;
    detector.setup();
}

void ModuleIV::run()
{
    if (!active) return;
    detector.scan();
}