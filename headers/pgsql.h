#ifndef PGSQL_H
#define PGSQL_H

#include <libpq-fe.h>

// Close the connection to exit
static int exit_nicely (PGconn *conn);

// Opens a connection
// If the connection fails, returns NULL
static PGconn *open_conn(char *conninfo);

// Returns 1 if the password is correct, 0 of not
// If the connection fails, returns -1
int check_pwd(char *conninfo, char *username, char *hashpass);

#endif // PGSQL_H