#include <iostream>
#include <string.h>
#include "login.h"
#include "web.h"

using namespace std;

#define start_html() printf("<!doctype html>\n<html>")
#define end_html() printf("</html>")
#define start_body() printf("<body>\n")
#define end_body() printf("</body>\n")


void print_head()
{
    cout << "<head>";
    cout <<     "<title>DHIPS Login</title>";
    cout << "</head>";
}

void print_login_form(char *user)
{
    cout << "<form action=\"/cgi-bin/main\" method=\"post\">\n";
    if (user)
        cout << "Username<br><input type=\"text\" name=\"user\" value=\"" << user << "\" required><br>\n";
    else
        cout << "Username<br><input type=\"text\" name=\"user\" required><br>\n";
    cout << "Password<br><input type=\"password\" name=\"password\"><br><br>\n";
    cout << "<input type=\"submit\" value=\"Log in\">\n";
    cout << "</form>\n";
}

void print_error(char *e)
{
    if (strcmp(e, "1") == 0)
    {
        cout << "Incorrect password";
    } else if (strcmp(e, "2") == 0)
    {
        cout << "User does not exist";
    }
}

int main()
{
    web_print_header();
    
    // Check variables to print errors
    char **qs = web_get_query_string();
    char *e = web_get_from_query_string(qs, (char*)"e");
    char *u = web_get_from_query_string(qs, (char*)"u");

    start_html();
    print_head();
    start_body();
    print_login_form(u);

    // Print error if there is one
    if (e)
    {
        cout << "<p style=\"color:Tomato;\">";
        print_error(e);
        cout << "</p>";
    }

    end_body();
    end_html();

    return 0;
}