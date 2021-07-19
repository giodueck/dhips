#ifndef DETECTOR_H
#define DETECTOR_H

#include <string>
#include "preventer.h"

enum AlarmType { notice, important, warning, alert };

class Detector
{
    private:
        const char *filename = "/var/log/hips/alarmas.log";
        const std::string module;

    public:
    
    Detector(){}

    /* Logs an alarm in alarmas.log along with a timestamp (this version takes in the module instead
       of using the private variable).
       The module is used for readability of the logs, the id is used to recover the description
       of the alarm to add to the log.
        R: int corresponding to an enum AlarmType for the alarm if successful, negatives otherwise
        P:  const char *module: roman numeral in the range I-X
            int id: unique identifier of alarm
            char *location: the IP address that originated the alarm, or localhost
    */
    int log(const char *module, int id, const char *location);

    /* Logs an alarm in alarmas.log along with a timestamp.
       The id is used to recover the description of the alarm to add to the log.
        R: int corresponding to an enum AlarmType for the alarm if successful, negatives otherwise
        P:  int id: unique identifier of alarm
            char *location: the IP address that originated the alarm, or localhost
    */
    int log(int id, const char *location);

    /* This function is called periodically */
    virtual int scan();

    // TODO: function to get all config from db
};

#endif // DETECTOR_H