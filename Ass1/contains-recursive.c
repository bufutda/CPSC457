#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/syslimits.h>
#include <dirent.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

// run the find command and parse it's output
// cmd - the command string
// first - the first dir
// second - the second dir
int execFind (char* cmd, char* first, char* second);

int main (int argc, char* argv[]) {
    int status; // the status of the find command
    char cmdBuffer[2 * PATH_MAX + 32]; // enough space for the find command + 2 max paths
    if (argc != 3) { // if there is an incorrect number of args
        printf("  contains: test if a directory contains another directory\n");
        printf("  Usage:\tcontains <dir> <dir>\n");
        return 1;
    }
    sprintf(cmdBuffer, "find \"%s\" -name \"%s\" -type d", argv[1], argv[2]); // create the command string
    status = execFind(cmdBuffer, argv[1], argv[2]); // run the find command
    if (status == 0) { // find ran ok
        sprintf(cmdBuffer, "find \"%s\" -name \"%s\" -type d", argv[2], argv[1]); // create the command string
        status = execFind(cmdBuffer, argv[2], argv[1]); // run the find command
    }
    return status; // exit with fail code if find exited with fail code
}

int execFind (char* cmd, char* first, char* second) {
    FILE* procStream; // stdout/stderr of the process
    char path[PATH_MAX]; // enough space for one line of output
    int status; // the status of the process
    struct stat file; // structure for status information
    if (stat(first, &file) < 0) { // get info about the base path
        return 2; // stat failed
    } else {
        if (S_ISDIR(file.st_mode)) { // if the base path is a directory
            procStream = popen(cmd, "r"); // run the process
            if (procStream == NULL) { // if the process failed to execute
                printf("\x1b[31mFatal\x1b[0m: Cannot spawn find process (%d)\n", errno);
                return 1;
            }
            while (fgets(path, PATH_MAX, procStream) != NULL) { // read line by line from stream
                printf("  %s contains %s: %s", first, second, path);
            }
            status = pclose(procStream);
            if (status != 0) { // if the stream can't close
                printf("\x1b[31mFatal\x1b[0m: Could not close stream (%d)\n", status);
                return 1;
            }
            return 0;
        } else {
            return 0;
        }
    }
}
