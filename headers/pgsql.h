#ifndef PGSQL_H
#define PGSQL_H

#include <libpq-fe.h>

#define SBUFSIZ 1024

// Copies the hashed passphrase stored for username into dest
//  Returns 0 if successful, other if not
// If the resulting dest string is NULL, returns 2
int pg_get_hashed_passphrase(char *username, char *dest);

// Checks password against the stored password for user by calling pg_get_hashed_passphrase
//  Returns 1 if correct, 0 if not. If user does not exist, returns 2. If another error
// ocurrs, writes to stderr and returns -2. In any case, errors will be reported by 
// pg_get_hashed_passphrase to stderr
int pg_check_pass(char *user, char *password);

// Checks if a session for user exists and is valid. If one is found, the expiration
// timestamp is updated to lifetime minutes + current time and the return value is 1.
// If no active session was found returns 0, if an expired session was found returns 2
int pg_check_session(char *username, int session, int lifetime);

// Creates a new session for user with the given lifetime in minutes
//  Returns 0 if successful, other if not
int pg_create_session(char *username, int lifetime);

// Ends session
//  Returns 0 if successful, other if not
int pg_terminate_session(int session);

// Checks the ruleset for the severity of the given alarm.
// severity can be of any numeric value in enum AlarmType
//  Returns severity if successful, negative if not
int pg_get_alarm_severity(int alarm_id);

// Checks the ruleset for the description of the given alarm.
//  Returns description if successful, NULL if not
char *pg_get_alarm_description(int alarm_id);

#endif // PGSQL_H