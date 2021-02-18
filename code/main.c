/***********************************************************************
name:
	readable -- recursively count readable files.
description:	
	See CS 360 Files and Directories lectures for details.
***********************************************************************/

/* Includes and definitions */
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <dirent.h>

/**********************************************************************
Take as input some path. Recursively count the files that are readable.
If a file is not readable, do not count it. Return the negative value
of any error codes you may get. However, consider what errors you want
to respond to.
**********************************************************************/

int isDirectory (char* path) {  // directory and user can read
    struct stat area, *s = &area;
    return ((lstat(path, s) == 0) && (s -> st_mode & S_IRUSR) && S_ISDIR(s -> st_mode));
}

int isReg (char* path) {  // regular and user can read
    struct stat area, *s = &area;
    return ((lstat(path, s) == 0) && (s -> st_mode & S_IRUSR) && S_ISREG(s -> st_mode));
}

int isSymLink (char* path) {  // symbolic link directory
    struct stat area, *s = &area;
    return (lstat(path, s) == 0 && S_ISLNK(s -> st_mode));
}

int isSpecialDir (char *name) {
    return (isDirectory(name) &&
            ((strcmp(name, ".") == 0 || strcmp(name, "..") == 0)));
}

int countFiles(char* path) {
    // if the path is given is only a single file and readable
    if (isReg(path) && (access(path, R_OK) == 0))
        return 1;

    if (isSymLink(path))  // ignore if it's a symbolic link
        return 0;

    char currentPath[240];
    char* nextPath;
    DIR *dir;
    struct dirent *dirEntry;
    int files = 0;
    int returnValue = 0;

    if (isDirectory(path)) {  // if the path is a directory

        dir = opendir(path);
        if (dir == NULL) {  // if cannot open the directory
            write(2, strerror(errno), strlen(strerror(errno)));
            return (-errno);
        }

        errno = 0;
        dirEntry = readdir(dir);
        if (dirEntry == NULL && errno != 0) {  // error when reading a directory
            write(2, strerror(errno), strlen(strerror(errno)));
            return (-errno);
        }

        while(dirEntry != NULL) {  // when not end of directory

            if (!isSymLink(dirEntry -> d_name)) {

                // if it's a directory
                if (isDirectory(dirEntry->d_name) && !isSpecialDir(dirEntry -> d_name)) {
                    if (chdir(dirEntry->d_name) < 0)
                        return 0;
                    if (getcwd(currentPath, sizeof(currentPath)) == NULL)
                        return 0;

                    // count file in the subdirectory and update total "files"
                    returnValue = countFiles(currentPath);
                    if (returnValue > 0)
                        files += returnValue;

                    chdir("..");  // change back to the previous path
                }

                // regular file and readable
                if (isReg(dirEntry->d_name) && (access(path, R_OK) == 0))
                    files += 1;
            }

            dirEntry = readdir(dir);
        }

        closedir(dir);
        return files;
    }
    return 0;
}

int readable(char* inputPath) {
    char buffer[240];  // path to pass to the recursive function

    if (inputPath == NULL) {  // if path is not provided
        if (getcwd(buffer, sizeof(buffer)) == NULL) {  // cannot set the current working directory
            write(2, strerror(errno), strlen(strerror(errno)));
            return (-errno);
        }
    }
    else {
        if (isSymLink(inputPath))
            return 0;

        // if the path is given is only a single file
        if (isReg(inputPath) && (access(inputPath, R_OK) == 0))
            return 1;

        // then, the path is a directory file
        if (chdir(inputPath) < 0) {  // if error occurs
            write(2, strerror(errno), strlen(strerror(errno)));
            return (-errno);
        }
        if (getcwd(buffer, sizeof(buffer)) == NULL) {  // cannot set "buffer"
            write(2, strerror(errno), strlen(strerror(errno)));
            return (-errno);
        }
    }
    return (countFiles(buffer));  // calling recursive function to return result
}

int main() {
//    char* path = NULL;
//    char* path = "../layer2/./";
    char* path = ".";
//    char* path = "assignment3.c";
    int result = readable(path);
    printf("total file = %d\n", result);
}
