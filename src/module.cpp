#include "module.h"

Module::Module()
{
    active = false;
}

void Module::setup()
{
    printf("Module::setup() overloading didn't work\n");
}

void Module::run()
{
    printf("Module::run() overloading didn't work\n");
}

void Module::stop()
{
    printf("Module::stop() overloading didn't work\n");
}