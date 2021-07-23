#ifndef DHIPS_WEB_H
#define DHIPS_WEB_H

#define PAGE_SIZE 15

// prints the html for showing all alarms arranged in tables and divided into PAGE_SIZE sized chunks
int show_alarm_table(int page);

// prints the html for showing all preventions arranged in tables and divided into PAGE_SIZE sized chunks
int show_prevention_table(int page);

// Returns the amount of pages filename can be divided up into
int pagecount(const char *filename);

// prints the config form for the module of the given number 
int show_config(int module);

// Prints n in roman numeral notation
void printr(int n);

#endif // DHIPS_WEB_H