#ifndef DETECTOR_H
#define DETECTOR_H

#include <string>
#include "preventer.h"

enum AlarmType { notice, important, warning, alert };

class Detector
{
    protected:
        const char *filename = "/var/log/hips/alarmas.log";
        std::string module;

    public:

    Detector();

    // std::string module: roman numeral in the range I-X, or DHIPS for the base program
    Detector(std::string module);

    /* Logs an alarm in alarmas.log along with a timestamp (this version takes in the module instead
       of using the private variable).
       The module is used for readability of the logs, the id is used to recover the description
       of the alarm to add to the log.
        R: int corresponding to an enum AlarmType for the alarm if successful, negatives otherwise
        P:  const char *module: roman numeral in the range I-X
            int id: unique identifier of alarm
            char *location: the IP address that originated the alarm, or localhost
            char *msg: an additional optional message or observation
    */
    int log(const char *module, int id, const char *location, const char *msg = NULL);

    /* Logs an alarm in alarmas.log along with a timestamp.
       The id is used to recover the description of the alarm to add to the log.
        R: int corresponding to an enum AlarmType for the alarm if successful, negatives otherwise
        P:  int id: unique identifier of alarm
            char *location: the IP address that originated the alarm, or localhost
            char *msg: an additional optional message or observation
    */
    int log(int id, const char *location, const char *msg = NULL);

    /* Logs a message in alarmas.log along with a timestamp.
       The id is used to recover the description of the alarm to add to the log.
        R: int corresponding to an enum AlarmType for the alarm if successful, negatives otherwise
        P:  char *msg: an additional optional message or observation
            char *location: the IP address that originated the alarm, or localhost
    */
    int log(const char *msg, const char *location, const char *additional = NULL);

    /* This function is called once at the start of the program */
    int setup();

    /* This function is called periodically */
    int scan();
};

#endif // DETECTOR_H