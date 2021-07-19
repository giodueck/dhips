#include <iostream>
#include "module.h"
#include "startup.h"

int main(int argc, char *argv[])
{
    Detector alarm = Detector();
    Preventer preventer = Preventer();

    // Startup procedure
    int s = startup();

    // test code
    alarm.log("XVII", 12345, "localhost");
    alarm.log(12345, "localhost");
    // alarm.log("XVII", 1234, "localhost");
    // preventer.log("XVII", "This is a text description");

    return 0;
}