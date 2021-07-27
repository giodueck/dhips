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

    return 0;
}

int ModuleIV::DetectorIV::scan()
{

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