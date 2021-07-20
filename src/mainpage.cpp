#include <iostream>
#include <unistd.h>
#include <cstring>
#include "login.h"
#include "web.h"

using namespace std;

char **qs;
int session, e = 0, role;
char *user, *section;

bool is_post()
{
    char *cont_len = getenv("CONTENT_LENGTH");
    if (!cont_len)
    {
        return false;
    } else return true;
}

void logout_link()
{
    cout << "<a href=\"/cgi-bin/logout?s=" << session << "\">Log out</a>";
}

void change_passwd_link()
{
    user = web_get_from_query_string(qs, (char*)"u");
    cout << "<a href=\"/cgi-bin/main?u=" << user << "&s=" << session << "&section=pass\">Change password</a>";
}

void add_user_link()
{
    user = web_get_from_query_string(qs, (char*)"u");
    cout << "<a href=\"/cgi-bin/main?u=" << user << "&s=" << session << "&section=addu\">Add user</a>";
}

void edit_user_link()
{
    user = web_get_from_query_string(qs, (char*)"u");
    cout << "<a href=\"/cgi-bin/main?u=" << user << "&s=" << session << "&section=editu\">Edit user</a>";
}

void show_account_mgmt_links()
{
    logout_link();
    cout << " | ";
    change_passwd_link();

    // admin
    if (role == 0)
    {
        cout << "<br>";
        add_user_link();
        cout << " | ";
        edit_user_link();
    }
}

void show_section()
{
    if (strcmp(section, "pass") == 0)
    {
        cout << "<h2>Change password</h2>";
        
        // get query variables
        user = web_get_from_query_string(qs, (char*)"u");
        char *session_s = web_get_from_query_string(qs, (char*)"s");
        if (session_s) session = atoi(session_s);
        char *e_s = web_get_from_query_string(qs, (char*)"e");
        if (e_s) e = atoi(e_s);

        // show change password form
        change_passwd(user, session, e);

    } else if (strcmp(section, "addu") == 0 && role == 0)
    {
        cout << "<h2>Add user</h2>";
        
        // get query variables
        user = web_get_from_query_string(qs, (char*)"u");
        char *session_s = web_get_from_query_string(qs, (char*)"s");
        if (session_s) session = atoi(session_s);
        char *e_s = web_get_from_query_string(qs, (char*)"e");
        if (e_s) e = atoi(e_s);

        // show change password form
        add_user(user, session, e);
    } else if (strcmp(section, "editu") == 0 && role == 0)
    {
        cout << "<h2>Edit user</h2>";
        
        // get query variables
        user = web_get_from_query_string(qs, (char*)"u");
        char *session_s = web_get_from_query_string(qs, (char*)"s");
        if (session_s) session = atoi(session_s);
        char *e_s = web_get_from_query_string(qs, (char*)"e");
        if (e_s) e = atoi(e_s);

        // show change password form
        edit_user(user, session, e);
    }

    // Back to home section
    cout << "<br><a href=\"/cgi-bin/main?u=" << user << "&s=" << session << "\">Home</a>";

    cout << "</body>";
}

void show_links()
{
    // show if role permits it
}

int main()
{
    // The main page should not be accessible without being logged in as a registered account
    // if post, call login
    // if get and no user given, redirect to login
    // if get and user given, check sessions
    //  if session active, renew and continue
    //  if no session or not active, redirect to login

    web_print_header();

    if (is_post())
    {
        login();
    } else
    {
        cout << "<head><title>DHIPS | Main Page</title></head>\n";
        cout << "<body>\n";
        qs = verify_session(&session, user);

        section = web_get_from_query_string(qs, (char*)"section");
        user = web_get_from_query_string(qs, (char*)"u");
        role = get_role(user);
        if (section)
        {
            // show elements for the current section
            // will add and close <head>, <title> and <body> tags
            show_section();
        } else  // home section
        {
            cout << "<h1>DHIPS Home<br></h1>";

            // show all the links for the different sections of the main page
            show_links();

            // Account
            show_account_mgmt_links();

            cout << "</body>";
        }
    }
    return 0;
}