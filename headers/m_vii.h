/*
    Module VII
    Description: Verificar directorio /tmp; que no hayan procesos con nombres extraños o scripts
                ubicados en el mismo. Puede ser tomada como medida de prevención la
                eliminación del archivo o mudarlo a una carpeta de cuarentena.
*/

#ifndef MOD_VII_H
#define MOD_VII_H

#include "module.h"
#include "defines.h"
#include "dhipslib.h"
#include "pgsql.h"
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits>
#include <signal.h>
#include <vector>

class ModuleVII: public Module
{
    private:

    class DetectorVII: public Detector
    {
        private:

        class PreventerVII: public Preventer
        {
            private:

            const char *targetName;
            pid_t target;

            public:

            PreventerVII();

            void setTarget(pid_t pid);
            void setTargetName(const char *name);
            int act(int action = 0);
        } preventer;

        std::vector<std::string> susExtensions, whitelistedProcs;

        public:
        
        DetectorVII();
        DetectorVII(PreventerVII *preventer);

        int setup();
        int scan();
    } detector;

    public: 

    // Constructor
    ModuleVII();

    void setup();
    void run();
    void stop();
};

#endif // MOD_VII_H