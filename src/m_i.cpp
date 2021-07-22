#include "m_i.h"
#include "pgsql.h"
#include <iostream>

using namespace std;

ModuleI::DetectorI::DetectorI()
{
    sysFileMonitorFilename = string("/var/log/hips/sfmonitor");
    binMonitorFilename = string("/var/log/hips/bmonitor");
    nSysFileWatchedNames = 0;
    nBinWatchedNames = 0;
    this->module = "I";
    sysFileMonitor = new Monitor("I");
    binMonitor = new Monitor("I");
    this->preventer = NULL; // no preventing for modifying files
}

int ModuleI::DetectorI::setup()
{
    int i = 0, j = 0;
    char *dest;

    // get all monitoring filenames
    do
    {
        // system files
        i = pg_get_monitor_filename(i, I_SYSFILE_TYPE, &dest);
        if (i > 0)
        {
            sysFileMonitor->addWatch(string(dest));
        }
        // possible mem leak with dest

        // binaries
        j = pg_get_monitor_filename(j, I_BINARY_TYPE, &dest);
        if (j > 0)
        {
            binMonitor->addWatch(string(dest));
        }
        // possible mem leak with dest
    } while (i > 0 || j > 0);

    // start monitoring
    sysFileMonitor->start(sysFileMonitorFilename + ".log", sysFileMonitorFilename + ".out");
    binMonitor->start(binMonitorFilename + ".log", binMonitorFilename + ".out");

    return 0;
}

int ModuleI::DetectorI::scan()
{
    // get reports
    vector<struct MonitorEvent> sfevents, bevents;
    sysFileMonitor->extractEvents(sfevents, sysFileMonitorFilename + ".log");
    binMonitor->extractEvents(bevents, binMonitorFilename + ".log");

    // process events
    string msg;
    // system files
    while (sfevents.size())
    {
        // log an event
        if (sfevents[0].mask & IN_MODIFY)
        {
            msg = "File=" + sfevents[0].filename;
            log(ALARM_I_FILE_MOD, "localhost", msg.c_str());
        }
        if (sfevents[0].mask & IN_DELETE_SELF)
        {
            msg = "File=" + sfevents[0].filename;
            log(ALARM_I_FILE_DEL, "localhost", msg.c_str());
        }

        // delete the event
        sfevents.erase(sfevents.begin());
    }

    // binary files
    while (bevents.size())
    {
        // log an event
        if (bevents[0].mask & IN_MODIFY)
        {
            msg = "File=" + bevents[0].filename;
            log(ALARM_I_BIN_MOD, "localhost", msg.c_str());
        }
        if (bevents[0].mask & IN_DELETE_SELF)
        {
            msg = "File=" + bevents[0].filename;
            log(ALARM_I_BIN_DEL, "localhost", msg.c_str());
        }

        // delete the event
        bevents.erase(bevents.begin());
    }

    // restart monitor in case unavailable files become available, log if it is the case
    int aux;
    string auxs;

    // record how many files were being watched before stopping
    sysFileMonitor->stop();
    nSysFileWatchedNames = sysFileMonitor->getWatchedNameCount();
    for (int i = 1; i <= nSysFileWatchedNames; i++)
    {
        sysFileWatchedNames[i] = sysFileMonitor->getWatchedName(i);
    }

    // restart monitor, if a file was added the count will be bigger
    sysFileMonitor->start(sysFileMonitorFilename + ".log", sysFileMonitorFilename + ".out");
    aux = sysFileMonitor->getWatchedNameCount();

    // if number of watched system files increased this loop will execute
    for (int x = aux - nSysFileWatchedNames; x > 0;)
    {
        for (int i = 1; i <= aux; i++)
        {
            auxs = sysFileMonitor->getWatchedName(i);
            bool present = false;
            for (int j = 1; j <= nSysFileWatchedNames; j++)
            {
                if (strcmp(auxs.c_str(), sysFileWatchedNames[j].c_str()) == 0)
                {
                    present = true;
                    break;
                }
            }

            if (!present)
            {
                // add log
                msg = "File=" + auxs;
                log(ALARM_I_FILE_CREATE, "localhost", msg.c_str());
                // decrement amount of extra files and start check again
                x--;
                break;
            }
        }
    }
    // update number of watched system files
    nSysFileWatchedNames = aux;

    binMonitor->stop();
    nBinWatchedNames = sysFileMonitor->getWatchedNameCount();
    for (int i = 1; i <= nBinWatchedNames; i++)
    {
        sysFileWatchedNames[i] = sysFileMonitor->getWatchedName(i);
    }

    // restart monitor, if a file was added the count will be bigger
    binMonitor->start(binMonitorFilename + ".log", binMonitorFilename + ".out");
    aux = binMonitor->getWatchedNameCount();

    // if number of watched binaries increased this loop will execute
    for (int x = aux - nBinWatchedNames; x > 0;)
    {
        for (int i = 1; i <= aux; i++)
        {
            auxs = binMonitor->getWatchedName(i);
            bool present = false;
            for (int j = 1; j <= nBinWatchedNames; j++)
            {
                if (strcmp(auxs.c_str(), binWatchedNames[j].c_str()) == 0)
                {
                    present = true;
                    break;
                }
            }

            if (!present)
            {
                // add log
                msg = "File=" + auxs;
                log(ALARM_I_FILE_CREATE, "localhost", msg.c_str());
                // decrement amount of extra files and start check again
                x--;
                break;
            }
        }
    }
    // update number of watched binaries
    nBinWatchedNames = aux;

    return 0;
}

ModuleI::ModuleI()
{
    detector = DetectorI();
    // no preventer
}

void ModuleI::setup()
{
    if (detector.setup() == -1)
        cout << "Overloading didn't work\n";
}

void ModuleI::run()
{
    // detector scan
    detector.scan();

    // preventer act
}

void ModuleI::stop()
{
    // stop the detector if necessary
    detector.sysFileMonitor->stop();
    detector.binMonitor->stop();
}
