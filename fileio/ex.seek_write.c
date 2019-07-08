/*************************************************************************\
*                  Copyright (C) Michael Kerrisk, 2019.                   *
*                                                                         *
* This program is free software. You may use, modify, and redistribute it *
* under the terms of the GNU General Public License as published by the   *
* Free Software Foundation, either version 3 or (at your option) any      *
* later version. This program is distributed without any warranty.  See   *
* the file COPYING.gpl-v3 for details.                                    *
\*************************************************************************/

/* seek_write.c

   Demonstrate the use of lseek() and file I/O system calls.

   Usage: seek_write pathname offset string
*/
#include <sys/stat.h>
#include <fcntl.h>
#include "tlpi_hdr.h"

int main(int argc, char *argv[])
{
    char *pathname, *string;
    off_t offset, newOffset;
    size_t stringLen;
    ssize_t wrote;
    int fileFd;

    if (argc != 4 || strcmp(argv[1], "--help") == 0)
        usageErr("%s pathname offset string\n", argv[0]);

    pathname = argv[1];
    // TODO: check errno for strtoll
    offset = strtoll(argv[2], NULL, 0);
    string = argv[3];

    fileFd = open(
        pathname,
        O_CREAT | O_WRONLY,
        S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH
    );
    if (fileFd == -1) errExit("opening file %s", pathname);

    newOffset = lseek(fileFd, offset, SEEK_SET);
    if (newOffset == -1) errExit("seeking in file %s", pathname);
    if (newOffset != offset) {
        fatal("offset != newOffset");
    }

    stringLen = strlen(string);
    wrote = write(fileFd, string, stringLen);
    if (wrote == -1) errExit("writing to file %s", pathname);
    if (wrote != stringLen) {
        fatal("wrote != stringLen");
    }

    exit(EXIT_SUCCESS);
}
