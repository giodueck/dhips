#include <iostream>
#include <signal.h>
#include "dhipslib.h"
#include "modules.h"
#include "startup.h"
#include "pgsql.h"

using namespace std;

bool killed = false;

// declare all modules
Detector globalLogger;
ModuleI mod_i;
ModuleII mod_ii;

static void break_handler(int sig)
{
    killed = true;
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
    globalLogger = Detector("DHIPS");
    globalLogger.log((const char*)"Started", "localhost");
    mod_i = ModuleI();
    mod_ii = ModuleII();

    // initialize modules
    globalLogger.log((const char*)"Started module initialization", "localhost");
    mod_i.setup();
    mod_ii.setup();

    // while loop
    globalLogger.log((const char*)"Ready", "localhost");
    cout << "before loop" << endl;
    killed = false;
    while (!killed)
    {
        // run module scans
        cout << "before mod_i" << endl;
        mod_i.run();
        cout << "before mod_ii" << endl;
        mod_ii.run();
        
        // sleep a little
        cout << "before sleep" << endl;
        sleep(2);
        cout << ".";
        fflush(stdout);
    }
    cout << "after loop" << endl;

    // stop all modules and exit
    globalLogger.log((const char*)"Stopping modules", "localhost");
    mod_i.stop();
    cout << endl;
    globalLogger.log((const char*)"Stopped", "localhost");
    exit(0);
}