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

    return 0;
}

int ModuleIII::DetectorIII::scan(bool setup)
{
    // use netstat to see if promiscous mode
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

    return 0;
}

int ModuleIII::DetectorIII::PreventerIII::act(int action)
{
    return 0;
}

void ModuleIII::setup()
{
    detector.setup();
}

void ModuleIII::run()
{
    detector.scan();
}