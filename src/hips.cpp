#include <iostream>
#include <signal.h>
#include "dhipslib.h"
#include "modules.h"
#include "startup.h"
#include "pgsql.h"

using namespace std;

// declare all modules
ModuleI mod_i;

static void break_handler(int sig)
{
    // stop all modules and exit
    mod_i.stop();
    cout << endl;
    exit(0);
}

int main(int argc, char *argv[])
{
    /* Startup procedures */
    int s = startup();

    // test code

    // set up break signal handling
    struct sigaction sigbreak;

    sigbreak.sa_handler = break_handler;
    sigemptyset (&sigbreak.sa_mask);
    sigbreak.sa_flags = 0;
    if (sigaction(SIGINT, &sigbreak, NULL) != 0) dhips_perror("sigaction");

    // create module instances
    mod_i = ModuleI();

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