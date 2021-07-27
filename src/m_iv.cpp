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
    // check journalctl
    FILE *jp = popen("journalctl -u sshd | grep 'authentication failure'", "r");
    char buf[BUFSIZ];
    char *tok;

    // sample lines: 
    // Jul 27 15:39:30 localhost.localdomain sshd[1704]: pam_unix(sshd:auth): authentication failure; logname= uid=0 euid=0 tty=ssh ruser= rhost=192.168.0.13  user=root
    // Jul 27 15:40:38 localhost.localdomain sshd[1710]: pam_unix(sshd:auth): authentication failure; logname= uid=0 euid=0 tty=ssh ruser= rhost=192.168.0.13

    while (fgets(buf, BUFSIZ, jp))
    {
        tok = strtok(buf, ";");

        for (int i = 0; i < 5; i++)
        {
            tok = strtok(NULL, " ");
        }
        
        printf("%s\n", tok);
    }

    pclose(jp);

    return 0;
}

int ModuleIV::DetectorIV::PreventerIV::act(int action)
{
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