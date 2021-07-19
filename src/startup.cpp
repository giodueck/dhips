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
        fprintf(f, "# timestamp :: descripcion :: IP :: codigo de alarma\n");
        fprintf(f, "# [dd]/[mm]/[yyyy] [hh]:[mm]:[ss] :: [texto] :: [IPv4/localhost] :: [modulo].[code]\n\n");
        fclose(f);
    }

    // Database
    int res = pg_check_db();
    return res;
}