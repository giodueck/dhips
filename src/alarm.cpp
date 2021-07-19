#include "alarm.h"
#include "dhipslib.h"
#include "pgsql.h"
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <iostream>

using namespace std;

int Alarm::generate(const char *module, int id, const char *location)
{
    char *description = NULL;
    int severity = 0, res;
    FILE *f;
    struct tm *ts;
    time_t unixtime;

    // open file
    f = fopen(filename, "a");
    if (!f)
    {
        char msg[128];
        sprintf(msg, "could not open log file %s", filename);
        dhips_perror(msg);
        return -1;
    }

    // get description from postgres DB
    description = pg_get_alarm_description(id);
    if (!description)
    {
        char msg[128];
        sprintf(msg, "could not find info for alarm with id = %d", id);
        dhips_perror_no_errno(msg);
        return -1;
    }

    // get severity from postgres DB
    severity = pg_get_alarm_severity(id);
    if (severity < 0)
    {
        char msg[128];
        sprintf(msg, "could not find info for alarm with id = %d", id);
        dhips_perror_no_errno(msg);
        return -1;
    }

    // write to log
    unixtime = time(NULL);
    ts = localtime(&unixtime);
    fprintf(f, "%02d/%02d/%04d %02d:%02d:%02d :: %s.%d: %s\t:: %s\t\n",
        ts->tm_mday, ts->tm_mon, ts->tm_year + 1900, ts->tm_hour, ts->tm_min, ts->tm_sec, module, id, description, location);

    // close file and free description
    fclose(f);

    return severity;
}