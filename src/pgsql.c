#include "pgsql.h"
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <crypt.h>

static int exit_nicely(PGconn *conn)
{
    PQfinish(conn);
    return 1;
}

// Start db connection
static PGconn *open_conn(const char *conninfo)
{
    PGconn     *conn;
    PGresult   *res;

    /* Make a connection to the database */
    conn = PQconnectdb(conninfo);

    /* Check to see that the backend connection was successfully made */
    if (PQstatus(conn) != CONNECTION_OK)
    {
        fprintf(stderr, "Connection to database failed: %s", PQerrorMessage(conn));
        exit_nicely(conn);
        return NULL;
    }

    /* Set always-secure search path, so malicious users can't take control. */
    res = PQexec(conn, "SELECT pg_catalog.set_config('search_path', '', false)");
    if (PQresultStatus(res) != PGRES_TUPLES_OK)
    {
        fprintf(stderr, "SET failed: %s", PQerrorMessage(conn));
        PQclear(res);
        exit_nicely(conn);
        return NULL;
    }

    /*
     * Should PQclear PGresult whenever it is no longer needed to avoid memory
     * leaks
     */
    PQclear(res);

    return conn;
}

int get_hashed_passphrase(char *username, char *dest)
{
    char *connstring;
    FILE *file;

    PGconn     *conn;
    PGresult   *res;
    int         i,
                j;
    char cmd[512] = "";
    char buf[BUFSIZ];

    // Get connection string from config file
    file = fopen("/var/www/config/connstring", "r");    // connstring is restricted to root:www-data, apache should be able to access it
    if (!file)
    {
        fprintf(stderr, "fopen failed: %s", strerror(errno));
        return -2;
    }
    connstring = (char*)malloc(sizeof(char) * BUFSIZ);
    fgets(connstring, BUFSIZ, file);
    fclose(file);

    /* Make a connection to the database */
    conn = open_conn(connstring);
    free(connstring);   // avoid memory leaks
    if (!conn) return -1;

    /* Start a transaction block */
    res = PQexec(conn, "BEGIN");
    if (PQresultStatus(res) != PGRES_COMMAND_OK)
    {
        fprintf(stderr, "BEGIN command failed: %s", PQerrorMessage(conn));
        PQclear(res);
        exit_nicely(conn);
        return -1;
    }
    PQclear(res);

    // Retrieve the stored hashphrase for username
    // sprintf(cmd, "SELECT VERSION();");
    sprintf(cmd, "SELECT * FROM public.login WHERE username = '%s';", username);
    // Set stderr buffer to be buf
    setbuf(stderr, buf);
    res = PQexec(conn, cmd);

    if (PQresultStatus(res) != PGRES_TUPLES_OK)
    {
        // error retrieving data
        dest[0] = '\0';
        fprintf(stderr, "No data retrieved: %s\n", PQresultErrorMessage(res));
        PQclear(res);
        exit_nicely(conn);
        return 1;
    }

    int rows = PQntuples(res);
    if (rows)
        sprintf(dest, "%s", PQgetvalue(res, 0, 1));
    else dest = NULL;
    PQclear(res);

    /* end the transaction */
    res = PQexec(conn, "END");
    PQclear(res);

    /* close the connection to the database and cleanup */
    PQfinish(conn);

    if (!dest) return 2;
    return 0;
}

int check_pass(char *user, char *password)
{
    char pwd[SBUFSIZ];
    char hash[SBUFSIZ];
    int res;

    // get hash from database
    res = get_hashed_passphrase(user, pwd);
    if (res == 2) return -1;    // user does not exist
    else if (res == 0)          // user exists
    {
        // hash password using pwd as the setting
        strcpy(hash, crypt(password, pwd));

        // // debugging
        // printf("password: %s<br>hash phrase: %s (length %d)<br>stored hash: %s (length %d)<br>", password, hash, (int)strlen(hash), pwd, (int)strlen(pwd));
        // printf("strcmp(hash, pwd) = %d<br>", strcmp(hash, pwd));

        // compare hashes
        if (strcmp(hash, pwd) == 0)
        {
            // valid password
            return 1;
        } else return 0;
    }
    else return -2;             // other error, check stderr
}