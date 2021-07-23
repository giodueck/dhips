#include "m_iii.h"

// Dud functions
void ModuleIII::setup(){}
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

int ModuleIII::DetectorIII::scan()
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

    int i = 0;

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

        printf("iface=%s\tflg=%s\n", ifaces[i].c_str(), flgs[i].c_str());
        i++;
    }

    // clean up
    fclose(fd);
    remove("/tmp/netstatlog");
    return 0;
}

int ModuleIII::DetectorIII::PreventerIII::act(int action)
{
    return 0;
}

void ModuleIII::run()
{
    detector.scan();
}