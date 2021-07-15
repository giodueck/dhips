#ifndef PGSQL_H
#define PGSQL_H

#include <libpq-fe.h>

#define SBUFSIZ 1024

// Close the connection to exit
static int exit_nicely (PGconn *conn);

// Opens a connection
// If the connection fails, returns NULL
static PGconn *open_conn(const char *conninfo);

// Copies the hashed passphrase stored for username into dest
// Returns 0 if successful, other if not
// If the resulting dest string is NULL, returns 2
int get_hashed_passphrase(char *username, char *dest);

// Checks password against the stored password for user by calling get_hashed_passphrase
//  Returns 1 if correct, 0 if not. If user does not exist, returns 2. If another error
// ocurrs, writes to stderr and returns -2. In any case, errors will be reported by 
// get_hashed_passphrase to stderr
int check_pass(char *user, char *password);

#endif // PGSQL_H