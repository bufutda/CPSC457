#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/syslimits.h>
#include <dirent.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
int main (int argc, char* argv[]) {
    DIR *directory; // the base directory
    struct dirent *dirst; // a structure to place dir info
    struct stat file; // a structure to place file info
    int dirs = 0; // directory counter
    int files = 0; // file counter
    char pathBuffer [PATH_MAX]; // system-defined max path length
    if (argc != 2) { // if there is an incorrect number of args
        printf("  dirstats: count direct children of a directory\n");
        printf("  Usage:\tdirstat <path>\n");
        return 1;
    }
    directory = opendir (argv[1]); // open the directory
    if (directory != NULL) {
        while ((dirst = readdir (directory))) { // while there are still items in the dir
            strcpy(pathBuffer, argv[1]); // form the new path
            strcat(pathBuffer, "/");
            strcat(pathBuffer, dirst->d_name);
            if (stat(pathBuffer, &file) < 0) { // gather info about the item
                printf("\x1b[31mError:\x1b[0m: Could not stat file: %s (%d)\n", pathBuffer, errno);
            } else {
                if (S_ISREG(file.st_mode)) { // item is a file
                    files++;
                } else if (S_ISDIR(file.st_mode)) { // item is a directory
                    dirs++;
                }
            }
        }
        closedir(directory); // close the directory
        printf("  %d directories and %d files\n", dirs, files);
        return 0;
    } else {
        printf("\x1b[31mFatal\x1b[0m: Could not access the directory\n");
        return 1;
    }
}
