#include "dhipslib.h"
#include <iostream>
#include <string>

using namespace std;

void dhips_perror(const char *msg)
{
    string errmsg;
    errmsg = "dhips: " + string(msg);
    perror(errmsg.c_str());
}
