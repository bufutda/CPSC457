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
// name - the directory or file
// type - the file type to look for
int execFind (char* cmd, char* name, char* type, int* suc);

int main (int argc, char* argv[]) {
    int status; // the status of the find command
    char cmdBuffer[PATH_MAX + 32]; // enough space for the find command + a max path
    int suc = 0; // if there were results
    if (argc != 2) { // if there is an incorrect number of args
        printf("  fod: check if a string is a file or a directory\n");
        printf("  Usage:\tfod <filename>\n");
    }
    sprintf(cmdBuffer, "find . -name \"%s\" -type d", argv[1]); // create the command string
    status = execFind(cmdBuffer, argv[1], "directory", &suc); // run the find command
    if (status == 0) { // find ran ok
        sprintf(cmdBuffer, "find . -name \"%s\" -type f", argv[1]); // create the command string
        status = execFind(cmdBuffer, argv[1], "file", &suc); // run the find command
    }
    if (suc == 0) { // there were no results
        printf("  %s is not a file or a directory in the current directory tree\n", argv[1]);
    }
    return status; // exit with fail code if find exited with fail code
}

int execFind (char* cmd, char* name, char* type, int* suc) {
    FILE* procStream; // stdout/stderr of the process
    char path[PATH_MAX]; // enough space for one line of output
    int status; // the status of the process
    procStream = popen(cmd, "r"); // run the process
    if (procStream == NULL) { // if the process failed to execute
        printf("\x1b[31mFatal\x1b[0m: Cannot spawn find process (%d)\n", errno);
        return 1;
    }
    while (fgets(path, PATH_MAX, procStream) != NULL) { // read line by line from stream
        (*suc) = 1;
        printf("  found %s: %s", type, path);
    }
    status = pclose(procStream);
    if (status != 0) { // if the stream can't close
        printf("\x1b[31mFatal\x1b[0m: Could not close stream (%d)\n", status);
        return 1;
    }
    return 0;
}
