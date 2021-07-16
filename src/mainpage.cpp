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
    // if post, call login
    // if get and no user given, redirect to login
    // if get and user given, check sessions
    //  if session active, renew and continue
    //  if no session or not active, redirect to login

    web_print_header();
    cout << "<head><title>DHIPS Config</title></head>\n";
    cout << "<body>\n";

    if (is_post())
    {
        login();
    } else
    {
        verify_session();
    }

    cout << "</body>";
    return 0;
}