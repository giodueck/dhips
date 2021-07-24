#include "pgsql.h"
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <crypt.h>
#include <time.h>

// Close the connection to exit
static int exit_nicely(PGconn *conn)
{
    PQfinish(conn);
    return 1;
}

// Opens a connection
// If the connection fails, returns NULL
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
        fprintf(stderr, "dhips//pgsql: could not open /var/www/config/connstring: %s\n", strerror(errno));
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
        fprintf(stderr, "dhips//pgsql: connection to database failed: %s\n", PQerrorMessage(conn));
        exit_nicely(conn);
        return NULL;
    }

    /* Set always-secure search path, so malicious users can't take control. */
    res = PQexec(conn, "SELECT pg_catalog.set_config('search_path', '', false)");
    if (PQresultStatus(res) != PGRES_TUPLES_OK)
    {
        fprintf(stderr, "dhips//pgsql: SET failed: %s\n", PQerrorMessage(conn));
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

int pg_get_hashed_passphrase(char *username, char *dest)
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
        fprintf(stderr, "dhips//pgsql: BEGIN command failed: %s\n", PQerrorMessage(conn));
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
        fprintf(stderr, "dhips//pgsql pg_get_hashed_passphrase: no data retrieved: %s\n", PQresultErrorMessage(res));
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

int pg_check_pass(char *user, char *password)
{
    char pwd[SBUFSIZ];
    char hash[SBUFSIZ];
    int res;

    // get hash from database
    res = pg_get_hashed_passphrase(user, pwd);
    if (res == 2) return -1;    // user does not exist
    else if (res == 0)          // user exists
    {
        // hash password using pwd as the setting
        strcpy(hash, crypt(password, pwd));

        // compare hashes
        if (strcmp(hash, pwd) == 0)
        {
            // valid password
            return 1;
        } else return 0;
    }
    else return -2;             // other error, check stderr
}

int pg_change_pass(char *user, char *hash)
{
    PGconn     *conn;
    PGresult   *res;
    char cmd[512] = "";
    char buf[BUFSIZ];

    char *rets;

    /* Make a connection to the database */
    conn = open_conn();
    if (!conn) return -1;

    /* Start a transaction block */
    res = PQexec(conn, "BEGIN");
    if (PQresultStatus(res) != PGRES_COMMAND_OK)
    {
        fprintf(stderr, "dhips//pgsql: BEGIN command failed: %s", PQerrorMessage(conn));
        PQclear(res);
        exit_nicely(conn);
        return -1;
    }
    PQclear(res);

    // Update the stored hashphrase for user
    sprintf(cmd, "UPDATE public.login SET pwd = '%s' WHERE username = '%s';", hash, user);
    // Set stderr buffer to be buf
    setbuf(stderr, buf);
    res = PQexec(conn, cmd);

    // check if update was successful
    rets = PQcmdStatus(res);
    if (strcmp(rets, "UPDATE 1") != 0)
    {
        fprintf(stderr, "dhips//pgsql pg_change_pass: no data updated: %s", PQerrorMessage(conn));
        PQclear(res);
        exit_nicely(conn);
        return -2;
    }
    PQclear(res);

    /* end the transaction */
    res = PQexec(conn, "END");
    PQclear(res);

    /* close the connection to the database and cleanup */
    PQfinish(conn);
    return 0;
}

int pg_change_role(char *user, char *role)
{
    PGconn     *conn;
    PGresult   *res;
    char cmd[512] = "";
    char buf[BUFSIZ];

    char *rets;

    /* Make a connection to the database */
    conn = open_conn();
    if (!conn) return -1;

    /* Start a transaction block */
    res = PQexec(conn, "BEGIN");
    if (PQresultStatus(res) != PGRES_COMMAND_OK)
    {
        fprintf(stderr, "dhips//pgsql: BEGIN command failed: %s", PQerrorMessage(conn));
        PQclear(res);
        exit_nicely(conn);
        return -1;
    }
    PQclear(res);

    // Update the role for user
    sprintf(cmd, "UPDATE public.login SET role = '%s' WHERE username = '%s';", role, user);
    // Set stderr buffer to be buf
    setbuf(stderr, buf);
    res = PQexec(conn, cmd);

    // check if update was successful
    rets = PQcmdStatus(res);
    if (strcmp(rets, "UPDATE 1") != 0)
    {
        fprintf(stderr, "dhips//pgsql pg_change_role: no data updated: %s", PQerrorMessage(conn));
        PQclear(res);
        exit_nicely(conn);
        return -2;
    }
    PQclear(res);

    /* end the transaction */
    res = PQexec(conn, "END");
    PQclear(res);

    /* close the connection to the database and cleanup */
    PQfinish(conn);
    return 0;
}

int pg_add_user(char *username, char *hash, char *role)
{
    PGconn     *conn;
    PGresult   *res;
    char cmd[512] = "";
    char buf[BUFSIZ];

    char *rets;
    int ret;

    /* Make a connection to the database */
    conn = open_conn();
    if (!conn) return -1;

    /* Start a transaction block */
    res = PQexec(conn, "BEGIN");
    if (PQresultStatus(res) != PGRES_COMMAND_OK)
    {
        fprintf(stderr, "dhips//pgsql: BEGIN command failed: %s", PQerrorMessage(conn));
        PQclear(res);
        exit_nicely(conn);
        return -1;
    }
    PQclear(res);

    // Insert new user
    sprintf(cmd, "INSERT INTO public.login VALUES ('%s', '%s', '%s');", username, hash, role);
    // Set stderr buffer to be buf
    setbuf(stderr, buf);
    res = PQexec(conn, cmd);

    rets = PQcmdStatus(res);
    if (strcmp(rets, "INSERT 0 1") != 0)
    {
        fprintf(stderr, "dhips//pgsql pg_add_user: no data inserted: %s", PQerrorMessage(conn));
        PQclear(res);
        exit_nicely(conn);
        return 1;
    }
    PQclear(res);

    /* end the transaction */
    res = PQexec(conn, "END");
    PQclear(res);

    /* close the connection to the database and cleanup */
    PQfinish(conn);
    return 0;
}

int pg_delete_user(char *username)
{
    PGconn     *conn;
    PGresult   *res;
    char cmd[512] = "";
    char buf[BUFSIZ];

    char *rets;
    int ret;

    /* Make a connection to the database */
    conn = open_conn();
    if (!conn) return -1;

    /* Start a transaction block */
    res = PQexec(conn, "BEGIN");
    if (PQresultStatus(res) != PGRES_COMMAND_OK)
    {
        fprintf(stderr, "dhips//pgsql: BEGIN command failed: %s", PQerrorMessage(conn));
        PQclear(res);
        exit_nicely(conn);
        return -1;
    }
    PQclear(res);

    // Delete user sessions
    sprintf(cmd, "DELETE FROM public.session WHERE username = '%s';", username);
    // Set stderr buffer to be buf
    setbuf(stderr, buf);
    res = PQexec(conn, cmd);
    PQclear(res);

    // Delete user
    sprintf(cmd, "DELETE FROM public.login WHERE username = '%s';", username);
    // Set stderr buffer to be buf
    setbuf(stderr, buf);
    res = PQexec(conn, cmd);

    // check if delete was successful
    rets = PQcmdStatus(res);
    if (strcmp(rets, "DELETE 1") != 0)
    {
        fprintf(stderr, "dhips//pgsql pg_delete_user: no data deleted: %s", PQerrorMessage(conn));
        PQclear(res);
        exit_nicely(conn);
        return -2;
    }
    PQclear(res);

    /* end the transaction */
    res = PQexec(conn, "END");
    PQclear(res);

    /* close the connection to the database and cleanup */
    PQfinish(conn);
    return 0;
}

int pg_get_role(char *user)
{
    PGconn     *conn;
    PGresult   *res;
    char cmd[512] = "";
    char buf[BUFSIZ];

    char *rets;
    int ret;

    /* Make a connection to the database */
    conn = open_conn();
    if (!conn) return -1;

    /* Start a transaction block */
    res = PQexec(conn, "BEGIN");
    if (PQresultStatus(res) != PGRES_COMMAND_OK)
    {
        fprintf(stderr, "dhips//pgsql: BEGIN command failed: %s", PQerrorMessage(conn));
        PQclear(res);
        exit_nicely(conn);
        return -1;
    }
    PQclear(res);

    // Retrieve role
    sprintf(cmd, "SELECT \"role\" FROM public.login WHERE username = '%s';", user);
    // Set stderr buffer to be buf
    setbuf(stderr, buf);
    res = PQexec(conn, cmd);

    if (PQresultStatus(res) != PGRES_TUPLES_OK)
    {
        // error retrieving data
        fprintf(stderr, "dhips//pgsql pg_get_role: no data retrieved: %s\n", PQresultErrorMessage(res));
        PQclear(res);
        exit_nicely(conn);
        return -1;
    }

    int rows = PQntuples(res);
    if (rows)
    {
        rets = PQgetvalue(res, 0, 0);
    }
    PQclear(res);

    // determine role
    if (strcmp(rets, "admin") == 0)
        ret = 0;
    else if (strcmp(rets, "user") == 0)
        ret = 1;
    else if (strcmp(rets, "spectator") == 0)
        ret = 2;
    else
        ret = 3;

    /* end the transaction */
    res = PQexec(conn, "END");
    PQclear(res);

    /* close the connection to the database and cleanup */
    PQfinish(conn);

    return ret;
}

int pg_get_users(char ***dest)
{
    PGconn     *conn;
    PGresult   *res;
    char cmd[512] = "";
    char buf[BUFSIZ];

    char *rets;
    int n;

    /* Make a connection to the database */
    conn = open_conn();
    if (!conn) return -1;

    /* Start a transaction block */
    res = PQexec(conn, "BEGIN");
    if (PQresultStatus(res) != PGRES_COMMAND_OK)
    {
        fprintf(stderr, "dhips//pgsql: BEGIN command failed: %s", PQerrorMessage(conn));
        PQclear(res);
        exit_nicely(conn);
        return -1;
    }
    PQclear(res);

    // Retrieve all users
    sprintf(cmd, "SELECT username FROM public.login;");
    // Set stderr buffer to be buf
    setbuf(stderr, buf);
    res = PQexec(conn, cmd);

    if (PQresultStatus(res) != PGRES_TUPLES_OK)
    {
        // error retrieving data
        fprintf(stderr, "dhips//pgsql pg_get_users: no data retrieved: %s\n", PQresultErrorMessage(res));
        PQclear(res);
        exit_nicely(conn);
        return -1;
    }

    // allocate space for array
    n = PQntuples(res);
    *dest = (char**)malloc(sizeof(char*) * n);
    if (!dest)
    {
        // malloc error
        fprintf(stderr, "dhips//pgsql pg_get_users: memory allocation failed\n");
        PQclear(res);
        exit_nicely(conn);
        return -1;
    }

    for (int i = 0; i < n; i++)
    {
        // allocate space for and store usernames
        rets = PQgetvalue(res, i, 0);
        (*dest)[i] = (char*)malloc(strlen(rets) + 1);
        if (!(*dest)[i])
        {
            // malloc error
            fprintf(stderr, "dhips//pgsql pg_get_users: memory allocation failed\n");
            PQclear(res);
            exit_nicely(conn);
            return -1;
        }
        strcpy((*dest)[i], rets);
    }
    PQclear(res);

    /* end the transaction */
    res = PQexec(conn, "END");
    PQclear(res);

    /* close the connection to the database and cleanup */
    PQfinish(conn);

    return n;
}

int pg_check_session(char *username, int session, int lifetime)
{
    PGconn     *conn;
    PGresult   *res;
    char cmd[512] = "";
    char buf[BUFSIZ];

    char retstr[64];
    double exp_epoch;
    double current_epoch;
    int sid;
    int result;

    /* Make a connection to the database */
    conn = open_conn();
    if (!conn) return -1;

    /* Start a transaction block */
    res = PQexec(conn, "BEGIN");
    if (PQresultStatus(res) != PGRES_COMMAND_OK)
    {
        fprintf(stderr, "dhips//pgsql: BEGIN command failed: %s", PQerrorMessage(conn));
        PQclear(res);
        exit_nicely(conn);
        return -1;
    }
    PQclear(res);

    // Retrieve the last session for username
    sprintf(cmd, "SELECT EXTRACT(EPOCH FROM expiration) FROM public.session WHERE username = '%s' and session_id = %d;", username, session);
    // Set stderr buffer to be buf
    setbuf(stderr, buf);
    res = PQexec(conn, cmd);

    if (PQresultStatus(res) != PGRES_TUPLES_OK)
    {
        // error retrieving data
        fprintf(stderr, "dhips//pgsql pg_check_session: no data retrieved: %s\n", PQresultErrorMessage(res));
        PQclear(res);
        exit_nicely(conn);
        return -1;
    }

    int rows = PQntuples(res);
    if (rows)
    {
        sprintf(retstr, "%s", PQgetvalue(res, 0, 0));
        if (strcmp(retstr, "(null)") == 0)
            exp_epoch = -1; // session ended
        else
            exp_epoch = atof(retstr);
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
            fprintf(stderr, "dhips//pgsql pg_check_session: no data retrieved: %s\n", PQresultErrorMessage(res));
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
        sprintf(cmd, "UPDATE public.session SET expiration = LOCALTIMESTAMP + INTERVAL '%d minutes' WHERE session_id = %d;", lifetime, session);
        res = PQexec(conn, cmd);
        result = 1;
    } else if (exp_epoch == -1) // session ended
    {
        result = 3;
    } else result = 2;  // expired

    /* end the transaction */
    res = PQexec(conn, "END");
    PQclear(res);

    /* close the connection to the database and cleanup */
    PQfinish(conn);

    return result;
}

int pg_create_session(char *username, int lifetime)
{
    PGconn     *conn;
    PGresult   *res;
    char cmd[512] = "";
    char buf[BUFSIZ];

    int sid;

    /* Make a connection to the database */
    conn = open_conn();
    if (!conn) return -1;

    /* Start a transaction block */
    res = PQexec(conn, "BEGIN");
    if (PQresultStatus(res) != PGRES_COMMAND_OK)
    {
        fprintf(stderr, "dhips//pgsql: BEGIN command failed: %s", PQerrorMessage(conn));
        PQclear(res);
        exit_nicely(conn);
        return -1;
    }
    PQclear(res);

    // Insert new session with current timestamp
    sprintf(cmd, "INSERT INTO public.session (username, expiration, creation) VALUES ('%s', LOCALTIMESTAMP + INTERVAL '%d minutes', LOCALTIMESTAMP) RETURNING session_id;", username, lifetime);
    res = PQexec(conn, cmd);
    if (PQresultStatus(res) != PGRES_TUPLES_OK)
    {
        // error retrieving data
        fprintf(stderr, "dhips//pgsql pg_create_session: no data retrieved: %s\n", PQresultErrorMessage(res));
        PQclear(res);
        exit_nicely(conn);
        return -1;
    }
    sid = atoi(PQgetvalue(res, 0, 0));

    /* end the transaction */
    res = PQexec(conn, "END");
    PQclear(res);

    /* close the connection to the database and cleanup */
    PQfinish(conn);

    return sid;
}

int pg_terminate_session(int session)
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
        fprintf(stderr, "dhips//pgsql: BEGIN command failed: %s", PQerrorMessage(conn));
        PQclear(res);
        exit_nicely(conn);
        return -1;
    }
    PQclear(res);

    // Insert new session with current timestamp
    sprintf(cmd, "UPDATE public.session SET expiration = NULL WHERE session_id = %d;", session);
    res = PQexec(conn, cmd);
    PQclear(res);

    /* end the transaction */
    res = PQexec(conn, "END");
    PQclear(res);

    /* close the connection to the database and cleanup */
    PQfinish(conn);

    return 0;
}

