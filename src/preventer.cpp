#include "preventer.h"
#include "dhipslib.h"
#include <stdio.h>

int Preventer::log(const char *module, const char *msg)
{
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
    fprintf(f, "%02d/%02d/%04d %02d:%02d:%02d :: %s: %s\n",
        ts->tm_mday, ts->tm_mon, ts->tm_year + 1900, ts->tm_hour, ts->tm_min, ts->tm_sec, module, msg);

    // close file and free description
    fclose(f);

    return 0;
}

int Preventer::log(const char *msg)
{
    return this->log(this->module.c_str(), msg);
}

int Preventer::act(int action)
{
    printf("Preventer::act() overloading didn't work\n");
    return -1;
}
