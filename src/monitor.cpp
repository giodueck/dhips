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
    nWatchedNames = 0;
}

Monitor::Monitor()
{
    this->module = string("").c_str();
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

int Monitor::getWatchedNameCount()
{
    return nWatchedNames;
}

// Monitor child SIGINT (^C) handler
static void break_handler(int sig)
{
    pid_t pid = getpid();
    // send sigkill to itself to get rid of the stopped process
    kill(pid, SIGKILL);
    exit(0);
}

int Monitor::start(string logfile, string outfile)
{
    // check if already started
    if (pid > 0) return 1;

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
    nWatchedNames = 0;
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
                nWatchedNames++;
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
        close(notifyfd);
        notifyfd = -1;
        return -1;
    } else if (pid)
    {
        close(notifyfd);
        notifyfd = -1;
        return 0;
    }

    // set up break signal handling
    struct sigaction sigbreak;

    sigbreak.sa_handler = break_handler;
    sigemptyset (&sigbreak.sa_mask);
    sigbreak.sa_flags = 0;
    if (sigaction(SIGINT, &sigbreak, NULL) != 0) dhips_perror("sigaction");

    // monitor
    struct inotify_event *event;
    FILE *tmpfd;
    FILE *logfd = fopen(log.c_str(), "w");
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
            if (event->mask & IN_MODIFY) fprintf(logfd, "IN_MODIFY;%s\n", watchedNames[event->wd]);
            if (event->mask & IN_DELETE_SELF) fprintf(logfd, "IN_DELETE_SELF;%s\n", watchedNames[event->wd]);
            fflush(logfd);
        }
    }
}

int Monitor::stop()
{
    // check if running
    if (pid == -1) return 1;

    // kill child and close file descriptors 
    int status;
    kill(pid, SIGINT);
    waitpid(pid, &status, 0);
    pid = -1;

    // The notify process terminates when all related file descriptors are closed 
    return 0;
}

bool Monitor::isMonitoring()
{
    return (notifyfd > 0);
}

bool Monitor::eventAvailable()
{
    return (p < eventBuf + n);
}

int Monitor::extractEvents(vector<struct MonitorEvent> &events, string logfile)
{
    // read the log file
    int i;
    char buf[BUFSIZ];
    char *tok;
    struct MonitorEvent event;
    FILE *log = fopen(logfile.c_str(), "r");
    if (!log)
    {
        char msg[128];
        sprintf(msg, "%s: could not open %s", module, logfile.c_str());
        dhips_perror(msg);
        return -1;
    }

    // read log
    while (fgets(buf, BUFSIZ, log) != NULL)
    {
        tok = strtok(buf, ";\n");
        
        // event mask
        event.mask = 0;
        if (strcmp(tok, "IN_MODIFY") == 0)
            event.mask = IN_MODIFY;
        else if (strcmp(tok, "IN_DELETE_SELF") == 0)
            event.mask = IN_DELETE_SELF;
        
        // filename
        tok = strtok(NULL, ";\n");
        event.filename = string(tok);

        // cookie is there in case i ever need it
        event.cookie = 0;

        // add event to events
        events.push_back(event);
    }

    fclose(log);
    return i;
}