int pg_get_alarm_severity(int alarm_id)
{
    PGconn     *conn;
    PGresult   *res;
    char cmd[512] = "";
    char buf[BUFSIZ];

    int ret;

    /* Make a connection to the database */
    conn = open_conn();
    if (!conn) return -1;

    /* Start a transaction block */
    res = PQexec(conn, "BEGIN");
    if (PQresultStatus(res) != PGRES_COMMAND_OK)
    {
        fprintf(stderr, "dhips//pgsql: BEGIN command failed: %s", PQerrorMessage(conn));
        PQclear(res);
        exit_nicely(conn);
        return -1;
    }
    PQclear(res);

    // Retrieve severity
    sprintf(cmd, "SELECT severity FROM public.alarm WHERE alarm_id = %d;", alarm_id);
    // Set stderr buffer to be buf
    setbuf(stderr, buf);
    res = PQexec(conn, cmd);

    if (PQresultStatus(res) != PGRES_TUPLES_OK)
    {
        // error retrieving data
        fprintf(stderr, "dhips//pgsql pg_get_alarm_severity: no data retrieved: %s\n", PQresultErrorMessage(res));
        PQclear(res);
        exit_nicely(conn);
        return -1;
    }

    int rows = PQntuples(res);
    if (rows)
    {
        ret = atoi(PQgetvalue(res, 0, 0));
    }
    PQclear(res);

    /* end the transaction */
    res = PQexec(conn, "END");
    PQclear(res);

    /* close the connection to the database and cleanup */
    PQfinish(conn);

    return ret;
}

