#include "dhipsweb.h"
#include "dhipslib.h"
#include <iostream>
#include <stdio.h>
#include <string.h>

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
        sprintf(msg, "pagecount: could not open file %s", filename);
        dhips_perror(msg);
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
    return i / PAGE_SIZE + (i % PAGE_SIZE > 0);
}

int parse_and_print_line(const char *filename, int page)
{
    int pc = pagecount(filename);
    FILE *fd = fopen(filename, "r");
    if (!fd)
    {
        char msg[128];
        sprintf(msg, "could not open file %s", filename);
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

    cout << "<table>\n";
    cout << "  <tr>\n";
    cout << "    <th>Timestamp</th>\n";
    cout << "    <th>Tipo de alarma</th>\n";
    cout << "    <th>IP</th>\n";
    cout << "    <th>Observaciones</th>\n";
    cout << "  </tr>\n";

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
            cout << "  <tr>\n";

            // timestamp
            tok = strtok(buf, "\t");
            cout << "    <td>" << tok << "</td>\n";
            // description
            tok = strtok(NULL, "\t");
            tok += 3;
            cout << "    <td>" << tok << "</td>\n";
            // IP
            tok = strtok(NULL, "\t");
            tok += 3;
            cout << "    <td>" << tok << "</td>\n";
            // aditional
            tok = strtok(NULL, "\t");
            if (tok[0] != '\n')
            {
                tok += 3;
                cout << "    <td>" << tok << "</td>\n";
            }

            cout << "  </tr>\n";
        }

        // i is used as an alarm counter
        i++;
    }
    
    fclose(fd);
    cout << "</table>\n";

    return page;
}

int parse_and_print_line_a(int page)
{
    return parse_and_print_line("/var/log/hips/alarmas.log", page);
}

int parse_and_print_line_p(int page)
{
    return parse_and_print_line("/var/log/hips/prevencion.log", page);
}

int show_alarm_table(int page)
{
    print_table_style();
    return parse_and_print_line_a(page);
}