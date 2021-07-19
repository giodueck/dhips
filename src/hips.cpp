#include <iostream>
#include "alarm.h"
#include "startup.h"

int main(int argc, char *argv[])
{
    Alarm alarm = Alarm();

    // Startup procedure
    int s = startup();

    // test code
    alarm.generate("XVII", 1234, "localhost");

    return 0;
}