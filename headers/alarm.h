#ifndef ALARM_H
#define ALARM_H

#include <string>

enum AlarmType { notice, important, warning, alert };

class Alarm
{
    private:
        const char *filename = "/var/log/hips/alarmas.log";

    public:
    
    Alarm(){}

    /* Generates an alarm and logs it in alarmas.log along with a timestamp.
       The module is used for readability of the logs, the id is used to recover the description
       of the alarm to add to the log. The severity is determined by the rulesets and only used
       internally.
        R: int corresponding to an enum AlarmType for the alarm if successful, negatives otherwise
        P:  const char *module: roman numeral in the range I-X
            int id: unique identifier of alarm
            char *location: the IP address that originated the alarm, or localhost
    */
    int generate(const char *module, int id, const char *location);
};

#endif // ALARM_H