#include <iostream>
#include <crypt.h>
#include <shadow.h>
#include <string>
#include "web.h"
#include "login.h"

using namespace std;

int login()
{
    char **qs = web_get_post_string();

	char *user = web_get_from_query_string(qs, (char*)"user");
	char *password = web_get_from_query_string(qs, (char*)"password");

	web_print_header();
	printf("<body>");

	if (user && user[0] != '\0')
    {
        FILE *output = popen(string("/usr/bin/id -u " + string(user)).c_str(), "r");
        char buffer[1024];
        buffer[0] = '\0';
        fgets(buffer, sizeof(buffer), output);

        printf("<h1>");
        if (buffer[0] == '\0')
        {
            printf("%c", buffer[0]);
            printf("No such user");
        } else
        {
            printf("Welcome: %s", user);
        }
        printf("</h1>");

        pclose(output);
	} else
    {
		printf("<h1>No username given</h1>");
	}

	printf("</body>");

    return 0;
}