#include <iostream>
#include "alarmgenerator.h"
#include "startup.h"

int main(int argc, char *argv[])
{
    AlarmGenerator alarm = AlarmGenerator();

    // Startup procedure
    startup();

    alarm.generate("XVII", 1234, "localhost");

    return 0;
}