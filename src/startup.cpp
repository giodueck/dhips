#include "startup.h"
#include "pgsql.h"
#include "dhipslib.h"
#include <stdio.h>

int startup()
{
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

    // Database
    int res = pg_check_db();
    return res;
}