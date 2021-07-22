/*
    Module II
    Description: Verificar los usuarios que están conectados. Y desde que origen.
*/

#ifndef MOD_II_H
#define MOD_II_H

#include "module.h"
#include "alarms.h"
#include <utmpx.h>
#include <paths.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <vector>

class ModuleII: public Module
{
    private:

    class DetectorII: public Detector
    {
        private:

        // all currently logged in users
        std::vector<struct utmpx> baseline;

        public:
        
        DetectorII();

        int setup();
        int scan();
    } detector;

    public: 

    // Constructor
    ModuleII();

    void setup();
    void run();
    void stop();
};

#endif // MOD_II_H