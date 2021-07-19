/*
    Useful functions for the whole program
*/
#ifndef DHIPS_H
#define DHIPS_H

// Print error message to stderr in the format 'dhips: perror(msg)'
void dhips_perror(const char *msg);

// Print error message to stderr in the format 'dhips: msg'
void dhips_perror_no_errno(const char *msg);

#endif // DHIPS_H