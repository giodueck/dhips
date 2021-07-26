/*
    Useful functions for the whole program
*/
#ifndef DHIPS_H
#define DHIPS_H

#include <stdlib.h>

// Print error message to stderr in the format 'dhips: perror(msg)'
void dhips_perror(const char *msg);

// Print error message to stderr in the format 'dhips: msg'
void dhips_perror_no_errno(const char *msg);

// If a process by the name task_name is found, its pid is copied into *pid
void getPidByName(pid_t *pid, const char *task_name);

// If a process with the given pid is found, its name is copied into task_name
void getNameByPid(pid_t pid, char *task_name);

#endif // DHIPS_H