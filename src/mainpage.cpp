#include <iostream>
#include <unistd.h>
#include <cstring>
#include "login.h"
#include "web.h"
#include "dhipsweb.h"
#include "pgsql.h"

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
        cout << "<br>";

    } else if (strcmp(section, "addu") == 0 && role == 0)
    {
        cout << "<h2>Add user</h2>";
        
        // get query variables
        user = web_get_from_query_string(qs, (char*)"u");
        char *session_s = web_get_from_query_string(qs, (char*)"s");
        if (session_s) session = atoi(session_s);
        char *e_s = web_get_from_query_string(qs, (char*)"e");
        if (e_s) e = atoi(e_s);

        // show add user form
        add_user(user, session, e);
        cout << "<br>";

    } else if (strcmp(section, "editu") == 0 && role == 0)
    {
        cout << "<h2>Edit user</h2>";
        
        // get query variables
        user = web_get_from_query_string(qs, (char*)"u");
        char *session_s = web_get_from_query_string(qs, (char*)"s");
        if (session_s) session = atoi(session_s);
        char *e_s = web_get_from_query_string(qs, (char*)"e");
        if (e_s) e = atoi(e_s);

        // show edit user form
        edit_user(user, session, e);

        cout << "<br>";

    } else if (strcmp(section, "alog") == 0 || strcmp(section, "plog") == 0)
    {
        const char *filename;

        if (section[0] == 'a')
        {
            cout << "<h2>Alarms</h2>";
            filename = "/var/www/log/alarmas.log";
        } else
        {
            cout << "<h2>Prevenciones</h2>";
            filename = "/var/www/log/prevencion.log";
        }
        
        int p = 1;
        int maxpage = pagecount(filename);

        // get query variables
        user = web_get_from_query_string(qs, (char*)"u");
        char *session_s = web_get_from_query_string(qs, (char*)"s");
        if (session_s) session = atoi(session_s);
        char *p_s = web_get_from_query_string(qs, (char*)"p");
        if (p_s) p = atoi(p_s);

        // page index error causes redirect to correct page
        if (p <= 0 and p != -1)
            cout << "<meta http-equiv=\"refresh\" content=\"0; URL=/cgi-bin/main?u=" << user << "&s=" << session << "&section=" << section << "&p=1\" />";
        else if (p > maxpage)
            cout << "<meta http-equiv=\"refresh\" content=\"0; URL=/cgi-bin/main?u=" << user << "&s=" << session << "&section=" << section << "&p=" << maxpage << "\" />";

        // show page buttons
        // first page
        cout << " Navigate: ";
        if (p > 1)
        {
            cout << "<a href=\"/cgi-bin/main?u=" << user << "&s=" << session << "&section=" << section << "&p=1\">";
            cout << "<<";
            cout << "</a> | ";
        } else cout << "<< | ";
        // previous page
        if (p > 1)
        {
            cout << "<a href=\"/cgi-bin/main?u=" << user << "&s=" << session << "&section=" << section << "&p=" << p - 1 << "\">";
            cout << "<";
            cout << "</a> | ";
        } else cout << "< | ";
        // current page
        cout << p << " | ";
        // next page
        if (p < maxpage)
        {
            cout << "<a href=\"/cgi-bin/main?u=" << user << "&s=" << session << "&section=" << section << "&p=" << p + 1 << "\">";
            cout << ">";
            cout << "</a> | ";
        } else cout << "> | ";
        // last page
        if (p < maxpage)
        {
            cout << "<a href=\"/cgi-bin/main?u=" << user << "&s=" << session << "&section=" << section << "&p=" << maxpage << "\">";
            cout << ">>";
            cout << "</a>";
        } else cout << ">>";

        if (section[0] == 'a')
        {
            p = show_alarm_table(p);
        } else
        {
            p = show_prevention_table(p);
        }

        cout << "<br>";

    } else if (strcmp(section, "conf") == 0 && role <= 1)
    {
        cout << "<h2>Settings</h2>";

        int p = 1, maxpage = 10;

        // get query variables
        user = web_get_from_query_string(qs, (char*)"u");
        char *session_s = web_get_from_query_string(qs, (char*)"s");
        if (session_s) session = atoi(session_s);
        char *p_s = web_get_from_query_string(qs, (char*)"p");
        if (!p_s)
        {
            // Config home, show links for each module
            cout << "<p>Select a module:</p>";

            cout << "<a href=\"/cgi-bin/main?u=" << user << "&s=" << session << "&section=conf&p=1\">";
            cout << "I";
            cout << "</a> | ";

            cout << "<a href=\"/cgi-bin/main?u=" << user << "&s=" << session << "&section=conf&p=2\">";
            cout << "II";
            cout << "</a> | ";

            cout << "<a href=\"/cgi-bin/main?u=" << user << "&s=" << session << "&section=conf&p=3\">";
            cout << "III";
            cout << "</a> | ";

            cout << "<a href=\"/cgi-bin/main?u=" << user << "&s=" << session << "&section=conf&p=4\">";
            cout << "IV";
            cout << "</a> | ";

            cout << "<a href=\"/cgi-bin/main?u=" << user << "&s=" << session << "&section=conf&p=5\">";
            cout << "V";
            cout << "</a> | ";

            cout << "<a href=\"/cgi-bin/main?u=" << user << "&s=" << session << "&section=conf&p=6\">";
            cout << "VI";
            cout << "</a> | ";

            cout << "<a href=\"/cgi-bin/main?u=" << user << "&s=" << session << "&section=conf&p=7\">";
            cout << "VII";
            cout << "</a> | ";

            cout << "<a href=\"/cgi-bin/main?u=" << user << "&s=" << session << "&section=conf&p=8\">";
            cout << "VIII";
            cout << "</a> | ";

            cout << "<a href=\"/cgi-bin/main?u=" << user << "&s=" << session << "&section=conf&p=9\">";
            cout << "IX";
            cout << "</a> | ";

            cout << "<a href=\"/cgi-bin/main?u=" << user << "&s=" << session << "&section=conf&p=10\">";
            cout << "X";
            cout << "</a><br><br>";
        } else
        {
            p = atoi(p_s);

            // page index error causes redirect to correct page
            if (p <= 0 and p != -1)
                cout << "<meta http-equiv=\"refresh\" content=\"0; URL=/cgi-bin/main?u=" << user << "&s=" << session << "&section=" << section << "&p=1\" />";
            else if (p > maxpage)
                cout << "<meta http-equiv=\"refresh\" content=\"0; URL=/cgi-bin/main?u=" << user << "&s=" << session << "&section=" << section << "&p=" << maxpage << "\" />";

            // show page buttons
            // first page
            cout << " Navigate: ";
            if (p > 1)
            {
                cout << "<a href=\"/cgi-bin/main?u=" << user << "&s=" << session << "&section=" << section << "&p=1\">";
                cout << "<<";
                cout << "</a> | ";
            } else cout << "<< | ";
            // previous page
            if (p > 1)
            {
                cout << "<a href=\"/cgi-bin/main?u=" << user << "&s=" << session << "&section=" << section << "&p=" << p - 1 << "\">";
                cout << "<";
                cout << "</a> | ";
            } else cout << "< | ";
            // current page
            printr(p);
            cout << " | ";
            // next page
            if (p < maxpage)
            {
                cout << "<a href=\"/cgi-bin/main?u=" << user << "&s=" << session << "&section=" << section << "&p=" << p + 1 << "\">";
                cout << ">";
                cout << "</a> | ";
            } else cout << "> | ";
            // last page
            if (p < maxpage)
            {
                cout << "<a href=\"/cgi-bin/main?u=" << user << "&s=" << session << "&section=" << section << "&p=" << maxpage << "\">";
                cout << ">>";
                cout << "</a>";
            } else cout << ">>";
            
            cout << "<h3>Modulo ";
            printr(p);
            cout << "</h3>";

            show_config(p, user, session);

            cout << "<a href=\"/cgi-bin/main?u=" << user << "&s=" << session << "&section=conf\">";
            cout << "Settings";
            cout << "</a> | ";
        }
    }

    // Back to home section
    cout << "<a href=\"/cgi-bin/main?u=" << user << "&s=" << session << "\">Home</a>";

    cout << "</body>";
}

void show_links()
{
    // show only if role permits it
    
    user = web_get_from_query_string(qs, (char*)"u");

    // logs, role: spectator+
    cout << "<a href=\"/cgi-bin/main?u=" << user << "&s=" << session << "&section=alog\">";
    cout << "Alarms";
    cout << "</a> | ";

    cout << "<a href=\"/cgi-bin/main?u=" << user << "&s=" << session << "&section=plog\">";
    cout << "Preventions";
    cout << "</a><br>";

    // config, role: user+
    if (role <= 1)
    {
        cout << "<a href=\"/cgi-bin/main?u=" << user << "&s=" << session << "&section=conf\">";
        cout << "Settings";
        cout << "</a><br>";
    }
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

            cout << "<br>";

            // Account
            show_account_mgmt_links();

            cout << "</body>";
        }
    }
    return 0;
}