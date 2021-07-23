#ifndef DHIPS_WEB_H
#define DHIPS_WEB_H

#define PAGE_SIZE 15

// prints the html for showing all alarms arranged in tables and divided into PAGE_SIZE sized chunks
int show_alarm_table(int page);

// Returns the amount of pages filename can be divided up into
int pagecount(const char *filename);

#endif // DHIPS_WEB_H