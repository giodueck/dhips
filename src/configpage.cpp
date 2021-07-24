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

    char *dest;
    int active;
    int i = 0, j = 0;

    // if no file was added manually
    // toggle all
    if (!enablesys)
    {
        // disable all
        pg_toggle_monitor_filetype(SYSFILE_TYPE, 0);
    } else
    {
        // enable all
        pg_toggle_monitor_filetype(SYSFILE_TYPE, 1);
    }

    if (!enablebin)
    {
        // disable all
        pg_toggle_monitor_filetype(BINARY_TYPE, 0);
    } else
    {
        // enable all
        pg_toggle_monitor_filetype(BINARY_TYPE, 1);
    }

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
    char *enabled = web_get_from_query_string(qs, (char*)"enabled");

    printf("<body>");
    
    // check if logged in
    if (!user || !session || pg_check_session(user, atoi(session), SESSION_LIFETIME) != 1)
    {
        printf("<meta http-equiv=\"refresh\" content=\"0; URL=/cgi-bin/login?u=%s\" />", user);
        printf ("</body>");
        return 1;
    }

    // toggle module
    if (enabled)
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

    pg_set_config_changed(1);

    printf("<h3>Settings applied</h3>");
    printf("<a href=\"/cgi-bin/main?u=%s&s=%s&section=conf\">", user, session);
    printf("Settings");
    printf("</a> | ");
    printf("<a href=\"/cgi-bin/main?u=%s&s=%s\">Home</a>", user, session);
    
    printf("</body>");

    return 0;
}