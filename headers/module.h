#ifndef MODULE_H
#define MODULE_H

#include "detector.h"
#include "preventer.h"

class Module
{
    private:
    Detector detector;
    Preventer preventer;

    public:
    
    Module(){}

    /* This function is called periodically and runs a detector scan and a preventer act,
       but the latter is dependent on the former.
    */
    virtual void run();

};

#endif // MODULE_H