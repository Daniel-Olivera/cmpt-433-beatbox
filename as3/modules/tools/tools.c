// Utility functions such as sleep and runCommand

#include "tools.h"
#include <stdio.h>
#include <dirent.h>

void sleepForMs (long long delayInMs)
{
    const long long NS_PER_MS = 1000000;
    const long long NS_PER_SECOND = 1000000000;

    long long delayNs = delayInMs * NS_PER_MS;
    int seconds = delayNs / NS_PER_SECOND;
    int nanoseconds = delayNs % NS_PER_SECOND;

    struct timespec reqDelay = {seconds, nanoseconds};
    nanosleep(&reqDelay, (struct timespec *) NULL);
}

void runCommand(char* command)
{
    FILE *pipe = popen(command, "r");

    char buffer[1024];
    while (!feof(pipe) && !ferror(pipe)) {
        if (fgets(buffer, sizeof(buffer), pipe) == NULL)
            break;
    }

    int exitCode = WEXITSTATUS(pclose(pipe));
    if (exitCode != 0){
        perror("Unable to execute command: ");
        printf("    %s\n", command);
        printf("    exit code: %d\n", exitCode);
    }
}

void exportGpioPin(char value[])
{
    // Try opening the directory for the desired pin
    char fileName[256]; 
    char gpioDirPath[] = "/sys/class/gpio/gpio%s";
    sprintf(fileName, gpioDirPath, value);
    DIR* dir = opendir(fileName);

    if(!dir){ // pin not exported yet
    char command[256];
    char commandTemplate[] = "echo %s > /sys/class/gpio/export";
    sprintf(command, commandTemplate, value);
        runCommand(command);
    }
    else{
        closedir(dir);
    }
}