char *pg_get_alarm_description(int alarm_id)
{
    PGconn     *conn;
    PGresult   *res;
    char cmd[512] = "";
    char buf[BUFSIZ];
    
    char *description;
    char *ret = (char*)malloc(513); // description is varchar(512)

    /* Make a connection to the database */
    conn = open_conn();
    if (!conn) return NULL;

    /* Start a transaction block */
    res = PQexec(conn, "BEGIN");
    if (PQresultStatus(res) != PGRES_COMMAND_OK)
    {
        fprintf(stderr, "dhips//pgsql: BEGIN command failed: %s", PQerrorMessage(conn));
        PQclear(res);
        exit_nicely(conn);
        return NULL;
    }
    PQclear(res);

    // Retrieve description
    sprintf(cmd, "SELECT description FROM public.alarm WHERE alarm_id = %d;", alarm_id);
    // Set stderr buffer to be buf
    setbuf(stderr, buf);
    res = PQexec(conn, cmd);

    if (PQresultStatus(res) != PGRES_TUPLES_OK)
    {
        // error retrieving data
        fprintf(stderr, "dhips//pgsql pg_get_alarm_description: no data retrieved: %s\n", PQresultErrorMessage(res));
        PQclear(res);
        exit_nicely(conn);
        return NULL;
    }

    int rows = PQntuples(res);
    if (rows > 0)
    {
        description = PQgetvalue(res, 0, 0);
        strcpy(ret, description);
    } else ret = NULL;
    PQclear(res);

    /* end the transaction */
    res = PQexec(conn, "END");
    PQclear(res);

    /* close the connection to the database and cleanup */
    PQfinish(conn);

    return ret;
}

