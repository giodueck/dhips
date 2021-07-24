#include "web.h"
#include "pgsql.h"
#include "login.h"
#include <stdio.h>
#include <string.h>

void conf1(char **qs)
{

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
    
    
    printf("</body>");

    return 0;
}