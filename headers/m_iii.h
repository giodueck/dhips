/*
    Module III
    Description: Chequear si hay sniffers o si el equipo ha entrado en modo promiscuo. No
                deben controlar solamente los registros de auditorías, sino también deben
                controlar que no existan herramientas conocidas de aplicaciones de captura de
                paquetes en ejecución, como tcpdump, ethereal, wireshark entre otros. Para el
                módulo de prevención estas herramientas deben ser bloqueadas o eliminadas
                del sistema, además de todas las consideraciones vistas en clases.
*/

#ifndef MOD_III_H
#define MOD_III_H

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
#include <vector>
#include <limits.h>
#include <signal.h>

class ModuleIII: public Module
{
    private:

    class DetectorIII: public Detector
    {
        private:

        class PreventerIII: public Preventer
        {
            private:

            const char *targetName;
            pid_t target;

            public:

            PreventerIII();

            void setTarget(pid_t pid);
            void setTargetName(const char *name);
            int act(int action = 0);
        } preventer;

        struct iface
        {
            std::string name;
            bool promisc;
        };
        
        std::vector<std::string> ifaces, flgs;
        std::vector<std::string> sniffers;
        
        std::vector<struct iface> knownifaces;

        public:
        
        DetectorIII();
        DetectorIII(PreventerIII *preventer);

        int setup();
        int scan(bool setup = false);
    } detector;

    public: 

    // Constructor
    ModuleIII();

    void setup();
    void run();
    void stop();
};

#endif // MOD_III_H