int pg_check_db()
{
    PGconn     *conn;
    PGresult   *res;
    char cmd[512] = "";
    char buf[BUFSIZ];

    int ret;

    /* Make a connection to the database */
    conn = open_conn();
    if (!conn) return -1;

    /* Start a transaction block */
    res = PQexec(conn, "BEGIN");
    if (PQresultStatus(res) != PGRES_COMMAND_OK)
    {
        fprintf(stderr, "dhips//pgsql: BEGIN command failed: %s", PQerrorMessage(conn));
        PQclear(res);
        exit_nicely(conn);
        return -1;
    }
    PQclear(res);

    // Set stderr buffer to be buf
    setbuf(stderr, buf);

    // Test every table
    
    res = PQexec(conn, "SELECT * FROM public.login LIMIT 1;");
    if (PQresultStatus(res) != PGRES_TUPLES_OK)
    {
        // error retrieving data
        fprintf(stderr, "dhips//pgsql pg_check_db: error: %s\n", PQresultErrorMessage(res));
        PQclear(res);
        exit_nicely(conn);
        return -1;
    }
    PQclear(res);

    res = PQexec(conn, "SELECT * FROM public.session LIMIT 1;");
    if (PQresultStatus(res) != PGRES_TUPLES_OK)
    {
        // error retrieving data
        fprintf(stderr, "dhips//pgsql pg_check_db: error: %s\n", PQresultErrorMessage(res));
        PQclear(res);
        exit_nicely(conn);
        return -1;
    }
    PQclear(res);

    res = PQexec(conn, "SELECT * FROM public.alarm LIMIT 1;");
    if (PQresultStatus(res) != PGRES_TUPLES_OK)
    {
        // error retrieving data
        fprintf(stderr, "dhips//pgsql pg_check_db: error: %s\n", PQresultErrorMessage(res));
        PQclear(res);
        exit_nicely(conn);
        return -1;
    }
    PQclear(res);

    /* end the transaction */
    res = PQexec(conn, "END");
    PQclear(res);

    /* close the connection to the database and cleanup */
    PQfinish(conn);

    return ret;
}

