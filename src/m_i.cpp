#include "m_i.h"
#include "pgsql.h"
#include <iostream>

using namespace std;

ModuleI::DetectorI::DetectorI()
{
    this->module = "I";
    sysFileMonitor = new Monitor("I");
    this->preventer = NULL; // no preventing for modifying files
}

int ModuleI::DetectorI::setup()
{
    int i = 0;
    char *dest;

    // get all monitoring filenames
    do
    {
        i = pg_get_monitor_filename(i, I_SYSFILE_TYPE, &dest);
        if (i > 0)
        {
            sysFileMonitor->addWatch(string(dest));
        }
    } while (i > 0);

    // start monitoring
    sysFileMonitor->start(string("/var/log/hips/monitor.log"), string("/var/log/hips/monitor.out"));

    return 0;
}

int ModuleI::DetectorI::scan()
{
    // get reports
    vector<struct MonitorEvent> events;
    sysFileMonitor->extractEvents(events, string("/var/log/hips/monitor.log"));

    // process events
    string msg;
    while (events.size())
    {
        // log an event
        if (events[0].mask & IN_MODIFY)
        {
            msg = "File=" + events[0].filename;
            log(ALARM_I_FILE_MOD, "localhost", msg.c_str());
        }
        if (events[0].mask & IN_DELETE_SELF)
        {
            msg = "File=" + events[0].filename;
            log(ALARM_I_FILE_DEL, "localhost", msg.c_str());
        }

        // delete the event
        events.erase(events.begin());
    }

    // restart monitor in case unavailable files become available
    sysFileMonitor->stop();
    sysFileMonitor->start(string("/var/log/hips/monitor.log"), string("/var/log/hips/monitor.out"));

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
