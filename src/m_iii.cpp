#include "m_iii.h"

// Dud functions
void ModuleIII::stop(){}

ModuleIII::ModuleIII(){}

ModuleIII::DetectorIII::PreventerIII::PreventerIII()
{
    this->module = "III";
}

ModuleIII::DetectorIII::DetectorIII()
{
    this->module = "III";
    preventer = PreventerIII();
}

int ModuleIII::DetectorIII::setup()
{
    scan(true);

    // process findings
    struct iface ifc;

    while (ifaces.size())
    {
        ifc.name = ifaces[0];
        ifc.promisc = false;

        // check flags for 'P'
        for (int c = 0; c < flgs[0].length(); c++)
        {
            // if promiscous mode found log it
            if (flgs[0][c] == 'P')
            {
                std::string msg = "Iface: " + ifaces[0];
                log(ALARM_III_PROMISC_ON, "localhost", msg.c_str());
                ifc.promisc = true;
                break;
            } // else dont
        }

        knownifaces.push_back(ifc);

        ifaces.erase(ifaces.begin());
        flgs.erase(flgs.begin());
    }

    // get list of sniffers
    int i = 0;
    char *dest;
    do
    {
        // system files
        i = pg_get_targeted_proc_name(i, SNIFFER_TYPE, &dest);
        if (i > 0)
            sniffers.push_back(std::string(dest));
    } while (i > 0);

    return 0;
}

int ModuleIII::DetectorIII::scan(bool setup)
{
    // use netstat to see if in promiscous mode
    system("/usr/bin/netstat -i > /tmp/netstatlog");

    // creates temporary file
    FILE *fd = fopen("/tmp/netstatlog", "r");
    if (!fd)
    {
        dhips_perror("could not open /tmp/netstatlog");
        return -1;
    }

    char buf[BUFSIZ];
    char *tok;
    std::string delim = "\t\n ";

    // parse netstat -i output, store iface and flg
    while (fgets(buf, BUFSIZ, fd))
    {
        if (strcmp(buf, "Kernel Interface table\n") == 0)
            continue;

        tok = strtok(buf, delim.c_str());
        if (strcmp(tok, "Iface") == 0)
            continue;
        else
            ifaces.push_back(std::string(tok));
        
        for (int i = 0; i < 10; i++)
            tok = strtok(NULL, delim.c_str());
        
        flgs.push_back(std::string(tok));
    }

    // clean up
    fclose(fd);
    remove("/tmp/netstatlog");

    // setup creates different logs
    if (setup)
        return 0;
    
    // process findings
    bool known = false;
    int i;
    struct iface ifc;

    while (ifaces.size())
    {
        ifc.name = ifaces[0];
        ifc.promisc = false;

        // check if known
        for (i = 0; i < knownifaces.size(); i++)
        {
            if (strcmp(knownifaces[i].name.c_str(), ifaces[0].c_str()) == 0)
            {
                known = true;
                break;
            }
        }

        if (!known)
        {
            // check flags for 'P'
            for (int c = 0; c < flgs[0].length(); c++)
            {
                // if promiscous mode found log it
                if (flgs[0][c] == 'P')
                {
                    std::string msg = "Iface: " + ifaces[0];
                    log(ALARM_III_PROMISC_ON, "localhost", msg.c_str());
                    ifc.promisc = true;
                    break;
                } // else dont
            }

            knownifaces.push_back(ifc);
        } else
        {
            // if known, check if it has changed
            for (int c = 0; c < flgs[0].length(); c++)
            {
                // if promiscous mode found
                if (flgs[0][c] == 'P')
                {
                    ifc.promisc = true;
                    break;
                }
            }

            // if promisc changed log it and update
            if (ifc.promisc != knownifaces[i].promisc)
            {
                std::string msg = "Iface: " + ifaces[0];
                if (ifc.promisc) log(ALARM_III_PROMISC_ON, "localhost", msg.c_str());
                else log(ALARM_III_PROMISC_OFF, "localhost", msg.c_str());
                knownifaces[i] = ifc;
            }
        }

        ifaces.erase(ifaces.begin());
        flgs.erase(flgs.begin());
    }

    // sniffers
    pid_t pid;
    for (i = 0; i < sniffers.size(); i++)
    {
        pid = 0;
        getPidByName(&pid, sniffers[i].c_str());
        if (pid)
        {
            // sniffer found
            char msg[1024];
            sprintf(msg, "PID: %d Name: %s [Action taken]", pid, sniffers[i].c_str()); 
            log(ALARM_III_SNIFFER_FOUND, "localhost", msg);
            preventer.setTargetName(sniffers[i].c_str());
            preventer.setTarget(pid);
            preventer.act();
        }
    }

    return 0;
}

void ModuleIII::DetectorIII::PreventerIII::setTarget(pid_t pid)
{
    target = pid;
}

void ModuleIII::DetectorIII::PreventerIII::setTargetName(const char *name)
{
    targetName = name;
}

int ModuleIII::DetectorIII::PreventerIII::act(int action)
{
    if (target)
    {
        // forcibly kill the process with pid = target
        kill(target, SIGKILL);

        // log the action
        char msg[1024];
        sprintf(msg, "Killed process \"%s\" with PID %d", targetName, target);
        log(msg);
        target = 0;
    }

    return 0;
}

void ModuleIII::setup()
{
    active = true;
    detector.setup();
}

void ModuleIII::run()
{
    if (!active) return;
    detector.scan();
}