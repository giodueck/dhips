#include <iostream>
#include "modules.h"
#include "startup.h"
#include "pgsql.h"

using namespace std;

int main(int argc, char *argv[])
{
    /* Startup procedures */
    int s = startup();

    // test code


    // create module instances
    ModuleI mod_i = ModuleI();

    // initialize modules
    mod_i.setup();

    // while loop
    while (1)
    {
        // run module scans
        mod_i.run();
        
        // sleep a little
        sleep(2);
        cout << ".";
        fflush(stdout);
    }

    return 0;
}