int pg_get_monitor_filename(int index, int type, char **dest)
{
    PGconn     *conn;
    PGresult   *res;
    char cmd[512] = "";
    char buf[BUFSIZ];
    
    char *fullfilename;
    int ret;

    /* Make a connection to the database */
    conn = open_conn();
    if (!conn) return -1;

    /* Start a transaction block */
    res = PQexec(conn, "BEGIN");
    if (PQresultStatus(res) != PGRES_COMMAND_OK)
    {
        fprintf(stderr, "dhips//pgsql: BEGIN command failed: %s", PQerrorMessage(conn));
        PQclear(res);
        exit_nicely(conn);
        return -1;
    }
    PQclear(res);

    // Retrieve fullfilename
    sprintf(cmd, "SELECT fullfilename, monitor_id FROM public.monitor WHERE monitor_id > %d AND type = %d AND active = true LIMIT 1;", index, type);
    // Set stderr buffer to be buf
    setbuf(stderr, buf);
    res = PQexec(conn, cmd);

    if (PQresultStatus(res) != PGRES_TUPLES_OK)
    {
        // error retrieving data
        fprintf(stderr, "dhips//pgsql pg_get_monitor_filename: no data retrieved: %s\n", PQresultErrorMessage(res));
        PQclear(res);
        exit_nicely(conn);
        return -1;
    }

    // copy 
    int rows = PQntuples(res);
    if (rows > 0)
    {
        fullfilename = PQgetvalue(res, 0, 0);
        *dest = (char*)malloc(strlen(fullfilename) + 1);
        strcpy(*dest, fullfilename);
        ret = atoi(PQgetvalue(res, 0, 1));
    } else ret = 0;
    PQclear(res);

    /* end the transaction */
    res = PQexec(conn, "END");
    PQclear(res);

    /* close the connection to the database and cleanup */
    PQfinish(conn);

    return ret;
}

