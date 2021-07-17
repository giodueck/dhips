#ifndef ALARMGENERATOR_H
#define ALARMGENERATOR_H

#include <string>

enum AlarmType { notice, important, warning, alert };

class AlarmGenerator
{
    private:

    public:
    
    AlarmGenerator(){}

    /* Generates an alarm and logs it in alarmas.log along with a timestamp.
       The task is used for readability of the logs, the id is used to recover the description
       of the alarm to add to the log. The severity is determined by the rulesets.
        R: enum AlarmType for the alarm if successful, negatives otherwise
        P:  int task: number in the range 1-10
            int id: unique identifier of alarm
    */
    int generate(int task, int id);
};

#endif // ALARMGENERATOR_H