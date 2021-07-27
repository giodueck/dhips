/*
    Module IV
    Description: Examinar los archivos log: Se deben buscar patrones de acceso indebidos en las
                bitácoras del sistema. A partir de lo detectado, deberán bloquear IPs o cambiar
                contraseña de usuario o directamente bloquear un usuario, incluso puede
                requerir bajar temporalmente el servicio de correo.
*/

#ifndef MOD_IV_H
#define MOD_IV_H

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

class ModuleIV: public Module
{
    private:

    class DetectorIV: public Detector
    {
        private:

        class PreventerIV: public Preventer
        {
            private:

            const char *ip;

            public:

            PreventerIV();

            void setIp(const char* ip);

            int act(int action = 0);
        } preventer;

        // database names
        const char *sshWarnName = "sshWarn";
        const char *sshBlockName = "sshBlock";
        const char *webWarnName = "webWarn";
        const char *webBlockName = "webBlock";
        
        // database values are copied during setup
        int sshWarnThreshold, sshBlockThreshold;
        int webWarnThreshold, webBlockThreshold;

        // lists that contain all IPs that appear in error logs
        std::vector<std::string> sshIps;
        std::vector<std::string> webIps;
        std::vector<int> sshIpsFailures;
        std::vector<int> webIpsFailures;

        // already blocked, this list is not reset
        std::vector<std::string> sshIgnoreIps;
        std::vector<std::string> webIgnoreIps;

        public:
        
        DetectorIV();
        DetectorIV(PreventerIV *preventer);

        int setup();
        int scan();
    } detector;

    public: 

    // Constructor
    ModuleIV();

    void setup();
    void run();
    void stop();
};

#endif // MOD_IV_H