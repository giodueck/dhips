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
ModuleIII mod_iii;

static void break_handler(int sig)
{
    killed = true;
}

static int cp_to_www()
{
    // Alarmas
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

    // Prevencion
    fp1=fopen("/var/log/hips/prevencion.log","r");
    fp2=fopen("/var/www/log/prevencion.log","w");
    if(!fp1)
    {
        char msg[128];
        sprintf(msg, "cp_to_www: could not open /var/log/hips/prevencion.log");
        dhips_perror(msg);
        return -1;
    }
    if(!fp2)
	{
        char msg[128];
        sprintf(msg, "cp_to_www: could not open /var/www/log/prevencion.log");
        dhips_perror(msg);
        return -1;
	}
    ch=(char *)malloc(BUFSIZ);
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
    mod_i = ModuleI();
    mod_ii = ModuleII();
    mod_iii = ModuleIII();

    // initialize modules
    cout << "Starting modules...";
    globalLogger.log((const char*)"Started module initialization", "localhost");
    int chars = 0;
    if (pg_module_enabled(1) == 1)
    {
        cout << "I";
        fflush(stdout);
        mod_i.setup();
        chars = 1;
    }
    if (pg_module_enabled(2) == 1)
    {
        while (chars--) cout << '\b';
        cout << "II";
        fflush(stdout);
        mod_ii.setup();
        chars = 2;
    }
    if (pg_module_enabled(3) == 1)
    {
        while (chars--) cout << '\b';
        cout << "III";
        fflush(stdout);
        mod_iii.setup();
        chars = 3;
    }

    while (chars--) cout << '\b';
    cout << "done\n";
    fflush(stdout);

    // while loop
    globalLogger.log((const char*)"Started", "localhost");
    killed = false;
    cout << "HIPS running\n";
    while (!killed)
    {
        cp_to_www();

        // run module scans
        mod_i.run();
        mod_ii.run();
        mod_iii.run();
        
        // sleep a little
        sleep(2);
    }

    // stop all modules and exit
    cout << "\nStopping";
    mod_i.stop();
    cout << endl;
    globalLogger.log((const char*)"Stopped", "localhost");
    cp_to_www();
    exit(0);
}