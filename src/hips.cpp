#include <iostream>
#include "alarmgenerator.h"
#include "startup.h"

int main(int argc, char *argv[])
{
    AlarmGenerator alarm = AlarmGenerator();

    // Startup procedure
    int s = startup();

    // test code
    alarm.generate("XVII", 1234, "localhost");

    return 0;
}