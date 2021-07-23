#include <iostream>
#include <string>
#include <cstring>
#include <unistd.h>
#include <stdlib.h>
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
//
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
    cout << "<form action=\"/cgi-bin/user_mgmt\" method=\"post\">";
    cout << "<label for=\"cpass\">Current password<br></label>";
    cout << "<input type=\"password\" name=\"cpass\" required><br>";
    cout << "<label for=\"npass\">New password<br></label>";
    cout << "<input type=\"password\" name=\"npass\" required><br>";
    cout << "<label for=\"mpass\">Confirm new password<br></label>";
    cout << "<input type=\"password\" name=\"mpass\" required><br><br>";

    cout << "<input type=\"hidden\" name=\"user\" value=\"" << user << "\">";
    cout << "<input type=\"hidden\" name=\"session\" value=" << session << ">";
    cout << "<input type=\"hidden\" name=\"own\" value=true>";

    cout << "<input type=\"submit\" name=\"action\" value=\"Update password\">";
    cout << "</form>";

    if (e)
    {
        if (e == 1)
        {
            cout << "<p style=\"color:Tomato;\">";
            cout << "Incorrect password";
            cout << "</p>";
        } else if (e == 2)
        {
            cout << "<p style=\"color:Tomato;\">";
            cout << "New password and confirmation are different";
            cout << "</p>";
        } else
        {
            cout << "<p>Password updated</p>";
        }
    }

    return 0;
}

int get_role(char *user)
{
    int r = pg_get_role(user);

    if (r < 0)
        return -1;
    else 
        return r;
}

int add_user(char *user, int session, int e)
{
    cout << "<form action=\"/cgi-bin/user_mgmt\" method=\"post\">";
    cout << "<label for=\"nuser\">Username<br></label>";
    cout << "<input type=\"text\" name=\"nuser\" required><br>";
    cout << "<label for=\"npass\">Password<br></label>";
    cout << "<input type=\"password\" name=\"npass\" required><br>";
    cout << "<label for=\"mpass\">Confirm password<br></label>";
    cout << "<input type=\"password\" name=\"mpass\" required><br>";
    cout << "<label for=\"admin\">Admin</label>";
    cout << "<input type=\"radio\" name=\"role\" value=\"admin\" required>";
    cout << "<label for=\"user\"><br>User</label>";
    cout << "<input type=\"radio\" name=\"role\" value=\"user\" required>";
    cout << "<label for=\"spectator\"><br>Spectator</label>";
    cout << "<input type=\"radio\" name=\"role\" value=\"spectator\" required>";

    cout << "<input type=\"hidden\" name=\"user\" value=\"" << user << "\">";
    cout << "<input type=\"hidden\" name=\"session\" value=" << session << ">";

    cout << "<br><br><input type=\"submit\" name=\"action\" value=\"Add user\">";
    cout << "</form>";

    if (e)
    {
        if (e == -1)
        {
            cout << "<p>User added</p>";
        } else if (e == -2)
        {
            cout << "<p style=\"color:Tomato;\">";
            cout << "An error occurred! Check the eror log";
            cout << "</p>";
        } else if (e == 1)
        {
            cout << "<p style=\"color:Tomato;\">";
            cout << "A user with that name already exists";
            cout << "</p>";
        } else if (e == 2)
        {
            cout << "<p style=\"color:Tomato;\">";
            cout << "New password and confirmation are different";
            cout << "</p>";
        }
    }

    return 0;
}

int edit_user(char *user, int session, int e)
{
    cout << "<form action=\"/cgi-bin/user_mgmt\" method=\"post\">";

    // select user
    // cout << "<label for=\"u\">Username<br></label>";
    // cout << "<input type=\"text\" name=\"u\" required><br><br>";

    char **users;
    int n_users = pg_get_users(&users);

    if (n_users < 0)
    {
        cout << "</form>";
        cout << "<p>An error occurred</p>";
        return -1;
    }
    
    cout << "<p>Select user:</p>";
    for (int i = 0; i < n_users; i++)
    {
        cout << "<input type=\"radio\" name=\"u\" value=\"" << users[i] << "\" required>";
        cout << "<label for=\"" << users[i] << "\">" << users[i] << "</label><br>";
        free(users[i]);
    }
    free(users);
    cout << "<br>";

    // hidden input
    cout << "<input type=\"hidden\" name=\"user\" value=\"" << user << "\">";
    cout << "<input type=\"hidden\" name=\"session\" value=" << session << ">";

    // edit password
    cout << "<label for=\"npass\">Password<br></label>";
    cout << "<input type=\"password\" name=\"npass\"><br>";
    cout << "<label for=\"mpass\">Confirm password<br></label>";
    cout << "<input type=\"password\" name=\"mpass\">";

    cout << "<br><input type=\"submit\" name=\"action\" value=\"Update password\"><br><br>";

    // edit role
    cout << "<label for=\"admin\">Admin</label>";
    cout << "<input type=\"radio\" name=\"role\" value=\"admin\">";
    cout << "<label for=\"user\"><br>User</label>";
    cout << "<input type=\"radio\" name=\"role\" value=\"user\">";
    cout << "<label for=\"spectator\"><br>Spectator</label>";
    cout << "<input type=\"radio\" name=\"role\" value=\"spectator\">";

    cout << "<br><input type=\"submit\" name=\"action\" value=\"Update role\">";

    // delete user
    cout << "<p style=\"color:Red;\">";
    cout << "<br>Danger zone";
    cout << "</p>";
    cout << "<input type=\"submit\" name=\"action\" value=\"Delete user\">";

    cout << "</form>";

    if (e)
    {
        if (e == 1)
        {
            cout << "<p>Password changed</p>";
        } else if (e == 2)
        {
            cout << "<p>Role changed</p>";
        } else if (e == 3)
        {
            cout << "<p>User deleted</p>";
        } else if (e == -1)
        {
            cout << "<p style=\"color:Tomato;\">";
            cout << "Password unchanged";
            cout << "</p>";
        } else if (e == -2)
        {
            cout << "<p style=\"color:Tomato;\">";
            cout << "Role unchanged";
            cout << "</p>";
        } else if (e == -3)
        {
            cout << "<p style=\"color:Tomato;\">";
            cout << "No users deleted";
            cout << "</p>";
        } else
        {
            cout << "<p style=\"color:Tomato;\">";
            cout << "An error occurred! Check the eror log";
            cout << "</p>";
        }
    }

    return 0;
}
