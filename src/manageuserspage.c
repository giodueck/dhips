#include "login.h"
#include "pgsql.h"
#include "web.h"
#include <string.h>
#include <crypt.h>
#include <stdio.h>
#include <stdio.h>

static void add_user_(char **qs)
{
    char *user = web_get_from_query_string(qs, (char*)"user");
    char *session = web_get_from_query_string(qs, (char*)"session");

    // check if admin
    if (get_role(user) != 0)
    {
        printf("<meta http-equiv=\"refresh\" content=\"0; URL=/cgi-bin/main?u=%s&s=%s\" />", user, session);
        printf ("<body>");
        return;
    }

    // get credentials
    char *username = web_get_from_query_string(qs, (char*)"nuser");
    char *pass = web_get_from_query_string(qs, (char*)"npass");
    char *role = web_get_from_query_string(qs, (char*)"role");
    
    // add user
    int res = pg_add_user(username, crypt(pass, crypt_gensalt("$6$", 0, NULL, 0)), role);

    if (res == 0)
    {
        // back to main page with confirmation message
        printf("<meta http-equiv=\"refresh\" content=\"0; URL=/cgi-bin/main?u=%s&s=%s&section=addu&e=-1\" />", user, session);
    } else if (res == 1)
    {
        printf("<meta http-equiv=\"refresh\" content=\"0; URL=/cgi-bin/main?u=%s&s=%s&section=addu&e=1\" />", user, session);
    } else
    {
        printf("<meta http-equiv=\"refresh\" content=\"0; URL=/cgi-bin/main?u=%s&s=%s&section=addu&e=-2\" />", user, session);
    }
}

// action: 0 for change pass, 1 for change role
static void edit_user_(char **qs)
{
    char *user = web_get_from_query_string(qs, (char*)"user");
    char *session = web_get_from_query_string(qs, (char*)"session");
    int res, e;

    // check if admin
    if (get_role(user) != 0)
    {
        printf("<meta http-equiv=\"refresh\" content=\"0; URL=/cgi-bin/main?u=%s&s=%s\" />", user, session);
        printf ("<body>");
        return;
    }
    
    char *username = web_get_from_query_string(qs, (char*)"u");
    char *action = web_get_from_query_string(qs, (char*)"action");

    if (strcmp(action, "Update password") == 0)
    {
        char *npass = web_get_from_query_string(qs, (char*)"npass");
        if (npass && strlen(npass))
        {
            res = pg_change_pass(username, crypt(npass, crypt_gensalt("$6$", 0, NULL, 0)));
            e = 1;
        } else
        {
            res = -1;
        }
    } else if (strcmp(action, "Update role") == 0)
    {
        char *role = web_get_from_query_string(qs, (char*)"role");
        if (role && strlen(role))
        {
            res = pg_change_role(username, role);
            e = 2;
        } else
        {
            res = -2;
        }
    }

    if (res == 0)
    {
        // back to main page with confirmation message
        printf("<meta http-equiv=\"refresh\" content=\"0; URL=/cgi-bin/main?u=%s&s=%s&section=editu&e=%d\" />", user, session, e);
    } else
    {
        // back to main page with error message
        printf("<meta http-equiv=\"refresh\" content=\"0; URL=/cgi-bin/main?u=%s&s=%s&section=editu&e=%d\" />", user, session, res);
    }
}

int main()
{
    web_print_header();

    // get query variables
    char **qs = web_get_post_string();
    char *user = web_get_from_query_string(qs, (char*)"user");
    char *session = web_get_from_query_string(qs, (char*)"session");
    char *action = web_get_from_query_string(qs, (char*)"action");

    printf("<body>");

    // check if logged in
    if (pg_check_session(user, atoi(session), SESSION_LIFETIME) != 1)
    {
        printf("<meta http-equiv=\"refresh\" content=\"0; URL=/cgi-bin/login?u=%s\" />", user);
        printf ("<body>");
        return 1;
    }

    if (strcmp(action, "adduser") == 0)
    {
        add_user_(qs);
    } else if (strcmp(action, "Update password") == 0 || strcmp(action, "Update role") == 0)
    {
        edit_user_(qs);
    }

    printf("</body>");
}