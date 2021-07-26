#include "dhipsweb.h"
#include "dhipslib.h"
#include "pgsql.h"
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <vector>
#include <alarms.h>

using namespace std;

// Prints the table style tag and its contents
void print_table_style()
{
    cout << "<style>";
    cout << "table, th, td {";
    cout << "border: 1px solid black;";
    cout << "border-collapse: collapse;";
    cout << "}";
    cout << "th, td {";
    cout << "padding: 5px;";
    cout << "}";
    cout << "</style>";
}

int pagecount(const char *filename)
{
    FILE *fd = fopen(filename, "r");
    if (!fd)
    {
        char msg[128];
        fprintf(stderr, "pagecount: could not open file %s", filename);
        return -1;
    }

    int i = 0;
    char buf[BUFSIZ];

    while (fgets(buf, BUFSIZ, fd))
    {
        // ignore comments and empty lines
        if (strlen(buf) == 0)
            continue;
        int j = 0;
        while (j < strlen(buf))
        {
            if (buf[j] == ' ' || buf[j] == '\t')
                j++;
            else break;
        }
        if (buf[j] == '#' || buf[j] == '\n')
            continue;

        // i is used as a counter
        i++;
    }

    fclose(fd);
    if (i)
        return i / PAGE_SIZE + (i % PAGE_SIZE > 0);
    else return 1;
}

int parse_and_print_line_a(const char *filename, int page)
{
    int pc = pagecount(filename);
    FILE *fd = fopen(filename, "r");
    if (!fd)
    {
        char msg[128];
        sprintf(msg, "parse_and_print_line_a: could not open file %s", filename);
        dhips_perror(msg);
        return -1;
    } else if (pc == -1)
        return -1;

    int i = 0;
    char buf[BUFSIZ];
    char *tok;

    // page index error handling
    if (page <= 0) page = 1;
    else if (page > pc) page = pc;

    cout << "<table>";
    cout << "  <tr>";
    cout << "    <th>Timestamp</th>";
    cout << "    <th>Description</th>";
    cout << "    <th>IP</th>";
    cout << "    <th>Notes</th>";
    cout << "  </tr>";

    while (fgets(buf, BUFSIZ, fd))
    {
        // ignore comments and empty lines
        if (strlen(buf) == 0)
            continue;
        int j = 0;
        while (j < strlen(buf))
        {
            if (buf[j] == ' ' || buf[j] == '\t')
                j++;
            else break;
        }
        if (buf[j] == '#' || buf[j] == '\n')
            continue;
        
        // parse and show alarm if on the right page
        if (i / PAGE_SIZE == page - 1)
        {
            cout << "  <tr>";

            // timestamp
            tok = strtok(buf, "\t");
            cout << "    <td>" << tok << "</td>";
            // description
            tok = strtok(NULL, "\t");
            tok += 3;
            cout << "    <td>" << tok << "</td>";
            // IP
            tok = strtok(NULL, "\t");
            tok += 3;
            cout << "    <td>" << tok << "</td>";
            // aditional
            tok = strtok(NULL, "\t");
            if (tok[0] != '\n')
            {
                tok += 3;
                cout << "    <td>" << tok << "</td>";
            }

            cout << "  </tr>";
        }

        // i is used as an alarm counter
        i++;
    }
    
    fclose(fd);
    cout << "</table>";

    return page;
}

int parse_and_print_line_p(const char *filename, int page)
{
    int pc = pagecount(filename);
    FILE *fd = fopen(filename, "r");
    if (!fd)
    {
        char msg[128];
        fprintf(stderr, "parse_and_print_line_p: could not open file %s", filename);
        return -1;
    } else if (pc == -1)
        return -1;

    int i = 0;
    char buf[BUFSIZ];
    char *tok;

    // page index error handling
    if (page <= 0) page = 1;
    else if (page > pc) page = pc;

    cout << "<table>";
    cout << "  <tr>";
    cout << "    <th>Timestamp</th>";
    cout << "    <th>Description</th>";
    cout << "  </tr>";

    while (fgets(buf, BUFSIZ, fd))
    {
        // ignore comments and empty lines
        if (strlen(buf) == 0)
            continue;
        int j = 0;
        while (j < strlen(buf))
        {
            if (buf[j] == ' ' || buf[j] == '\t')
                j++;
            else break;
        }
        if (buf[j] == '#' || buf[j] == '\n')
            continue;
        
        // parse and show alarm if on the right page
        if (i / PAGE_SIZE == page - 1)
        {
            cout << "  <tr>";

            // timestamp
            tok = strtok(buf, "\t");
            cout << "    <td>" << tok << "</td>";
            // description
            tok = strtok(NULL, "\t");
            tok += 3;
            cout << "    <td>" << tok << "</td>";

            cout << "  </tr>";
        }

        // i is used as an alarm counter
        i++;
    }
    
    fclose(fd);
    cout << "</table>";

    return page;
}

