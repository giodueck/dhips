#include "m_vii.h"

// Dud functions
void ModuleVII::stop(){}

ModuleVII::ModuleVII(){}

ModuleVII::DetectorVII::PreventerVII::PreventerVII()
{
    this->module = "VII";
}

ModuleVII::DetectorVII::DetectorVII()
{
    this->module = "VII";
    susExtensions = std::vector<std::string>();
    preventer = PreventerVII();
}

int ModuleVII::DetectorVII::setup()
{
    int i = 0;
    char *dest;

    // get all targeted file extension names
    do
    {
        // system files
        i = pg_get_targeted_ext_name(i, &dest);
        if (i > 0)
        {
            susExtensions.push_back(std::string(dest));
        }
    } while (i > 0);

    return 0;
}

int ModuleVII::DetectorVII::scan()
{
    // Codigo reciclado del proyecto de shell para SO1
    // Cumple la funcion del comando ls, identifica todos los archivos en un directorio,
    //  en este caso /tmp
    DIR *d;
    struct dirent *dir;

    d = opendir("/tmp");
    if (d)
    {
        int n = 0;
        int size = 10;
        char **dirs = (char **)malloc(sizeof(char*) * size);
        while ((dir = readdir(d)) != NULL)
        {
            dirs[n] = dir->d_name;
            n++;
            if (size <= n)
            {
                size += 10;
                dirs = (char **)realloc(dirs, sizeof(char*) * size);
            }
        }

        // check through files, delete if sus
        char aux[NAME_MAX], *tok, *auxtok;
        for (int i = 0; i < n; i++)
        {
            strcpy(aux, dirs[i]);
            tok = strtok(aux, ".");
            
            // loop through all '.'s in the filename
            while (tok)
            {
                auxtok = tok;
                tok = strtok(NULL, ".");

                // check if sus
                for (int j = 0; j < susExtensions.size(); j++)
                {
                    if (strcmp(auxtok, susExtensions[j].c_str()) == 0)
                    {
                        std::string auxs;
                        // create alarm
                        auxs = std::string("Name: /tmp/") + std::string(dirs[i]) + std::string(" [Action taken]");
                        log(ALARM_VII_SUSFILE_FOUND, "localhost", auxs.c_str());

                        // delete file
                        auxs = std::string("/tmp/") + std::string(dirs[i]);
                        preventer.setTargetName(auxs.c_str());
                        preventer.act(2);
                        tok = NULL; // to break out of the while early
                    }
                }
            }
        }

        free(dirs);
        closedir(d);
    } else
    {
        char msg[128];
        sprintf(msg, "could not open %s", filename);
        dhips_perror(msg);
        return -1;
    }

    return 0;
}

void ModuleVII::DetectorVII::PreventerVII::setTarget(pid_t pid)
{
    target = pid;
}

void ModuleVII::DetectorVII::PreventerVII::setTargetName(const char *name)
{
    targetName = name;
}

int ModuleVII::DetectorVII::PreventerVII::act(int action)
{
    if (action == 1 && target)
    {
        // forcibly kill the process with pid = target
        kill(target, SIGKILL);

        // log the action
        char msg[1024];
        sprintf(msg, "Killed process \"%s\" with PID %d", targetName, target);
        log(msg);
        target = 0;
    } else if (action == 2)
    {
        // remove file
        remove(targetName);

        // log the action
        char msg[1024];
        sprintf(msg, "Removed suspiscious file %s", targetName);
        log(msg);
        target = 0;
    }

    return 0;
}

void ModuleVII::setup()
{
    active = true;
    detector.setup();
}

void ModuleVII::run()
{
    if (!active) return;
    detector.scan();
}