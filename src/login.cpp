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
        // char hashpass[129];

        cout << "<p>";

        res = check_pass(user, password);
        if (res == 0)
        {
            // invalid password
            cout << "Invalid password";
        } else if (res == 1)
        {
            // successful login
            cout << "Logged in";
        } else if (res == -1)
        {
            // Invalid user
            cout << "User \"" << user << "\" is not a valid user";
        } else
        {
            // other error
            cout << "An error occurred, check the error log";
        }

        // res = get_hashed_passphrase(user, hashpass);
        // if (res == 0)
        // {
        //     // User is valid, check password against database
            
        // }
        // else if (res == 2)
        //     cout << user << " is not a registered user";
        // else if (res == -1)
        //     cout << "Connection to database unsuccessful";
        // else if (res == -2)
        //     cout << "Config file could not be opened";

        cout << "</p>";
	} else
    {
        // html should prevent this
		cout << "<h1>No username given</h1>";
	}

    cout << "</body>";

    return 0;
}