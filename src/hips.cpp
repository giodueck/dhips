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

static int cp_to_www()
{
    FILE *fp1=fopen("/var/log/hips/alarmas.log","r");
    FILE *fp2=fopen("/var/www/log/alarmas.log","w");
    if(!fp1)
    {
        char msg[128];
        sprintf(msg, "cp_to_www: could not open /var/log/hips/alarmas.log");
        dhips_perror(msg);
        return -1;
    }
    if(!fp2)
	{
        char msg[128];
        sprintf(msg, "cp_to_www: could not open /var/www/log/alarmas.log");
        dhips_perror(msg);
        return -1;
	}
    char *ch=(char *)malloc(BUFSIZ);
    while(!feof(fp1))
    {
        memset(ch,0,BUFSIZ);
        int size=fread(ch,1,BUFSIZ,fp1);
        fwrite(ch,size,1,fp2); 
    }
    fclose(fp1);
    fclose(fp2);

    return 0;
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
    killed = false;
    while (!killed)
    {
        cp_to_www();

        // run module scans
        mod_i.run();
        mod_ii.run();
        
        // sleep a little
        sleep(2);
        cout << ".";
        fflush(stdout);
    }

    // stop all modules and exit
    globalLogger.log((const char*)"Stopping modules", "localhost");
    mod_i.stop();
    cout << endl;
    globalLogger.log((const char*)"Stopped", "localhost");
    exit(0);
}