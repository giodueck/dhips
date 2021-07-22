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

    // // testing code
    // for (int i = 0; i < baseline.size(); i++)
    // {
        // unsigned char bytes[4];

        // bytes[0] = data.ut_addr_v6[0] & 0xFF;
        // bytes[1] = (data.ut_addr_v6[0] >> 8) & 0xFF;
        // bytes[2] = (data.ut_addr_v6[0] >> 16) & 0xFF;
        // bytes[3] = (data.ut_addr_v6[0] >> 24) & 0xFF;
    //     printf("Read a record, User : %s\n", data.ut_user);
    //     printf("               Line : %s\n", data.ut_line);
    //     printf("               IP   : %x.%x.%x.%x\n", (unsigned char)bytes[0], (unsigned char)bytes[1], (unsigned char)bytes[2], (unsigned char)bytes[3]);
    // }

    return 0;
}

int ModuleII::DetectorII::scan()
{
    struct utmpx data;
    FILE *log_file = fopen(_PATH_UTMP, "r");
    memset(&data, 0, sizeof(struct utmpx));

    while(fread(&data, sizeof(struct utmpx), 1, log_file) == 1)
    {
        if (m_ii_newLogin(baseline, data))
        {
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
                log(ALARM_II_USER_LOGGED_IN, location);
            } else
            {
                log(ALARM_II_USER_LOGGED_IN, "localhost");
            }
        }
    }
    fclose(log_file);

    return 0;
}