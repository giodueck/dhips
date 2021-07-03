#include <iostream>
#include <crypt.h>
#include <shadow.h>
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
        string connstr = "dbname=dhips user=apache password=apache";
        char hashpass[129];

        cout << "<p>";

        res = get_hashed_passphrase(connstr.c_str(), user, hashpass);
        if (res == 0)
            cout << hashpass;
        else if (res == 2)
            cout << user << " is not a registered user";

        cout << "</p>";
	} else
    {
        // html should prevent this
		cout << "<h1>No username given</h1>";
	}

    cout << "</body>";

    return 0;
}