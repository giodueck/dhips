#include "preventer.h"
#include "dhipslib.h"
#include <stdio.h>

using namespace std;

Preventer::Preventer(){}

Preventer::Preventer(string module)
{
    this->module = module;
}

int Preventer::log(const char *module, const char *msg, int action)
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
    if (action)
        fprintf(f, "%02d/%02d/%04d %02d:%02d:%02d\t:: %s.%d: %s\n",
            ts->tm_mday, ts->tm_mon, ts->tm_year + 1900, ts->tm_hour, ts->tm_min, ts->tm_sec, module, action, msg);
    else
        fprintf(f, "%02d/%02d/%04d %02d:%02d:%02d\t:: %s: %s\n",
            ts->tm_mday, ts->tm_mon, ts->tm_year + 1900, ts->tm_hour, ts->tm_min, ts->tm_sec, module, msg);

    // close file and free description
    fclose(f);

    // compose email
    const char *subject = "DHIPS: New prevention";
    char e_msg[BUFSIZ];
    if (action)
        sprintf(e_msg, "New preventive action by DHIPS. Log reads:\n%02d/%02d/%04d %02d:%02d:%02d\t:: %s.%d: %s\n",
            ts->tm_mday, ts->tm_mon, ts->tm_year + 1900, ts->tm_hour, ts->tm_min, ts->tm_sec, module, action, msg);
    else
        sprintf(e_msg, "New preventive action by DHIPS. Log reads:\n%02d/%02d/%04d %02d:%02d:%02d\t:: %s: %s\n",
            ts->tm_mday, ts->tm_mon, ts->tm_year + 1900, ts->tm_hour, ts->tm_min, ts->tm_sec, module, msg);

    dhips_send_email(subject, e_msg);

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
