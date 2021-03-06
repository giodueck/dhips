#include "dhipslib.h"
#include "pgsql.h"
#include <iostream>
#include <string>
#include <cstring>
#include <dirent.h>
#include <limits.h>
#include <unistd.h>
#include <vector>
#include <sys/wait.h>

using namespace std;

void dhips_perror(const char *msg)
{
    string errmsg;
    errmsg = "dhips: " + string(msg);
    perror(errmsg.c_str());
}

void dhips_perror_no_errno(const char *msg)
{
    cerr << "dhips: " << string(msg) << endl;
}

void dhips_get_pid_by_name(pid_t *pid, const char *task_name)
{
    DIR *dir;
    struct dirent *ptr;
    FILE *fp;
    char filepath[PATH_MAX];
    char cur_task_name[FILENAME_MAX];
    char buf[BUFSIZ];

    dir = opendir("/proc"); 
    if (NULL != dir)
    {
        while ((ptr = readdir(dir)) != NULL) //Cycle read every file/folder under /proc
        {
            // Skip if it reads "." or "..", and skip if it reads not the folder name
            if ((strcmp(ptr->d_name, ".") == 0) || (strcmp(ptr->d_name, "..") == 0))
                continue;
            if (DT_DIR != ptr->d_type)
                continue;

            //Generate the path of the file to be read
            sprintf(filepath, "/proc/%s/status", ptr->d_name);
            fp = fopen(filepath, "r");
            if (NULL != fp)
            {
                if( fgets(buf, BUFSIZ-1, fp)== NULL ){
                    fclose(fp);
                    continue;
                }
                sscanf(buf, "%*s %s", cur_task_name);

                //If the file content meets the requirements, print the name of the path (that is, the PID of the process)
                if (!strcmp(task_name, cur_task_name)){
                    sscanf(ptr->d_name, "%d", pid);
                }
                fclose(fp);
            }
        }
        closedir(dir);
    }
}

void dhips_get_name_by_pid(pid_t pid, char *task_name)
{
    char proc_pid_path[BUFSIZ];
    char buf[BUFSIZ];
 
    sprintf(proc_pid_path, "/proc/%d/status", pid);
    FILE* fp = fopen(proc_pid_path, "r");
    if(NULL != fp){
        if( fgets(buf, BUFSIZ-1, fp)== NULL ){
            fclose(fp);
        }
        fclose(fp);
        sscanf(buf, "%*s %s", task_name);
    }
}

void dhips_send_email(const char *subject, const char *msg)
{
    char **admins;
    int n = pg_get_users_with_role(&admins, 0);
    char *email;
    
    cout << "Sending emails...";
    fflush(stdout);
    for (int i = 0; i < n; i++)
    {
        if (pg_get_email(admins[i], &email) == 0)
        {
            char cmd[BUFSIZ] = {0};
            if (SEND_EMAIL_COMPILED == 0)   // use python script
            {
                sprintf(cmd, "/usr/bin/python3 %s %s \"%s\" \"%s\"", SEND_EMAIL_PATH, email, subject, msg);
            } else                          // use binary file
            {
                sprintf(cmd, ".%s %s \"%s\" \"%s\"", SEND_EMAIL_PATH, email, subject, msg);
            }
            pid_t pid = fork();
            if (pid)
            {
                continue;
            } else if (pid == 0)
            {
                system(cmd);
                exit(0);
            }
        }
    }

    // wait for all the dhips-send-mail commands to finish
    for (int i = 0; i < n; i++)
    {
        int status;
        wait(&status);
    }

    cout << "done" << endl;
}