#include "detector.h"
#include "dhipslib.h"
#include "pgsql.h"
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <iostream>
#include <cstring>
#include "defines.h"
#include "dhipslib.h"

using namespace std;

Detector::Detector(){}

Detector::Detector(string module)
{
    this->module = module;
}

int Detector::log(const char *msg, const char *location, const char *additional)
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

    // write to log
    unixtime = time(NULL);
    ts = localtime(&unixtime);
    if (additional)
    {
        fprintf(f, "%02d/%02d/%04d %02d:%02d:%02d\t:: %s: %s\t:: %s\t:: %s\n",
            ts->tm_mday, ts->tm_mon, ts->tm_year + 1900, ts->tm_hour, ts->tm_min, ts->tm_sec, module.c_str(), msg, location, additional);
    } else
        fprintf(f, "%02d/%02d/%04d %02d:%02d:%02d\t:: %s: %s\t:: %s\t::\n",
            ts->tm_mday, ts->tm_mon, ts->tm_year + 1900, ts->tm_hour, ts->tm_min, ts->tm_sec, module.c_str(), msg, location);

    // close file and free description
    free(description);
    fclose(f);

    return severity;
}

int Detector::log(const char *module, int id, const char *location, const char *msg)
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
    fprintf(f, "%02d/%02d/%04d %02d:%02d:%02d\t:: %s.%d: %s\t:: %s\t:: %s\n",
        ts->tm_mday, ts->tm_mon, ts->tm_year + 1900, ts->tm_hour, ts->tm_min, ts->tm_sec, module, id, description, location, (msg && strlen(msg)) ? msg : "");

    // close file
    fclose(f);

    // compose email and free description
    const char *subject = "DHIPS: New alarm";
    char e_msg[BUFSIZ];
    sprintf(e_msg, "New alarm raised by DHIPS. Log reads:\n%02d/%02d/%04d %02d:%02d:%02d\t:: %s.%d: %s\t:: %s\t:: %s\n",
        ts->tm_mday, ts->tm_mon, ts->tm_year + 1900, ts->tm_hour, ts->tm_min, ts->tm_sec, module, id, description, location, (msg && strlen(msg)) ? msg : "");
    
    dhips_send_email(subject, e_msg);
    free(description);

    return severity;
}

int Detector::log(int id, const char *location, const char *msg)
{
    if (module.length())
        return this->log(this->module.c_str(), id, location, msg);
    else 
    {
        char msg[128];
        sprintf(msg, "module not set");
        dhips_perror_no_errno(msg);
        return -1;
    }
}

int Detector::setup()
{
    cout << "Detector::setup() overriding didn't work\n";
    return -1;
}

int Detector::scan()
{
    cout << "Detector::scan() overriding didn't work\n";
    return -1;
}