int pg_get_monitor_filename_conf(int index, int type, char **dest, int *active)
{
    PGconn     *conn;
    PGresult   *res;
    char cmd[512] = "";
    char buf[BUFSIZ];
    
    char *fullfilename;
    int ret;

    /* Make a connection to the database */
    conn = open_conn();
    if (!conn) return -1;

    /* Start a transaction block */
    res = PQexec(conn, "BEGIN");
    if (PQresultStatus(res) != PGRES_COMMAND_OK)
    {
        fprintf(stderr, "dhips//pgsql: BEGIN command failed: %s", PQerrorMessage(conn));
        PQclear(res);
        exit_nicely(conn);
        return -1;
    }
    PQclear(res);

    // Retrieve fullfilename
    sprintf(cmd, "SELECT fullfilename, monitor_id, active FROM public.monitor WHERE monitor_id > %d AND type = %d LIMIT 1;", index, type);
    // Set stderr buffer to be buf
    setbuf(stderr, buf);
    res = PQexec(conn, cmd);

    if (PQresultStatus(res) != PGRES_TUPLES_OK)
    {
        // error retrieving data
        fprintf(stderr, "dhips//pgsql pg_get_monitor_filename: no data retrieved: %s\n", PQresultErrorMessage(res));
        PQclear(res);
        exit_nicely(conn);
        return -1;
    }

    // copy 
    int rows = PQntuples(res);
    if (rows > 0)
    {
        fullfilename = PQgetvalue(res, 0, 0);
        *dest = (char*)malloc(strlen(fullfilename) + 1);
        strcpy(*dest, fullfilename);
        ret = atoi(PQgetvalue(res, 0, 1));
        if (strcmp("true", PQgetvalue(res, 0, 2)) == 0 || strcmp("t", PQgetvalue(res, 0, 2)) == 0)
            *active = 1;
        else *active = 0;
    } else ret = 0;
    PQclear(res);

    /* end the transaction */
    res = PQexec(conn, "END");
    PQclear(res);

    /* close the connection to the database and cleanup */
    PQfinish(conn);

    return ret;
}

