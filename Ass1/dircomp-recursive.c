#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/syslimits.h>
#include <dirent.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

// Recursively search a path for files and directories
// base - the path to start from
// pathBuffer - a buffer with length PATH_MAX to store the new paths in
// files - a pointer to the int containing the number of files found
// dirs - a pointer to the int contiaining the number of dirs found
int search (char* base, char* pathBuffer, int* files, int* dirs);

int main (int argc, char* argv[]) {
    int dirsA = 0, dirsB = 0; // directory counter
    int filesA = 0, filesB = 0; // file counter
    char pathBufferA [PATH_MAX], pathBufferB [PATH_MAX]; // system-defined max path length
    if (argc != 3) { // if there is an incorrect number of args
        printf("  dircomp: compare direct children of a two directories\n");
        printf("  Usage:\tdircomp <path> <path>\n");
        return 1;
    }
    int rep = search(argv[1], pathBufferA, &filesA, &dirsA); // start recursive search at the first directory
    rep += search(argv[2], pathBufferB, &filesB, &dirsB); // start recursive search at the second directory
    if (rep != 0) { // at least one search was unsuccsessful
        return rep;
    } else {
        // output logic
        if (filesA < filesB) {
            printf("  %s has less files than %s\n", argv[1], argv[2]);
        } else if (filesA == filesB) {
            printf("  %s has the same number of files as %s\n", argv[1], argv[2]);
        } else {
            printf("  %s has more files than %s\n", argv[1], argv[2]);
        }
        if (dirsA < dirsB) {
            printf("  %s has less directories than %s\n", argv[1], argv[2]);
        } else if (dirsA == dirsB) {
            printf("  %s has the same number of directories as %s\n", argv[1], argv[2]);
        } else {
            printf("  %s has more directories than %s\n", argv[1], argv[2]);
        }
        return 0;
    }
}

int search (char* base, char* pathBuffer, int* files, int* dirs) {
    DIR *directory; // the base directory
    struct dirent *dirst; // a structure to place dir info
    struct stat file; // a structure to place file info
    int rep = 0;
    if (stat(base, &file) < 0) { // information about base path
        printf("%s\x1b[31m Error\x1b[0m: %s (%d)\n", base, errno == 2 ? "EACCES" : "", errno); // stat failed
        return 2;
    } else {
        if (S_ISDIR(file.st_mode)) { // if the base dir is a directory
            directory = opendir (base); // open the directory
            if (directory != NULL) {
                while ((dirst = readdir (directory))) { // while there are still items in the dir
                    if (strcmp(dirst->d_name, ".") == 0 || strcmp(dirst->d_name, "..") == 0) { // avoid looping
                        continue;
                    }
                    strcpy(pathBuffer, base); // form the new path
                    strcat(pathBuffer, "/");
                    strcat(pathBuffer, dirst->d_name);
                    if (stat(pathBuffer, &file) < 0) { // gather info about the item
                        printf("%s\x1b[31m Error\x1b[0m: %s (%d)\n", pathBuffer, errno == 2 ? "EACCES" : "", errno);
                        rep = 2;
                    } else {
                        if (S_ISREG(file.st_mode)) { // item is a file
                            (*files)++; // increment file counter
                        } else if (S_ISDIR(file.st_mode)) { // item is a directory
                            (*dirs)++; // increment dir counter
                            char branchBase[PATH_MAX];
                            char branchBuffer[PATH_MAX]; // create new search buffer
                            strcpy(branchBase, pathBuffer); // clone the current path
                            rep = search(branchBase, branchBuffer, files, dirs); // search the new dir
                        }
                    }
                }
                closedir(directory); // close the directory
                return rep;
            } else {
                return 1;
            }
        } else {
            printf("%s\x1b[31m Error\x1b[0m: not a directory\n", base);
            return 3;
        }
    }
}
