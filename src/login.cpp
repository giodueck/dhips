#include <iostream>
#include <string>
#include <unistd.h>
#include "login.h"
#include "web.h"
#include "pgsql.h"

using namespace std;

int login()
{
    char **qs = web_get_post_string();

	char *user = web_get_from_query_string(qs, (char*)"user");
	char *password = web_get_from_query_string(qs, (char*)"password");

    int res;

    web_print_header();
    cout << "<body>";

	if (user && user[0] != '\0')
    {
        cout << "<p>";

        res = check_pass(user, password);
        if (res == 0)
        {
            // invalid password
            cout << "<meta http-equiv=\"refresh\" content=\"0; URL=/cgi-bin/login?e=1&u=" << user << "\" />";
        } else if (res == 1)
        {
            // successful login
            cout << "Logged in";
        } else if (res == -1)
        {
            // Invalid user
            cout << "<meta http-equiv=\"refresh\" content=\"0; URL=/cgi-bin/login?e=2&u=" << user << "\" />";
        } else
        {
            // other error
            cout << "An error occurred, check the error log";
        }

        cout << "</p>";
	} else
    {
        // html should prevent this
		cout << "<h1>No username given</h1>";
	}

    cout << "</body>";

    return 0;
}