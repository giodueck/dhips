#include "login.h"
#include "pgsql.h"
#include "web.h"
#include <string.h>
#include <crypt.h>
#include <stdio.h>

int main()
{
    web_print_header();

    // get query variables
    char **qs = web_get_post_string();
    char *user = web_get_from_query_string(qs, (char*)"user");
    char *session = web_get_from_query_string(qs, (char*)"session");
    char *cpass = web_get_from_query_string(qs, (char*)"cpass");
    char *npass = web_get_from_query_string(qs, (char*)"npass");

    printf("<body>");

    // check if logged in
    if (pg_check_session(user, atoi(session), SESSION_LIFETIME) != 1)
    {
        printf("<meta http-equiv=\"refresh\" content=\"0; URL=/cgi-bin/login?u=%s\" />", user);
        printf ("<body>");
        return 1;
    }

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

    printf("</body>");

    return 0;
}