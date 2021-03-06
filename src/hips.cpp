#include <iostream>
#include <signal.h>
#include "dhipslib.h"
#include "modules.h"
#include "startup.h"
#include "pgsql.h"
#include "defines.h"

using namespace std;

bool killed = false;

// declare all modules
Detector globalLogger;
ModuleI mod_i;
ModuleII mod_ii;
ModuleIII mod_iii;
ModuleIV mod_iv;
ModuleVII mod_vii;

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

static void init(string outmsg, string logmsg, const char *addmsg = NULL)
{
    string modules = "";
    cout << outmsg;
    globalLogger.log(logmsg.c_str(), "localhost", addmsg);
    cp_to_www();
    int chars = 0;

    if (pg_module_enabled(1) == 1)
    {
        cout << "I";
        fflush(stdout);
        mod_i.setup();
        chars = 1;
        modules += "I";
    } else mod_i.disable();

    if (pg_module_enabled(2) == 1)
    {
        while (chars--) cout << '\b';
        cout << "II";
        fflush(stdout);
        mod_ii.setup();
        chars = 2;
        if (modules.length()) modules += ", ";
        modules += "II";
    } else mod_ii.disable();

    if (pg_module_enabled(3) == 1)
    {
        while (chars--) cout << '\b';
        cout << "III";
        fflush(stdout);
        mod_iii.setup();
        chars = 3;
        if (modules.length()) modules += ", ";
        modules += "III";
    } else mod_iii.disable();

    if (pg_module_enabled(4) == 1)
    {
        while (chars--) cout << '\b';
        cout << "IV";
        fflush(stdout);
        mod_iv.setup();
        chars = 2;
        if (modules.length()) modules += ", ";
        modules += "IV";
    } else mod_iv.disable();
    
    if (pg_module_enabled(7) == 1)
    {
        while (chars--) cout << '\b';
        cout << "VII";
        fflush(stdout);
        mod_vii.setup();
        chars = 3;
        if (modules.length()) modules += ", ";
        modules += "VII";
    } else mod_vii.disable();

    while (chars--) cout << '\b';
    cout << "done\n";
    fflush(stdout);

    modules = "Modules active: " + modules;

    if (addmsg) globalLogger.log("Restarted", "localhost", modules.c_str());
    else globalLogger.log("Started", "localhost", modules.c_str());

    cp_to_www();
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
    mod_iv = ModuleIV();
    mod_vii = ModuleVII();

    // initialize modules
    pg_set_config_changed(0);
    init(string("Starting modules..."), string("Started module initialization"));

    // while loop
    killed = false;
    cout << "HIPS running\n";
    while (!killed)
    {
        // check if config changed
        if (pg_get_config_changed() == 1)
        {
            init(string("Restarting modules..."), string("Restarted module initialization"), (const char *)"Applying settings");
            pg_set_config_changed(0);
            cout << "HIPS running\n";
        }

        // run module scans
        mod_i.run();
        mod_ii.run();
        mod_iii.run();
        mod_iv.run();
        mod_vii.run();
        
        cp_to_www();
        
        // sleep a little
        sleep(SCAN_INTERVAL);
    }

    // stop all modules and exit
    cout << "\nStopping";
    mod_i.stop();
    cout << endl;
    globalLogger.log((const char*)"Stopped", "localhost");
    cp_to_www();
    exit(0);
}