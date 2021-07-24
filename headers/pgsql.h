#ifndef PGSQL_H
#define PGSQL_H

#include <libpq-fe.h>

#define SBUFSIZ 1024

// Verifies that the database and all the necessary objects exist
int pg_check_db();

// Copies the hashed passphrase stored for username into dest
//  Returns 0 if successful, other if not
// If the resulting dest string is NULL, returns 2
int pg_get_hashed_passphrase(char *username, char *dest);

// Checks password against the stored password for user by calling pg_get_hashed_passphrase
//  Returns 1 if correct, 0 if not. If user does not exist, returns 2. If another error
// ocurrs, writes to stderr and returns -2. In any case, errors will be reported by 
// pg_get_hashed_passphrase to stderr
int pg_check_pass(char *user, char *password);

// Updates the stored hashed passfrase pwd for the given user
//  Returns 0 if successful, other if not
int pg_change_pass(char *user, char *hash);

// Updates the role for the given user
//  Returns 0 if successful, other if not
int pg_change_role(char *user, char *role);

// Adds a user.
//  Returns 0 if successful, other if not
int pg_add_user(char *username, char *hash, char *role);

// Removes a user.
//  Returns 0 if successful, other if not
int pg_delete_user(char *username);

// Returns 0 if user is 'admin', 1 if user is 'user', 2 if user is 'spectator', 3 if none, and other if error
int pg_get_role(char *user);

// Retrieves all usernames from the database into dest. Each item should be freed after using
//  Returns the number of users found, negative if error
int pg_get_users(char ***dest);

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

// Copies the first active filename with monitor_id > index and the given type into dest.
// dest will be malloced, free after done.
//  Returns the monitor_id of the monitor item if found, 0 if none found, and negative if errors occur
int pg_get_monitor_filename(int index, int type, char **dest);

// Copies the first filename with monitor_id > index and the given type into dest regardless of active.
// Copies active into int *active, as true = 1 and false = 0. dest will be malloced, free after done.
//  Returns the monitor_id of the monitor item if found, 0 if none found, and negative if errors occur
int pg_get_monitor_filename_conf(int index, int type, char **dest, int *active);

// Copies the first active targeted_proc name with targeted_proc_id > index and the given type into dest.
// dest will be malloced, free after done.
//  Returns the targeted_proc_id of the targeted_proc item if found, 0 if none found, and negative if errors occur
int pg_get_targeted_proc_name(int index, int type, char **dest);

//  Returns 1 if enabled, 0 if not, 2 if not found, negatives if error
int pg_module_enabled(int module);

//  Returns 0 if enabled, 1 if not found, negatives if error
int pg_module_toggle(int module, int status);

#endif // PGSQL_H