int parse_and_print_line_a(int page)
{
    return parse_and_print_line_a("/var/www/log/alarmas.log", page);
}

int parse_and_print_line_p(int page)
{
    return parse_and_print_line_p("/var/www/log/prevencion.log", page);
}

int show_alarm_table(int page)
{
    print_table_style();
    return parse_and_print_line_a(page);
}

int show_prevention_table(int page)
{
    print_table_style();
    return parse_and_print_line_p(page);
}

// Prints n in roman numeral notation
void printr(int n)
{
    // numbers are 1-10, so just do the easy way
    switch (n)
    {
        case 3:
            cout << "I";
        case 2:
            cout << "I";
        case 1:
            cout << "I";
            break;
        case 4:
            cout << "IV";
            break;
        case 5:
            cout << "V";
            break;
        case 6:
            cout << "VI";
            break;
        case 7:
            cout << "VII";
            break;
        case 8:
            cout << "VIII";
            break;
        case 9:
            cout << "IX";
            break;
        case 10:
            cout << "X";
            break;
        
        default:
            break;
    }
}

int show_config_i(char *user, int session)
{
    print_table_style();

    // show form with data from 
    cout << "<form action=\"/cgi-bin/config\" method=\"post\">";

    // toggle module
    cout << "<input type=\"submit\" name=\"apply\" value=\"Apply configuration\"><br><br>";

    cout << "<input type=\"checkbox\" name=\"enabled\" " << ((pg_module_enabled(1) == 1) ? "checked" : "") << ">";
    cout << "<label for=\"enabled\"> Enabled</label><br>";

    // Toggle all system files
    cout << "<input type=\"checkbox\" name=\"enablesys\" checked>";
    cout << "<label for=\"enablesys\"> Monitor system files</label><br>";

    // Toggle all binary files
    cout << "<input type=\"checkbox\" name=\"enablebin\" checked>";
    cout << "<label for=\"enablebin\"> Monitor binary files</label><br>";

    // hidden info
    cout << "<input type=\"hidden\" name=\"user\" value=\"" << user << "\">";
    cout << "<input type=\"hidden\" name=\"session\" value=" << session << ">";
    cout << "<input type=\"hidden\" name=\"mod\" value=1>";

    cout << "</form>";

    return 0;
}

int show_config_ii(char *user, int session)
{
    print_table_style();

    // show form with data from 
    cout << "<form action=\"/cgi-bin/config\" method=\"post\">";

    // toggle module
    cout << "<input type=\"submit\" name=\"apply\" value=\"Apply configuration\"><br><br>";

    cout << "<input type=\"checkbox\" name=\"enabled\" " << ((pg_module_enabled(2) == 1) ? "checked" : "") << ">";
    cout << "<label for=\"enabled\"> Enabled</label><br>";

    // hidden info
    cout << "<input type=\"hidden\" name=\"user\" value=\"" << user << "\">";
    cout << "<input type=\"hidden\" name=\"session\" value=" << session << ">";
    cout << "<input type=\"hidden\" name=\"mod\" value=2>";

    cout << "</form>";

    return 0;
}

int show_config_iii(char *user, int session)
{
    print_table_style();

    // show form with data from 
    cout << "<form action=\"/cgi-bin/config\" method=\"post\">";

    // toggle module
    cout << "<input type=\"submit\" name=\"apply\" value=\"Apply configuration\"><br><br>";

    cout << "<input type=\"checkbox\" name=\"enabled\" " << ((pg_module_enabled(3) == 1) ? "checked" : "") << ">";
    cout << "<label for=\"enabled\"> Enabled</label><br>";

    // hidden info
    cout << "<input type=\"hidden\" name=\"user\" value=\"" << user << "\">";
    cout << "<input type=\"hidden\" name=\"session\" value=" << session << ">";
    cout << "<input type=\"hidden\" name=\"mod\" value=3>";

    cout << "</form>";

    return 0;
}

int show_config(int module, char *user, int session)
{
    switch (module)
    {
        case 1:
            show_config_i(user, session);
            break;
        case 2:
            show_config_ii(user, session);
            break;
        case 3:
            show_config_iii(user, session);
            break;
        case 4:
            break;
        case 5:
            break;
        case 6:
            break;
        case 7:
            break;
        case 8:
            break;
        case 9:
            break;
        case 10:
            break;
        
        default:
            cout << "<p style=\"color:Tomato\">Invalid module</p>";
    }

    return 0;
}
