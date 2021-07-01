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

    web_print_header();
    cout << "<body>";

	if (user && user[0] != '\0')
    {
        char connstr[] = "dbname=dhips user=apache password=apache";
        cout << "<h1>";
        check_pwd(connstr, user, password);
        cout << "</h1>";
	} else
    {
		cout << "<h1>No username given</h1>";
	}

    cout << "</body>";

    return 0;
}