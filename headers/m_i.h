/*
    Module I
    Description: Verificar archivos binarios de sistema y en particular modificaciones realizadas
                 en el archivo /etc/passwd o /etc/shadow.
*/

#ifndef MOD_I_H
#define MOD_I_H

#include "module.h"
#include "monitor.h"
#include "alarms.h"

#define MONITOR_LOG "/var/log/hips/monitor.log"
#define MONITOR_OUT "/var/log/hips/monitor.out"

class ModuleI: public Module
{
    private:

    class DetectorI: public Detector
    {
        private:

        public:

        Monitor *sysFileMonitor;
        
        DetectorI();

        int setup();
        int scan();
    } detector;

    public: 

    // Constructor
    ModuleI();

    void setup();
    void run();
};

#endif // MOD_I_H