#include "pgsql.h"
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <crypt.h>
#include <time.h>

static int exit_nicely(PGconn *conn)
{
    PQfinish(conn);
    return 1;
}

// Start db connection
static PGconn *open_conn()
{
    PGconn     *conn;
    PGresult   *res;
    FILE *file;
    char *conninfo;

    // Get connection string from config file
    file = fopen("/var/www/config/connstring", "r");    // connstring is restricted to root:www-data, apache should be able to access it
    if (!file)
    {
        fprintf(stderr, "fopen failed: %s", strerror(errno));
        return NULL;
    }
    conninfo = (char*)malloc(sizeof(char) * BUFSIZ);
    fgets(conninfo, BUFSIZ, file);
    fclose(file);

    /* Make a connection to the database */
    conn = PQconnectdb(conninfo);
    free(conninfo);   // avoid memory leaks

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
    PGconn     *conn;
    PGresult   *res;
    char cmd[512] = "";
    char buf[BUFSIZ];

    /* Make a connection to the database */
    conn = open_conn();
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

int check_session(char *username, int lifetime)
{
    PGconn     *conn;
    PGresult   *res;
    char cmd[512] = "";
    char buf[BUFSIZ];

    double exp_epoch;
    double current_epoch;
    long sid;
    int result;

    /* Make a connection to the database */
    conn = open_conn();
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

    // Retrieve the last session for username
    sprintf(cmd, "SELECT session_id, EXTRACT(EPOCH FROM expiration) FROM public.session WHERE username = '%s' ORDER BY expiration DESC LIMIT 1;", username);
    // Set stderr buffer to be buf
    setbuf(stderr, buf);
    res = PQexec(conn, cmd);

    if (PQresultStatus(res) != PGRES_TUPLES_OK)
    {
        // error retrieving data
        fprintf(stderr, "No data retrieved: %s\n", PQresultErrorMessage(res));
        PQclear(res);
        exit_nicely(conn);
        return -1;
    }

    int rows = PQntuples(res);
    if (rows)
    {
        sid = atol(PQgetvalue(res, 0, 0));
        exp_epoch = atof(PQgetvalue(res, 0, 1));
    }
    else exp_epoch = 0;
    PQclear(res);

    // Get current time in unix epoch
    if (exp_epoch > 0)
    {
        // Retrieve the last session for username
        sprintf(cmd, "SELECT EXTRACT(EPOCH FROM LOCALTIMESTAMP);");
        res = PQexec(conn, cmd);

        if (PQresultStatus(res) != PGRES_TUPLES_OK)
        {
            // error retrieving data
            fprintf(stderr, "No data retrieved: %s\n", PQresultErrorMessage(res));
            PQclear(res);
            exit_nicely(conn);
            return -1;
        }

        int rows = PQntuples(res);
        if (rows)
        {
            current_epoch = atof(PQgetvalue(res, 0, 0));
        }
        PQclear(res);
    }

    // Check if the expiration time has past
    if (exp_epoch == 0) result = 0; // no session
    else if (exp_epoch > current_epoch) // not expired
    {
        // renew session
        sprintf(cmd, "UPDATE public.session SET expiration = LOCALTIMESTAMP + INTERVAL '%d minutes' WHERE session_id = %ld;", lifetime, sid);
        res = PQexec(conn, cmd);
        result = 1;
    } else result = 2;  // expired

    /* end the transaction */
    res = PQexec(conn, "END");
    PQclear(res);

    /* close the connection to the database and cleanup */
    PQfinish(conn);

    return result;
}

int create_session(char *username, int lifetime)
{
    PGconn     *conn;
    PGresult   *res;
    char cmd[512] = "";
    char buf[BUFSIZ];

    /* Make a connection to the database */
    conn = open_conn();
    if (!conn) return 1;

    /* Start a transaction block */
    res = PQexec(conn, "BEGIN");
    if (PQresultStatus(res) != PGRES_COMMAND_OK)
    {
        fprintf(stderr, "BEGIN command failed: %s", PQerrorMessage(conn));
        PQclear(res);
        exit_nicely(conn);
        return 1;
    }
    PQclear(res);

    // Insert new session with current timestamp
    sprintf(cmd, "INSERT INTO public.session (username, expiration, creation) VALUES ('%s', LOCALTIMESTAMP + INTERVAL '%d minutes', LOCALTIMESTAMP);", username, lifetime);
    res = PQexec(conn, cmd);

    /* end the transaction */
    res = PQexec(conn, "END");
    PQclear(res);

    /* close the connection to the database and cleanup */
    PQfinish(conn);

    return 0;
}