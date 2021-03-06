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
    char *npass = web_get_from_query_string(qs, (char*)"npass");
    char *mpass = web_get_from_query_string(qs, (char*)"mpass");
    char *role = web_get_from_query_string(qs, (char*)"role");
    char *email = web_get_from_query_string(qs, (char*)"email");
    
    // check if password is the same as confirmed
    if (strcmp(npass, mpass))
    {
        // confirmation different
        // back to main page with error message
        printf("<meta http-equiv=\"refresh\" content=\"0; URL=/cgi-bin/main?u=%s&s=%s&section=addu&e=2\" />", user, session);
        return;
    }

    // add user
    int res = pg_add_user(username, crypt(npass, crypt_gensalt("$6$", 0, NULL, 0)), role, email);

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

static void change_pass_(char **qs)
{
    // get query variables
    char *user = web_get_from_query_string(qs, (char*)"user");
    char *session = web_get_from_query_string(qs, (char*)"session");
    char *cpass = web_get_from_query_string(qs, (char*)"cpass");
    char *npass = web_get_from_query_string(qs, (char*)"npass");
    char *mpass = web_get_from_query_string(qs, (char*)"mpass");

    // check if password is the same as confirmed
    if (strcmp(npass, mpass))
    {
        // confirmation different
        // back to main page with error message
        printf("<meta http-equiv=\"refresh\" content=\"0; URL=/cgi-bin/main?u=%s&s=%s&section=pass&e=2\" />", user, session);
    } else
    // check current password
    if (pg_check_pass(user, cpass) == 1)    // password correct
    {
        // store new hash
        pg_change_pass(user, crypt(npass, crypt_gensalt("$6$", 0, NULL, 0)));

        // back to main page with confirmation message
        printf("<meta http-equiv=\"refresh\" content=\"0; URL=/cgi-bin/main?u=%s&s=%s&section=pass&e=-1\" />", user, session);
    } else
    {
        // password incorrect
        // back to main page with error message
        printf("<meta http-equiv=\"refresh\" content=\"0; URL=/cgi-bin/main?u=%s&s=%s&section=pass&e=1\" />", user, session);
    }
}

// action: 0 for change pass, 1 for change role
static void edit_user_(char **qs)
{
    // check if changing own password
    char *own = web_get_from_query_string(qs, (char*)"own");
    if (own)
    {
        change_pass_(qs);
        return;
    }

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
        char *mpass = web_get_from_query_string(qs, (char*)"mpass");
        if (strcmp(npass, mpass))
        {
            // confirmation different
            // back to main page with error message
            printf("<meta http-equiv=\"refresh\" content=\"0; URL=/cgi-bin/main?u=%s&s=%s&section=pass&e=2\" />", user, session);
        } else if (npass && strlen(npass))
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
    } else if (strcmp(action, "Update email") == 0)
    {
        char *email = web_get_from_query_string(qs, (char*)"email");
        printf("Got here");
        res = pg_set_email(username, email);
        e = 4;
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

static void delete_user_(char **qs)
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

    char *username = web_get_from_query_string(qs, (char*)"u");
    
    // delete user
    int res = pg_delete_user(username);
    if (res == -2) res = -3;

    if (res == 0)
    {
        // back to main page with confirmation message
        printf("<meta http-equiv=\"refresh\" content=\"0; URL=/cgi-bin/main?u=%s&s=%s&section=editu&e=3\" />", user, session);
    } else
    {
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
        printf ("</body>");
        return 1;
    }

    if (strcmp(action, "Add user") == 0)
    {
        add_user_(qs);
    } else if (strcmp(action, "Update password") == 0 || strcmp(action, "Update role") == 0 || strcmp(action, "Update email") == 0)
    {
        edit_user_(qs);
    } else if (strcmp(action, "Delete user") == 0)
    {
        delete_user_(qs);
    }

    printf("</body>");
}