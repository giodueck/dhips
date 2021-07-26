#include "m_vii.h"

// Dud functions
void ModuleVII::stop(){}

ModuleVII::ModuleVII(){}

ModuleVII::DetectorVII::PreventerVII::PreventerVII()
{
    this->module = "VII";
}

ModuleVII::DetectorVII::DetectorVII()
{
    this->module = "VII";
    preventer = PreventerVII();
}

int ModuleVII::DetectorVII::setup()
{

    

    return 0;
}

int ModuleVII::DetectorVII::scan()
{
    

    return 0;
}

void ModuleVII::DetectorVII::PreventerVII::setTarget(pid_t pid)
{
    target = pid;
}

void ModuleVII::DetectorVII::PreventerVII::setTargetName(const char *name)
{
    targetName = name;
}

int ModuleVII::DetectorVII::PreventerVII::act(int action)
{
    if (action == 1)
    {
        // forcibly kill the process with pid = target
        kill(target, SIGKILL);

        // log the action
        char msg[1024];
        sprintf(msg, "Killed process \"%s\" with PID %d", targetName, target);
        log(msg);
        target = 0;
    } else if (action == 2)
    {
        // remove file
    }

    return 0;
}

void ModuleVII::setup()
{
    detector.setup();
}

void ModuleVII::run()
{
    detector.scan();
}