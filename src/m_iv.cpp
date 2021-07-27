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
    FILE *jp = popen("journalctl -u sshd | grep 'authentication failure'", "r");
    char buf[BUFSIZ];
    char *tok;
    bool there = false;
    std::vector<int> newSshIpsFailures, newWebIpsFailures;

    // set up nome new counters
    for (int i = 0; i < sshIps.size(); i++)
        newSshIpsFailures.push_back(0);
    
    for (int i = 0; i < webIps.size(); i++)
        newWebIpsFailures.push_back(0);

    // sample lines: 
    // Jul 27 15:39:30 localhost.localdomain sshd[1704]: pam_unix(sshd:auth): authentication failure; logname= uid=0 euid=0 tty=ssh ruser= rhost=192.168.0.13  user=root
    // Jul 27 15:40:38 localhost.localdomain sshd[1710]: pam_unix(sshd:auth): authentication failure; logname= uid=0 euid=0 tty=ssh ruser= rhost=192.168.0.13

    // parse output
    while (fgets(buf, BUFSIZ, jp))
    {
        tok = strtok(buf, ";");
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
                newSshIpsFailures[j]++;
                break;
            }
        }

        // store if not there
        if (!there)
        {
            sshIps.push_back(std::string(tok));
            newSshIpsFailures.push_back(1);
        }
    }
    pclose(jp);

    for (int i = 0; i < sshIps.size(); i++)
    {
        // if newly over warn threshold log warning
        if (newSshIpsFailures[i] > sshWarnThreshold && newSshIpsFailures[i] > sshIpsFailures[i])
        {
            log(ALARM_IV_SSH_WARN, sshIps[i].c_str());
        }

        // if newly over block threshold log adn take action
        if (newSshIpsFailures[i] > sshBlockThreshold && newSshIpsFailures[i] > sshIpsFailures[i])
        {
            log(ALARM_IV_SSH_BLOCK, sshIps[i].c_str(), "[Action taken]");
            preventer.act(0);
        }

        sshIpsFailures = newSshIpsFailures;
    }

    return 0;
}

void ModuleIV::DetectorIV::PreventerIV::setIp(const char *ip)
{
    strcpy(this->ip, ip);
}

int ModuleIV::DetectorIV::PreventerIV::act(int action)
{
    if (action = 0)
    {
        printf("SSH threshold passed for ip %s\n", ip);
    }
    // if (target)
    // {
    //     // forcibly kill the process with pid = target
    //     kill(target, SIGKILL);

    //     // log the action
    //     char msg[1024];
    //     sprintf(msg, "Killed process \"%s\" with PID %d", targetName, target);
    //     log(msg);
    //     target = 0;
    // }

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