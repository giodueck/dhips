#ifndef PREVENTER_H
#define PREVENTER_H

#include <string>

class Preventer
{
    private:
        const char *filename = "/var/log/hips/prevencion.log";

    public:
    
    Preventer(){}

    /* Logs an action in prevencion.log along with a timestamp.
       The module is used for readability of the logs.
        R: 0 if successful, negatives otherwise
        P:  const char *module: roman numeral in the range I-X
            const char *msg: a description of the action taken
    */
    int log(const char *module, const char *msg);
};

#endif // PREVENTER_H