#include "m_ii.h"

using namespace std;

// true if data is not already in baseline
static bool m_ii_newLogin(vector<struct utmpx> baseline, struct utmpx data)
{
    // look for identical entries
    for (int i = 0; i < baseline.size(); i++)
    {
        // if login time is the same, most likely not new
        if (baseline[i].ut_tv.tv_sec == data.ut_tv.tv_sec)
        {
            // if login is identical, definitely not new
            string buser = baseline[i].ut_user;
            string duser = data.ut_user;
            if (strcmp(buser.c_str(), duser.c_str()) == 0 && baseline[i].ut_addr_v6[0] == data.ut_addr_v6[0])
            {
                return false;
            }
        }
    }

    // if nothing matches, it is new
    return true;
}

// Dud functions
void ModuleII::stop(){}

ModuleII::ModuleII()
{
    
}

void ModuleII::setup()
{
    detector.setup();
}

void ModuleII::run()
{
    detector.scan();
}

ModuleII::DetectorII::DetectorII()
{
    this->module = "II";
    this->preventer = NULL; // no preventing for users already logged in
    vector<struct utmpx> logins = vector<struct utmpx>();
}

int ModuleII::DetectorII::setup()
{
    // check all the users logged when setup was called to get a baseline
    struct utmpx data;
    FILE *log_file = fopen(_PATH_UTMP, "r");
    memset(&data, 0, sizeof(struct utmpx));

    while(fread(&data, sizeof(struct utmpx), 1, log_file) == 1)
    {
        baseline.push_back(data);
    }
    fclose(log_file);

    return 0;
}

int ModuleII::DetectorII::scan()
{
    struct utmpx data;
    FILE *log_file = fopen(_PATH_UTMP, "r");
    memset(&data, 0, sizeof(struct utmpx));

    bool changed = false;
    bool loggedout = false;
    string aux;
    int i = 0;

    while(fread(&data, sizeof(struct utmpx), 1, log_file) == 1)
    {
        if (m_ii_newLogin(baseline, data))
        {
            // check if actually logged out
            aux = string(data.ut_user);
            if (strcmp(aux.c_str(), "LOGIN") == 0 || data.ut_user[0] == '\0')
            {
                loggedout = true;
            }
            // we want the user, not LOGIN or an empty string
            if (data.ut_user[0])
                aux = "User: " + string(baseline[i].ut_user) + "\tLine:" + string(data.ut_line);
            else
                aux = "User: " + string(data.ut_user) + "\tLine:" + string(data.ut_line);

            if (data.ut_addr_v6[0] != 0)
            {
                // work out IP
                unsigned char bytes[4];
                char location[16];

                bytes[0] = data.ut_addr_v6[0] & 0xFF;
                bytes[1] = (data.ut_addr_v6[0] >> 8) & 0xFF;
                bytes[2] = (data.ut_addr_v6[0] >> 16) & 0xFF;
                bytes[3] = (data.ut_addr_v6[0] >> 24) & 0xFF;

                sprintf(location, "%d.%d.%d.%d", bytes[0], bytes[1], bytes[2], bytes[3]);
                if (loggedout)
                    log(ALARM_II_USER_LOGGED_OUT, location, aux.c_str());
                else 
                    log(ALARM_II_USER_LOGGED_IN, location, aux.c_str());
            } else
            {
                if (loggedout)
                    log(ALARM_II_USER_LOGGED_OUT, "localhost", aux.c_str());
                else 
                    log(ALARM_II_USER_LOGGED_IN, "localhost", aux.c_str());
            }
            changed = true;
        }
        i++;
    }
    fclose(log_file);

    if (changed)
    {
        setup();
    }

    return 0;
}