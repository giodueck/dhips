#include "monitor.h"
#include "dhipslib.h"

using namespace std;

Monitor::Monitor(const char *module)
{
    this->module = module;
    watchNames = vector<string>();
    notifyfd = -1;
    n = -1;
    p = NULL;
    pid = -1;
    mask = IN_MODIFY | IN_DELETE_SELF;
}

bool Monitor::addWatch(const string filename)
{
    n = watchNames.size();
    for (int i = 0; i < n; i++)
    {
        // look for filename in watchNames
        if (strcmp(watchNames[i].c_str(), filename.c_str()) == 0)
        {
            // found
            return false;
        }
    }
    // not found
    watchNames.push_back(filename);
    return true;
}

bool Monitor::rmWatch(const string filename)
{
    n = watchNames.size();
    for (int i = 0; i < n; i++)
    {
        // look for filename in watchNames
        if (strcmp(watchNames[i].c_str(), filename.c_str()) == 0)
        {
            // found
            watchNames.erase(watchNames.begin() + i);
            return true;
        }
    }
    // not found
    return false;
}

void Monitor::setMask(uint32_t mask)
{
    this->mask = mask;
}

string Monitor::getWatchedName(int watchfd)
{
    return string(watchedNames[watchfd]);
}

int Monitor::start(string logfile, string outfile)
{
    // check if already started
    if (notifyfd > 0) return 1;

    struct stat sb;
    const char *fn;
    int watchfd;
    FILE *fout;

    // start inotify process
    notifyfd = inotify_init();

    // out file
    out = outfile;
    log = logfile;
    if (!out.empty())
    {
        fout = fopen(out.c_str(), "w");
        if (!fout)
        {
            char msg[128];
            sprintf(msg, "%s: could not open monitor output file %s", module, out.c_str());
            dhips_perror(msg);
            return -1;
        }
    }
    
    // add filenames to the watchlist
    int n_ = watchNames.size();
    for (int i = 0; i < n_; i++)
    {
        fn = watchNames.at(i).c_str();

        // test filename
        if (stat(fn, &sb) < 0)
        {
            if (!out.empty()) fprintf(fout, "start: could not stat %s, ignored\n", fn);
            continue;
        }

        // test file
        if (S_ISREG(sb.st_mode))
        {
            // regular file, add to watch list
            if ((watchfd = inotify_add_watch(notifyfd, fn, mask)) < 0 )
            {
                if (!out.empty()) fprintf(fout, "start: error adding watch for %s\n", fn);
            } else
            {
                if (!out.empty()) fprintf(fout, "start: added %s to watch list with watch descriptor %d\n", fn, watchfd);
                // add to watched names
                strcpy(watchedNames[watchfd], fn);
            }
        } else // probably a directory
        {
            if (!out.empty()) fprintf(fout, "start: %s is not a regular file, ignored\n", fn);
        }
    }

    fclose(fout);
    // Watches are in place, fork and start monitoring

    // create a temporary file to store all the monitor bytes read
    tmpf = string("/tmp/monitor") + to_string((long)time(0));

    // fork
    pid = fork();
    if (pid == -1)
    {
        char msg[128];
        sprintf(msg, "%s: fork", module);
        dhips_perror(msg);
        return -1;
    } else if (pid)
    {
        return 0;
    }

    // monitor
    struct inotify_event *event;
    FILE *tmpfd;
    FILE *logfd = fopen(log.c_str(), "a");
    while (1)
    {
        n = read(notifyfd, eventBuf, EVENT_BUFSIZE);

        // Loop over events
        for (p = eventBuf; p < eventBuf + n;)
        {
            event = (struct inotify_event*)p;
            // increment p manually because of variable struct inotify_event size
            p += sizeof(struct inotify_event) + event->len;
            // log events
            if (event->mask & IN_MODIFY) fprintf(logfd, "%s was modified\n", watchedNames[event->wd]);
            if (event->mask & IN_DELETE_SELF) fprintf(logfd, "%s was deleted\n", watchedNames[event->wd]);
            fflush(logfd);
        }
    }
}

int Monitor::stop()
{
    // check if running
    if (notifyfd == -1) return 1;

    // close file descriptors
    kill(pid, SIGKILL);
    close(notifyfd);
    notifyfd = -1;
    pid = -1;

    // The notify process terminates when all related file descriptors are closed 
    return 0;
}

bool Monitor::isMonitoring()
{
    return (notifyfd > 0);
}

int Monitor::read_()
{
    FILE *tmpfd;

    // open tmpf
    tmpfd = fopen(tmpf.c_str(), "r");
    if (!tmpfd) return 0;
    n = read(notifyfd, eventBuf, EVENT_BUFSIZE);
    fclose(tmpfd);
    remove(tmpf.c_str());
    return n;
}

bool Monitor::eventAvailable()
{
    return (p < eventBuf + n);
}

int Monitor::extractEvents(vector<struct inotify_event*> &events)
{
    // check if buffer has events, otherwise try to read from notifyfd
    // Loop over events
    int i;
    for (i = 0, p = eventBuf; p < eventBuf + n; i++)
    {
        events.push_back((struct inotify_event*)p);
        // increment p manually because of variable struct inotify_event size
        p += sizeof(struct inotify_event) + events[i]->len;
    }
    return i;
}
