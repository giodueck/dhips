#ifndef PREVENTER_H
#define PREVENTER_H

#include <string>

class Preventer
{
    private:
        const char *filename = "/var/log/hips/prevencion.log";
        const std::string module;

    public:
    
    Preventer(){}

    /* Logs an action in prevencion.log along with a timestamp (this version takes in the module instead
       of using the private variable).
       The module is used for readability of the logs.
        R: 0 if successful, negatives otherwise
        P:  const char *module: roman numeral in the range I-X
            const char *msg: a description of the action taken
    */
    int log(const char *module, const char *msg);

    /* Logs an action in prevencion.log along with a timestamp.
       The module is used for readability of the logs.
        R: 0 if successful, negatives otherwise
        P:  const char *msg: a description of the action taken
    */
    int log(const char *msg);

    /* This function is called to act upon an alarm, action may be used to differentiate between multiple
       different possibilities.
    */
    virtual int act(int action = 0);
};

#endif // PREVENTER_H