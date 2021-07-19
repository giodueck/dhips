#include <iostream>
#include "detector.h"
#include "preventer.h"
#include "startup.h"

int main(int argc, char *argv[])
{
    Detector alarm = Detector();
    Preventer preventer = Preventer();

    // Startup procedure
    int s = startup();

    // test code
    alarm.log("XVII", 1234, "localhost");
    preventer.log("XVII", "This is a text description");

    return 0;
}