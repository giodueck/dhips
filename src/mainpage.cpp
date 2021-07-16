#include <iostream>
#include <unistd.h>
#include "login.h"
#include "web.h"

using namespace std;

bool is_post()
{
    char *cont_len = getenv("CONTENT_LENGTH");
    if (!cont_len)
    {
        return false;
    } else return true;
}

int main()
{
    // The main page should not be accessible without being logged in as a registered account
    // if post, call login
    // if get and no user given, redirect to login
    // if get and user given, check sessions
    //  if session active, renew and continue
    //  if no session or not active, redirect to login

    int session;
    char *user;

    web_print_header();
    cout << "<head><title>DHIPS Config</title></head>\n";
    cout << "<body>\n";

    if (is_post())
    {
        login();
    } else
    {
        verify_session(&session, user);
        cout << "<p>Logged in!</p>";
    }

    // Log out to end session manually
    cout << "<br><br><a href=\"/cgi-bin/logout?s=" << session << "\">Log out</a>";

    cout << "</body>";
    return 0;
}