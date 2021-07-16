#ifndef LOGIN_H
#define LOGIN_H

#include <time.h>

// Time in minutes before a session expires
#define SESSION_LIFETIME 5

// Checks to see if the session for the current user exists and is valid
// Returns 0 and renews it if a valid session is found, else returns 1 and redirects to
// the login page
int verify_session(int &session, char *user);

// Fetches POST data and uses it to log in.
// If successful creates a session and reloads the page with a GET request,
// if not redirects back to login page with an error message for the user
int login();

#endif // LOGIN_H