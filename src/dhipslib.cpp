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

void dhips_perror_no_errno(const char *msg)
{
    cerr << "dhips: " << string(msg) << endl;
}
