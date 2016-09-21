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
    int dirs = 0; // directory counter
    int files = 0; // file counter
    char pathBuffer [PATH_MAX]; // system-defined max path length
    if (argc != 2) { // if there is an incorrect number of args
        printf("  dirstats: count direct children of a directory\n");
        printf("  Usage:\tdirstat <path>\n");
        return 1;
    }
    int rep = search(argv[1], pathBuffer, &files, &dirs); // start recursive search at the given path
    if (rep != 0) { // search was unsuccsessful
        return rep;
    } else {
        printf("  %d directories and %d files\n", dirs, files);
        return 0;
    }
}

int search (char* base, char* pathBuffer, int* files, int* dirs) {
    DIR *directory; // the base directory
    struct dirent *dirst; // a structure to place dir info
    struct stat file; // a structure to place file info
    int rep = 0;
    if (stat(base, &file) < 0) {
        printf("%s\x1b[31m Error\x1b[0m: %s (%d)\n", pathBuffer, errno == 2 ? "EACCES" : "", errno);
        return 2;
    } else {
        if (S_ISDIR(file.st_mode)) {
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
