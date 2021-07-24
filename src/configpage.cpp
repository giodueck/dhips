#include "web.h"
#include "pgsql.h"
#include "login.h"
#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <string>
#include <vector>
#include "alarms.h"

void conf1(char **qs)
{
    char *enablesys = web_get_from_query_string(qs, (char*)"enablesys");
    char *enablebin = web_get_from_query_string(qs, (char*)"enablebin");
    char *enablesysall = web_get_from_query_string(qs, (char*)"enablesysall");
    char *enablebinall = web_get_from_query_string(qs, (char*)"enablebinall");

    std::vector<std::string> sysNames, binNames;
    std::vector<bool> sysActive, binActive;
    char *dest;
    int active;
    int i = 0, j = 0;

    // get all filenames and their current configuration
    do
    {
        // system files
        i = pg_get_monitor_filename_conf(i, SYSFILE_TYPE, &dest, &active);
        if (i > 0)
        {
            sysNames.push_back(std::string(dest));
            if (active == 1) sysActive.push_back(true);
            else sysActive.push_back(false);
        }

        // binaries
        j = pg_get_monitor_filename_conf(j, BINARY_TYPE, &dest, &active);
        if (j > 0)
        {
            binNames.push_back(std::string(dest));
            if (active == 1) binActive.push_back(true);
            else binActive.push_back(false);
        }
    } while (i > 0 || j > 0);

    // toggle all
    int sfc = sysNames.size(), bc = binNames.size();
    bool sfdone = false, bdone = false;
    if (!enablesys)
    {
        // disable all
        for (i = 0; i < sfc; i++)
            sysActive[i] = false;
        sfdone = true;
    }
    if (!enablebin)
    {
        for (j = 0; j < sfc; j++)
            binActive[j] = false;
        bdone = true;
    }

    if (!sfdone && enablesysall)
    {
        // disable all
        for (i = 0; i < sfc; i++)
            sysActive[i] = true;
        sfdone = true;
    }
    if (!bdone && enablebinall)
    {
        for (j = 0; j < sfc; j++)
            binActive[j] = false;
        bdone = true;
    }

    if (!sfdone)
    {
        // enable all
        for (i = 0; i < sfc; i++)
            sysActive[i] = false;

        // go through all the files
        char *si;
        char aux[1024];
        for (i = 0; i < sfc; i++)
        {
            sprintf(aux, "s%d", i);
            char *si = web_get_from_query_string(qs, aux);

            // si argument only exists if the checkbox was checked
            if (si)
                sysActive[i] = true;
            else
                sysActive[i] = false;
        }
    }
    if (!bdone)
    {
        // enable all
        for (j = 0; j < sfc; j++)
            binActive[j] = false;

        // go through all the files
        char *bj;
        char aux[1024];
        for (j = 0; j < sfc; j++)
        {
            sprintf(aux, "b%d", j);
            char *bj = web_get_from_query_string(qs, aux);

            // bj argument only exists if the checkbox was checked
            if (bj)
                binActive[j] = true;
            else
                binActive[j] = false;
        }
    }

    // set configurations
    for (i = 0; i < sfc; i++)
        pg_set_monitor_filename_conf(i, SYSFILE_TYPE, sysNames[i].c_str(), sysActive[i]);
    for (j = 0; j < sfc; j++)
        pg_set_monitor_filename_conf(j, BINARY_TYPE, binNames[j].c_str(), binActive[j]);
    return;
}

int main()
{
    web_print_header();
    
    // Check variables to print errors
    char **qs = web_get_post_string();
    char *user = web_get_from_query_string(qs, (char*)"user");
    char *session = web_get_from_query_string(qs, (char*)"session");
    char *mod = web_get_from_query_string(qs, (char*)"mod");
    char *enable = web_get_from_query_string(qs, (char*)"enable");

    printf("<body>");
    
    // check if logged in
    if (!user || !session || pg_check_session(user, atoi(session), SESSION_LIFETIME) != 1)
    {
        printf("<meta http-equiv=\"refresh\" content=\"0; URL=/cgi-bin/login?u=%s\" />", user);
        printf ("</body>");
        return 1;
    }

    // toggle module
    if (enable)
        pg_module_toggle(atoi(mod), 1);
    else
        pg_module_toggle(atoi(mod), 0);

    // apply settings
    switch (atoi(mod))
    {
    case 1:
        conf1(qs);
        break;
    
    default:
        break;
    }

    printf("<h3>Settings changed. To apply click \"Reload modules\"</h3>");
    printf("<a href=\"/cgi-bin/main?u=%s&s=%s&section=conf&r=true\">", user, session);
    printf("Reload modules");
    printf("</a> | ");
    printf("<a href=\"/cgi-bin/main?u=%s&s=%s\">Home</a>", user, session);
    
    printf("</body>");

    return 0;
}