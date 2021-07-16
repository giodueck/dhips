#include "pgsql.h"
#include "web.h"
#include <stdlib.h>
#include <stdio.h>

// Ends the current session and returns to the login page
int main()
{
    // get session from query string
    char **qs = web_get_query_string();
    char *sid = web_get_from_query_string(qs, (char*)"s");

    terminate_session(atoi(sid));

    web_print_header();
    printf("<html><body>\n");
    printf("<meta http-equiv=\"refresh\" content=\"0; URL=/cgi-bin/login\" />\n");
    printf("</body></html>\n");
}