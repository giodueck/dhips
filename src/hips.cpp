#include <iostream>
#include "alarm.h"
#include "preventer.h"
#include "startup.h"

int main(int argc, char *argv[])
{
    Alarm alarm = Alarm();
    Preventer preventer = Preventer();

    // Startup procedure
    int s = startup();

    // test code
    alarm.generate("XVII", 1234, "localhost");
    preventer.log("XVII", "This is a text description");

    return 0;
}