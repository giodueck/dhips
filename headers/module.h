#ifndef MODULE_H
#define MODULE_H

#include "detector.h"
#include "preventer.h"

class Module
{
    public:
    
    Module();

    /* This function is called once at the start of the program */
    void setup();

    /* This function is called periodically and runs a detector scan and a preventer act,
       but the latter is dependent on the former.
    */
    void run();
};

#endif // MODULE_H