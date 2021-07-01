#include <iostream>
#include <unistd.h>
#include "login.h"

using namespace std;

int main()
{
    // uid_t uid = getuid();
    // // try to switch to root
    // if (setuid(0))
    // {
    //     cerr << "Execute as superuser\n";
    //     return -1;
    // }
    // // recover uid
    // setuid(uid);

    login();

    return 0;
}