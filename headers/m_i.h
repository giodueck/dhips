/*
    Module I
    Description: Verificar archivos binarios de sistema y en particular modificaciones realizadas
                en el archivo /etc/passwd o /etc/shadow.
*/

#ifndef MOD_I_H
#define MOD_I_H

#include "module.h"
#include "monitor.h"
#include "defines.h"

class ModuleI: public Module
{
    private:

    class DetectorI: public Detector
    {
        private:

        std::string sysFileMonitorFilename;
        std::string binMonitorFilename;

        std::string sysFileWatchedNames[EVENT_NUM];
        int nSysFileWatchedNames;
        std::string binWatchedNames[EVENT_NUM];
        int nBinWatchedNames;

        public:

        Monitor *sysFileMonitor;
        Monitor *binMonitor;
        
        DetectorI();

        int setup();
        int scan();
    } detector;

    public: 

    // Constructor
    ModuleI();

    void setup();
    void run();
    void stop();
};

#endif // MOD_I_H