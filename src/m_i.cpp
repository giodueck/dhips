#include "m_i.h"
#include "pgsql.h"
#include <iostream>

using namespace std;

ModuleI::DetectorI::DetectorI()
{
    sysFileMonitorFilename = string("/var/log/hips/sfmonitor");
    binMonitorFilename = string("/var/log/hips/bmonitor");
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

    // restart monitor in case unavailable files become available
    sysFileMonitor->stop();
    sysFileMonitor->start(sysFileMonitorFilename + ".log", sysFileMonitorFilename + ".out");
    binMonitor->stop();
    binMonitor->start(binMonitorFilename + ".log", binMonitorFilename + ".out");

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
