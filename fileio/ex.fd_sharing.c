/*************************************************************************\
*                  Copyright (C) Michael Kerrisk, 2019.                   *
*                                                                         *
* This program is free software. You may use, modify, and redistribute it *
* under the terms of the GNU General Public License as published by the   *
* Free Software Foundation, either version 3 or (at your option) any      *
* later version. This program is distributed without any warranty.  See   *
* the file COPYING.gpl-v3 for details.                                    *
\*************************************************************************/

/* fd_sharing.c

   Demonstrate that duplicate file descriptors share the file
   offset and file status flags by writing a program that:

   * Opens a file and duplicates the resulting file descriptor, to
     create a second file descriptor.
   * Displays the file offset and the state of the O_APPEND file
     status flag via the first file descriptor.
   * Changes the file offset and setting of the O_APPEND file status
     flag via the second file descriptor.
   * Displays the file offset and the state of the O_APPEND file
     status flag via the first file descriptor.
*/
#include <fcntl.h>
#include <stdio.h>
#include "tlpi_hdr.h"

static void
printFileDescriptionInfo(int fd)
{
    int flags;
    off_t offset;

    flags = fcntl(fd, F_GETFL);
    if (flags == -1) errExit("unable to read flags");

    offset = lseek(fd, 0, SEEK_CUR);
    if (offset == -1) errExit("unable to read offset");

    printf("offset: %ld O_APPEND: %d\n", offset, flags & O_APPEND);
}

int
main(int argc, char *argv[])
{
    int fd1, fd2, flags;
    off_t offset;

    if (argc < 2 || strcmp(argv[1], "--help") == 0)
        usageErr("%s pathname\n", argv[0]);

    fd1 = open(argv[1], O_RDWR);
    if (fd1 == -1) errExit("opening file %s", argv[1]);
    printFileDescriptionInfo(fd1);

    fd2 = dup(fd1);
    printFileDescriptionInfo(fd2);

    offset = lseek(fd1, 100000000, SEEK_SET);
    if (offset == -1) errExit("unable to read offset");
    printFileDescriptionInfo(fd1);
    printFileDescriptionInfo(fd2);

    flags = fcntl(fd1, F_GETFL);
    if (flags == -1) errExit("unable to read flags");
    flags = fcntl(fd1, F_SETFL, flags | O_APPEND);
    if (flags == -1) errExit("unable to write flags");
    printFileDescriptionInfo(fd1);
    printFileDescriptionInfo(fd2);

    exit(EXIT_SUCCESS);
}
