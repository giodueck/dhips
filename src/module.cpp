#include "module.h"

Module::Module()
{
    active = false;
}

void Module::setup()
{
    printf("Module::setup() overriding didn't work\n");
}

void Module::disable()
{
    active = false;
}

void Module::run()
{
    printf("Module::run() overriding didn't work\n");
}

void Module::stop()
{
    printf("Module::stop() overriding didn't work\n");
}