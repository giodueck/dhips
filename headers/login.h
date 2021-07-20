#ifndef LOGIN_H
#define LOGIN_H

#include <time.h>

// Time in minutes before a session expires
#define SESSION_LIFETIME 5

// Checks to see if the session for the current user exists and is valid.
// Use this version to get the query variables variables
//  Returns the query string. Renews the session if a valid one is found, else redirects to
// the login page
char **verify_session(int *session, char *user);

// Checks to see if the session for the current user exists and is valid.
// Use this version to use the query variables
//  Returns 0 and renews it if a valid session is found, else returns 1 and redirects to
// the login page
// int verify_session(int session, char *user);

// Fetches POST data and uses it to log in.
// If successful creates a session and reloads the page with a GET request,
// if not redirects back to login page with an error message for the user
int login();

// Changes the password for the given user, normally the user logged in
//  Returns 0 if the password was changed, else returns negatives
int change_passwd(char *user, int session, int e);

#endif // LOGIN_H