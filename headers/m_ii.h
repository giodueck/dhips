/*
    Module II
    Description: Verificar los usuarios que están conectados. Y desde que origen.
*/

#ifndef MOD_II_H
#define MOD_II_H

#include "module.h"
#include "alarms.h"

class ModuleII: public Module
{
    private:

    class DetectorII: public Detector
    {
        private:

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