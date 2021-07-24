#include "startup.h"
#include "pgsql.h"
#include "dhipslib.h"
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdlib.h>

int startup()
{
    // Directories
    mkdir("/var/log/hips", 0755);
    mkdir("/var/www/log", 0755);

    // Log files
    FILE *f;
    // alarmas.log
    f = fopen("/var/log/hips/alarmas.log", "r");
    if (f)
        fclose(f);
    else
    {
        f = fopen("/var/log/hips/alarmas.log", "w");
        fprintf(f, "# timestamp :: modulo y codigo de alarma: descripcion :: IP :: Observaciones\n");
        fprintf(f, "# [dd]/[mm]/[yyyy] [hh]:[mm]:[ss] :: [modulo(romano)].[codigo(numero)]: [texto] :: [IPv4/localhost] :: [texto]\n\n");
        fclose(f);
    }

    // prevencion.log
    f = fopen("/var/log/hips/prevencion.log", "r");
    if (f)
        fclose(f);
    else
    {
        f = fopen("/var/log/hips/prevencion.log", "w");
        fprintf(f, "# timestamp :: modulo: descripcion\n");
        fprintf(f, "# [dd]/[mm]/[yyyy] [hh]:[mm]:[ss] :: [modulo(romano)]: [texto]\n\n");
        fclose(f);
    }

    // utmp for WSL2 Ubuntu, which does not create it by default
    FILE *log_file = fopen("/var/run/utmp", "r");
    if (!log_file)
        system("/bin/sudo /bin/bash -c \"/bin/echo '[1] [00053] [~~  ] [runlevel] [~       ] [5.4.72-microsoft-standard-WSL2] [0.0.0.0    ] [2021-07-22T00:00:00,040218+00:00]' | /bin/utmpdump -r > /var/run/utmp\" 2> /dev/null");
    else fclose(log_file);
    
    // monitor.out and monitor.log
    f = fopen("/var/log/hips/sfmonitor.out", "a");
    fclose(f);
    f = fopen("/var/log/hips/sfmonitor.log", "a");
    fclose(f);
    f = fopen("/var/log/hips/bmonitor.out", "a");
    fclose(f);
    f = fopen("/var/log/hips/bmonitor.log", "a");
    fclose(f);

    // Database
    int res = pg_check_db();
    return res;
}