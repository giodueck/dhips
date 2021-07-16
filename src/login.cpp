#include <iostream>
#include <string>
#include <cstring>
#include <unistd.h>
#include "login.h"
#include "web.h"
#include "pgsql.h"

using namespace std;

int verify_session()
{
    // get username from query string
    char **qs = web_get_query_string();
    char *user = web_get_from_query_string(qs, (char*)"u");
    char *sid = web_get_from_query_string(qs, (char*)"s");

    // page was incorrectly accessed, redirect to login
    if (!user)
    {
        cout << "<meta http-equiv=\"refresh\" content=\"0; URL=/cgi-bin/login\" />";
        return 1;   // no session
    }

    // check database for session
    int res = check_session(user, atoi(sid), SESSION_LIFETIME);
    if (res == -1)  // misc error
    {
        cout << "<p>An error occurred, check the error log</p>";
        return 1;
    } else if (res == 0)
    {
        cout << "<meta http-equiv=\"refresh\" content=\"0; URL=/cgi-bin/login?u=" << user << "\" />";
        return 1;   // no session found
    } else if (res == 2)
    {
        cout << "<meta http-equiv=\"refresh\" content=\"0; URL=/cgi-bin/login?e=3&u=" << user << "\" />";
        return 1;   // session expired
    } else // if (res == 1)
    {
        cout << "<p>Logged in!</p>";
        return 0;   // session found
    }
}

int login()
{
    char **qs = web_get_post_string();

	char *user = web_get_from_query_string(qs, (char*)"user");
	char *password = web_get_from_query_string(qs, (char*)"password");

    int res;

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
            res = create_session(user, SESSION_LIFETIME);
            if (res)
                cout << "<meta http-equiv=\"refresh\" content=\"0; URL=/cgi-bin/main?u=" << user << "&s=" << res << "\" />";
            else
                cout << "An error occurred, check the error log";
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
		cout << "<p>No username given</p>";
	}

    return 0;
}