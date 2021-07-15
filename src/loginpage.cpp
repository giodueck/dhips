#include <iostream>
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

void print_login_form()
{
    cout << "<form action=\"/cgi-bin/main\" method=\"post\">\n";
    cout << "Username <input type=\"text\" name=\"user\" required><br>\n";
    cout << "Password <input type=\"password\" name=\"password\"><br>\n";
    cout << "<input type=\"submit\">\n";
    cout << "</form>\n";
}

int main()
{
    web_print_header();
    start_html();
    print_head();
    start_body();
    print_login_form();
    end_body();
    end_html();

    return 0;
}