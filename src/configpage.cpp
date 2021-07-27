#include "web.h"
#include "pgsql.h"
#include "login.h"
#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <string>
#include <vector>
#include "defines.h"

void conf1(char **qs)
{
    char *enablesys = web_get_from_query_string(qs, (char*)"enablesys");
    char *enablebin = web_get_from_query_string(qs, (char*)"enablebin");
    char *filename = web_get_from_query_string(qs, (char*)"filename");
    char *action = web_get_from_query_string(qs, (char*)"action");
    char *type = web_get_from_query_string(qs, (char*)"type");

    char *dest;
    int active;
    int i = 0, j = 0;
    int res = 0;

    // check for and apply edit list configuration
    if (filename)
    {
        // if no action or action is not remove and no filetype specified
        if (!action || (!type && strcmp(action, "remove")))
        {
            printf("<p>Select 'Add' and a filetype to add to the watchlist<br>");
            printf("or select 'Remove' to remove from the watchlist</p>");
        } else
        {
            if (strcmp(action, "remove") == 0)
            {
                res = pg_rm_monitor_filename(filename);
                if (res == 0)
                {
                    printf("<p>%s removed from watchlist</p>", filename);
                } else if (res == 1)
                {
                    printf("<p>%s not in watchlist</p>", filename);
                }
            } else if (strcmp(action, "add") == 0)
            {
                res = pg_add_monitor_filename((strcmp(type, "sys") == 0) ? SYSFILE_TYPE : BINARY_TYPE, filename);
                if (res == 0)
                {
                    printf("<p>%s added to %s file watchlist</p>", filename, (strcmp(type, "sys") == 0) ? "system" : "binary");
                }
            }
        }
    }

    // toggle all
    if (!enablesys)
    {
        // disable all
        pg_toggle_monitor_filetype(SYSFILE_TYPE, 0);
        printf("<p>System file monitoring off</p>");
    } else
    {
        // enable all
        pg_toggle_monitor_filetype(SYSFILE_TYPE, 1);
        printf("<p>System file monitoring on</p>");
    }

    if (!enablebin)
    {
        // disable all
        pg_toggle_monitor_filetype(BINARY_TYPE, 0);
        printf("<p>Binary file monitoring off</p>");
    } else
    {
        // enable all
        pg_toggle_monitor_filetype(BINARY_TYPE, 1);
        printf("<p>Binary file monitoring on</p>");
    }

    return;
}

void conf3(char **qs)
{
    char *procname = web_get_from_query_string(qs, (char*)"procname");
    char *action = web_get_from_query_string(qs, (char*)"action");

    char *dest;
    int active;
    int i = 0, j = 0;
    int res = 0;

    // check for and apply edit list configuration
    if (procname)
    {
        // if no action specified
        if (!action)
        {
            printf("<p>Select 'Add' to add to the watchlist<br>");
            printf("or select 'Remove' to remove from the watchlist</p>");
        } else
        {
            if (strcmp(action, "remove") == 0)
            {
                res = pg_rm_targeted_proc(SNIFFER_TYPE, procname);
                if (res == 0)
                {
                    printf("<p>%s removed from sniffer watchlist</p>", procname);
                } else if (res == 1)
                {
                    printf("<p>%s not in sniffer watchlist</p>", procname);
                }
            } else if (strcmp(action, "add") == 0)
            {
                res = pg_add_targeted_proc(SNIFFER_TYPE, procname);
                if (res == 0)
                {
                    printf("<p>%s added to sniffer watchlist</p>", procname);
                }
            }
        }
    }

    return;
}

void conf7(char **qs)
{
    char *name = web_get_from_query_string(qs, (char*)"name");
    char *action = web_get_from_query_string(qs, (char*)"action");
    char *type = web_get_from_query_string(qs, (char*)"type");

    char *dest;
    int active;
    int i = 0, j = 0;
    int res = 0;

    // check for and apply edit list configuration
    if (name)
    {
        // if no action specified
        if (!action || !type)
        {
            printf("<p>Select 'Add' and a category to add to the whitelist or watchlist<br>");
            printf("or select 'Remove' and a category to remove from the whitelist or watchlist</p>");
        } else if (strcmp(type, "whitelist") == 0)
        {
            if (strcmp(action, "remove") == 0)
            {
                res = pg_rm_targeted_proc(WHITELISTED_TYPE, name);
                if (res == 0)
                {
                    printf("<p>%s removed from whitelist</p>", name);
                } else if (res == 1)
                {
                    printf("<p>%s not in whitelist</p>", name);
                }
            } else if (strcmp(action, "add") == 0)
            {
                res = pg_add_targeted_proc(WHITELISTED_TYPE, name);
                if (res == 0)
                {
                    printf("<p>%s added to whitelist</p>", name);
                }
            }
        } else if (strcmp(type, "watchlist") == 0)
        {
            if (strcmp(action, "remove") == 0)
            {
                res = pg_rm_targeted_ext(name);
                if (res == 0)
                {
                    printf("<p>%s removed from extension watchlist</p>", name);
                } else if (res == 1)
                {
                    printf("<p>%s not in extension watchlist</p>", name);
                }
            } else if (strcmp(action, "add") == 0)
            {
                res = pg_add_targeted_ext(name);
                if (res == 0)
                {
                    printf("<p>%s added to extension watchlist</p>", name);
                }
            }
        }
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
    case 2:
        break;
    case 3:
        conf3(qs);
        break;
    case 7:
        conf7(qs);
        break;
    
    default:
        break;
    }

    pg_set_config_changed(1);

    printf("<h3>Settings applied</h3>");
    printf("<a href=\"/cgi-bin/main?u=%s&s=%s&section=conf\">", user, session);
    printf("Back to settings");
    printf("</a> | ");
    printf("<a href=\"/cgi-bin/main?u=%s&s=%s\">Home</a>", user, session);
    
    printf("</body>");

    return 0;
}