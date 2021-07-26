/*
    Useful functions for the whole program
*/
#ifndef DHIPS_H
#define DHIPS_H

#include <stdlib.h>
#include "defines.h"
#include "pgsql.h"

// Print error message to stderr in the format 'dhips: perror(msg)'
void dhips_perror(const char *msg);

// Print error message to stderr in the format 'dhips: msg'
void dhips_perror_no_errno(const char *msg);

// If a process by the name task_name is found, its pid is copied into *pid
void getPidByName(pid_t *pid, const char *task_name);

// If a process with the given pid is found, its name is copied into task_name
void getNameByPid(pid_t pid, char *task_name);

// Send an email to all the administrators with an email address
void dhips_send_email(const char *subject, const char *msg);

#endif // DHIPS_H