int pg_get_targeted_proc_name(int index, int type, char **dest)
{
    PGconn     *conn;
    PGresult   *res;
    char cmd[512] = "";
    char buf[BUFSIZ];
    
    char *proc_name;
    int ret;

    /* Make a connection to the database */
    conn = open_conn();
    if (!conn) return -1;

    /* Start a transaction block */
    res = PQexec(conn, "BEGIN");
    if (PQresultStatus(res) != PGRES_COMMAND_OK)
    {
        fprintf(stderr, "dhips//pgsql: BEGIN command failed: %s", PQerrorMessage(conn));
        PQclear(res);
        exit_nicely(conn);
        return -1;
    }
    PQclear(res);

    // Retrieve process name
    sprintf(cmd, "SELECT name, targeted_proc_id FROM public.targeted_proc WHERE targeted_proc_id > %d AND type = %d AND active = true LIMIT 1;", index, type);
    // Set stderr buffer to be buf
    setbuf(stderr, buf);
    res = PQexec(conn, cmd);

    if (PQresultStatus(res) != PGRES_TUPLES_OK)
    {
        // error retrieving data
        fprintf(stderr, "dhips//pgsql pg_get_targeted_proc_name: no data retrieved: %s\n", PQresultErrorMessage(res));
        PQclear(res);
        exit_nicely(conn);
        return -1;
    }

    // copy 
    int rows = PQntuples(res);
    if (rows > 0)
    {
        proc_name = PQgetvalue(res, 0, 0);
        *dest = (char*)malloc(strlen(proc_name) + 1);
        strcpy(*dest, proc_name);
        ret = atoi(PQgetvalue(res, 0, 1));
    } else ret = 0;
    PQclear(res);

    /* end the transaction */
    res = PQexec(conn, "END");
    PQclear(res);

    /* close the connection to the database and cleanup */
    PQfinish(conn);

    return ret;
}

int pg_module_enabled(int module)
{
    PGconn     *conn;
    PGresult   *res;
    char cmd[512] = "";
    char buf[BUFSIZ];

    int ret;

    /* Make a connection to the database */
    conn = open_conn();
    if (!conn) return -1;

    /* Start a transaction block */
    res = PQexec(conn, "BEGIN");
    if (PQresultStatus(res) != PGRES_COMMAND_OK)
    {
        fprintf(stderr, "dhips//pgsql: BEGIN command failed: %s", PQerrorMessage(conn));
        PQclear(res);
        exit_nicely(conn);
        return -1;
    }
    PQclear(res);

    // Retrieve module status
    sprintf(cmd, "SELECT status FROM public.module_status WHERE module_id = %d;", module);
    // Set stderr buffer to be buf
    setbuf(stderr, buf);
    res = PQexec(conn, cmd);

    if (PQresultStatus(res) != PGRES_TUPLES_OK)
    {
        // error retrieving data
        fprintf(stderr, "dhips//pgsql pg_module_enabled: no data retrieved: %s\n", PQresultErrorMessage(res));
        PQclear(res);
        exit_nicely(conn);
        return -1;
    }

    int rows = PQntuples(res);
    if (rows)
    {
        if (strcmp("true", PQgetvalue(res, 0, 0)) == 0 || strcmp("t", PQgetvalue(res, 0, 0)) == 0)
            ret = 1;
        else ret = 0;
    } else ret = 2;
    PQclear(res);

    /* end the transaction */
    res = PQexec(conn, "END");
    PQclear(res);

    /* close the connection to the database and cleanup */
    PQfinish(conn);

    return ret;
}

int pg_module_toggle(int module, int status)
{
    PGconn     *conn;
    PGresult   *res;
    char cmd[512] = "";
    char buf[BUFSIZ];

    int ret;

    /* Make a connection to the database */
    conn = open_conn();
    if (!conn) return -1;

    /* Start a transaction block */
    res = PQexec(conn, "BEGIN");
    if (PQresultStatus(res) != PGRES_COMMAND_OK)
    {
        fprintf(stderr, "dhips//pgsql: BEGIN command failed: %s", PQerrorMessage(conn));
        PQclear(res);
        exit_nicely(conn);
        return -1;
    }
    PQclear(res);

    // Set module status
    sprintf(cmd, "UPDATE public.module_status SET status = %s WHERE module_id = %d RETURNING module_id;", (status == 1) ? "true" : "false", module);
    // Set stderr buffer to be buf
    setbuf(stderr, buf);
    res = PQexec(conn, cmd);

    if (PQresultStatus(res) != PGRES_TUPLES_OK)
    {
        // error retrieving data
        fprintf(stderr, "dhips//pgsql pg_module_enabled: no data retrieved: %s\n", PQresultErrorMessage(res));
        PQclear(res);
        exit_nicely(conn);
        return -1;
    }

    int rows = PQntuples(res);
    if (rows)
    {
        ret = 0;
    } else ret = 1;
    PQclear(res);

    /* end the transaction */
    res = PQexec(conn, "END");
    PQclear(res);

    /* close the connection to the database and cleanup */
    PQfinish(conn);

    return ret;
}
