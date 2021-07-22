// The Monitor class has methods for starting and manipulating inotify instances
// Based on code from https://youtu.be/J8WU1ELkuyM

#ifndef MONITOR_H
#define MONITOR_H

#include <sys/stat.h>
#include <sys/inotify.h>
#include <sys/types.h>
#include <signal.h>
#include <wait.h>
#include <unistd.h>
#include <string>
#include <cstring>
#include <limits.h>
#include <vector>

// Space to store at least 100 events
#define EVENT_BUFSIZE (100 * sizeof(struct inotify_event) + NAME_MAX + 1)

// Stores information about an event
struct MonitorEvent
{
    uint32_t mask;
    uint32_t cookie;
    std::string filename;
};

class Monitor
{
    private:

    const char *module;

    // Watched filenames
    std::vector<std::string> watchNames;
    // inotify file descriptor
    int notifyfd;
    // child process
    pid_t pid;
    // output file
    std::string out;
    std::string log;
    std::string tmpf;

    uint32_t mask;

    // events are read into here
    char eventBuf[EVENT_BUFSIZE];
    // for read()
    int n;
    // points to the next inotify_event in eventBuf
    char *p;
    // watched names indexed by their watchfd
    char watchedNames[100][NAME_MAX + 1];

    public:

    Monitor();

    // Constructor
    Monitor(const char *module);

    // Add file to watch. When calling start, invalid filenames or non-regular files will be ignored
    //  Returns true if filename was added, false if it was already present
    bool addWatch(const std::string filename);

    // Remove file from watch
    //  Returns true if a filename was erased, false if it was not present
    bool rmWatch(const std::string filename);

    // Set which events are to cause events
    void setMask(uint32_t mask);

    // Get a filename with watchfd
    std::string getWatchedName(int watchfd);

    // Start an inotify instance as a child process and start monitoring. Applies watch configurations
    // Use eventAvailable and getEvent to get events. Use stop to stop monitoring.
    // logfile is the log output file, outfile is an optional file into which the child outputs text.
    //  Returns 0 if successful, 1 if already started, negatives if not
    int start(std::string logfile, std::string outfile = "");

    // Stop the inotify instance, i.e. kill the child and stop monitoring.
    // Any events not read into the internal buffer will be lost.
    //  Returns 0 if successful, 1 if not running, negatives if error
    int stop();

    //  Returns true if started, false if stopped
    bool isMonitoring();

    // Returns true if a new event is readable, false if not
    bool eventAvailable();

    // Attempts to read all available events into events. The events are stored in an internal buffer that is overwritten
    // every time this function is called.
    //  Returns the number of events read
    int extractEvents(std::vector<struct MonitorEvent> &events, std::string logfile);
};

#endif // MONITOR_H