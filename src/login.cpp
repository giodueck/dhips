#include <iostream>
#include <string>
#include <cstring>
#include <unistd.h>
#include "login.h"
#include "web.h"
#include "pgsql.h"

using namespace std;

char **verify_session(int *session, char *user)
{
    // get username from query string
    char **qs = web_get_query_string();
    user = web_get_from_query_string(qs, (char*)"u");
    char *sid = web_get_from_query_string(qs, (char*)"s");

    // page was incorrectly accessed, redirect to login
    if (!user)
    {
        // no session
        cout << "<meta http-equiv=\"refresh\" content=\"0; URL=/cgi-bin/login\" />";
    }

    // check database for session
    int res = pg_check_session(user, *session = atoi(sid), SESSION_LIFETIME);
    if (res == -1)  // misc error
    {
        cout << "<p>An error occurred, check the error log</p>";
    } else if (res == 0)
    {
        // no session found
        cout << "<meta http-equiv=\"refresh\" content=\"0; URL=/cgi-bin/login?u=" << user << "\" />";
    } else if (res == 2)
    {
        // session expired
        cout << "<meta http-equiv=\"refresh\" content=\"0; URL=/cgi-bin/login?e=3&u=" << user << "\" />";
    } else if (res == 3)
    {
        // session ended
        cout << "<meta http-equiv=\"refresh\" content=\"0; URL=/cgi-bin/login?e=4&u=" << user << "\" />";
    }
    return qs;
}

// char **verify_session(char **qs, int session, char *user)
// {
//     // page was incorrectly accessed, redirect to login
//     if (!user)
//     {
//         // no session
//         cout << "<meta http-equiv=\"refresh\" content=\"0; URL=/cgi-bin/login\" />";
//     }

//     // check database for session
//     int res = pg_check_session(user, session, SESSION_LIFETIME);
//     if (res == -1)  // misc error
//     {
//         cout << "<p>An error occurred, check the error log</p>";
//     } else if (res == 0)
//     {
//         // no session found
//         cout << "<meta http-equiv=\"refresh\" content=\"0; URL=/cgi-bin/login?u=" << user << "\" />";
//     } else if (res == 2)
//     {
//         // session expired
//         cout << "<meta http-equiv=\"refresh\" content=\"0; URL=/cgi-bin/login?e=3&u=" << user << "\" />";
//     } else if (res == 3)
//     {
//         // session ended
//         cout << "<meta http-equiv=\"refresh\" content=\"0; URL=/cgi-bin/login?e=4&u=" << user << "\" />";
//     }
//     return qs;
// }

int login()
{
    char **qs = web_get_post_string();

	char *user = web_get_from_query_string(qs, (char*)"user");
	char *password = web_get_from_query_string(qs, (char*)"password");

    int res;

	if (user && user[0] != '\0')
    {
        cout << "<p>";

        res = pg_check_pass(user, password);
        if (res == 0)
        {
            // invalid password
            cout << "<meta http-equiv=\"refresh\" content=\"0; URL=/cgi-bin/login?e=1&u=" << user << "\" />";
        } else if (res == 1)
        {
            // successful login
            res = pg_create_session(user, SESSION_LIFETIME);
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

int change_passwd(char *user, int session, int e)
{
    if (!user) // whose?
        return -1;

    cout << "<form action=\"/cgi-bin/change_passwd\" method=\"post\">";
    cout << "<label for=\"cpass\">Current password<br></label>";
    cout << "<input type=\"password\" name=\"cpass\" required><br>";
    cout << "<label for=\"npass\">New password<br></label>";
    cout << "<input type=\"password\" name=\"npass\" required><br><br>";

    cout << "<input type=\"hidden\" name=\"user\" value=\"" << user << "\">";
    cout << "<input type=\"hidden\" name=\"session\" value=" << session << ">";

    cout << "<input type=\"submit\" value=\"Update password\">";
    cout << "</form>";

    if (e)
    {
        if (e > 0)
        {
            cout << "<p style=\"color:Tomato;\">";
            cout << "Incorrect password";
            cout << "</p>";
        } else
        {
            cout << "<p>Password updated!</p>";
        }
    }

    return 0;
}
