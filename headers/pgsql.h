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

// Retrieves all usernames from the database into dest.
//  Returns the number of users found, negative if error
int pg_get_users(char ***dest);

// Retrieves all usernames from the database into dest and the given role.
//  Returns the number of users found, negative if error
int pg_get_users_with_role(char ***dest, int role);

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
//  Returns the monitor_id of the monitor item if found, 0 if none found, and negative if errors occur
int pg_get_monitor_filename(int index, int type, char **dest);

// Copies the first filename with monitor_id > index and the given type into dest regardless of active.
// Copies active into int *active, as true = 1 and false = 0.
//  Returns the monitor_id of the monitor item if found, 0 if none found, and negative if errors occur
int pg_get_monitor_filename_conf(int index, int type, char **dest, int *active);

// Copies the first active targeted_proc name with targeted_proc_id > index and the given type into dest.
//  Returns the targeted_proc_id of the targeted_proc item if found, 0 if none found, and negative if errors occur
int pg_get_targeted_proc_name(int index, int type, char **dest);

//  Returns 1 if enabled, 0 if not, 2 if not found, negatives if error
int pg_module_enabled(int module);

//  Returns 0 if enabled, 1 if not found, negatives if error
int pg_module_toggle(int module, int status);

// Sets the active status of the given filename and type
//  Returns 0 if successful, 1 if not found, and negative if errors occur
int pg_set_monitor_filename_conf(int index, int type, const char *fullfilename, int active);

// Sets the active status of the given filename and type
//  Returns 0 if successful, 1 if not found, and negative if errors occur
int pg_toggle_monitor_filetype(int type, int active);

// Sets the config_changed to active
//  Returns 0 if successful, negative if errors occur
int pg_set_config_changed(int changed);

// Gets config_changed
//  Returns 1 if true, 0 if false, negative if errors occur
int pg_get_config_changed();

// Copies the first active targeted_ext name with targeted_ext_id > index into dest.
//  Returns the targeted_ext_id of the targeted_ext item if found, 0 if none found, and negative if errors occur
int pg_get_targeted_ext_name(int index, char **dest);

// Copies the email of user into dest
//  Returns 0 if successful, 1 if not found, negatives if errors occur
int pg_get_email(char *user, char **dest);

// Adds filename to the monitor table with the given type. If the file is already in the list, it will
// be deleted and readded
//  Returns 0 if successful, negatives if errors occur
int pg_add_monitor_filename(int type, char *filename);

// Removes filename from the monitor table. Type is irrelevant
//  Returns 0 if successful, 1 if not found, negatives if errors occur
int pg_rm_monitor_filename(char *filename);

// Adds procname to the targeted_proc table with the given type. If the file is already in the list, it will
// be deleted and readded
//  Returns 0 if successful, negatives if errors occur
int pg_add_targeted_proc(int type, char *procname);

// Removes procname from the targeted_proc table. Type is relevant
//  Returns 0 if successful, 1 if not found, negatives if errors occur
int pg_rm_targeted_proc(int type, char *procname);

// Adds ext to the targeted_ext table. If the file is already in the list, it will
// be deleted and readded
//  Returns 0 if successful, negatives if errors occur
int pg_add_targeted_ext(char *ext);

// Removes ext from the targeted_ext table
//  Returns 0 if successful, 1 if not found, negatives if errors occur
int pg_rm_targeted_ext(char *ext);

#endif // PGSQL_H