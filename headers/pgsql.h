#ifndef PGSQL_H
#define PGSQL_H

#include <libpq-fe.h>

// Close the connection to exit
static int exit_nicely (PGconn *conn);

// Opens a connection
// If the connection fails, returns NULL
static PGconn *open_conn(const char *conninfo);

// Copies the hashed passphrase stored for username into dest
// Returns 0 if successful, other if not
// If the resulting dest string is NULL, returns 2
int get_hashed_passphrase(char *username, char *dest);

#endif // PGSQL_H