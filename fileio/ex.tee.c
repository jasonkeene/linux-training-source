/*************************************************************************\
*                  Copyright (C) Michael Kerrisk, 2019.                   *
*                                                                         *
* This program is free software. You may use, modify, and redistribute it *
* under the terms of the GNU General Public License as published by the   *
* Free Software Foundation, either version 3 or (at your option) any      *
* later version. This program is distributed without any warranty.  See   *
* the file COPYING.gpl-v3 for details.                                    *
\*************************************************************************/

/* tee.c

   tee [-a] <pathname>

   Duplicate standard input to standard output and also to the
   file named in the command-line argument. If 'pathname'
   does not exist, it is created.

   By default, 'pathname' is overwritten if it exists; use the '-a'
   option to append to an existing file.
*/
#include <sys/stat.h>
#include <fcntl.h>
#include "tlpi_hdr.h"

int
main(int argc, char *argv[])
{
    int doAppend;       /* Was the '-a' command-line option specified? */
    int errFnd;         /* Did getopt() find a bad option? */
    char *pathname;     /* Pathname supplied as command-line argument */
#define BUF_SIZE 4096
    char buf[BUF_SIZE]; /* Buffer for use by read() and write() calls */
    int opt;

    int inputFd, outputFd, openFlags;
    mode_t filePerms;
    ssize_t numRead;

    errFnd = 0;
    doAppend = 0;
    while ((opt = getopt(argc, argv, "a")) != -1) {
        switch (opt) {
        case 'a':
            doAppend = 1;
            break;
        default:
            errFnd = 1;
            break;
        }
    }

    /* 'optind' is a global variable that is updated by getopt().
       Upon completion of option processing, it contains the index of
       the next word in the command line following the options and
       option arguments. */

    if (errFnd || argc != optind + 1)
        usageErr("%s [-a] <pathname>\n", argv[0]);

    pathname = argv[optind];

    openFlags = O_CREAT | O_WRONLY;
    filePerms = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH;

    if (doAppend) {
        openFlags = openFlags | O_APPEND;
    } else {
        openFlags = openFlags | O_TRUNC;
    }

    outputFd = open(pathname, openFlags, filePerms);
    if (outputFd == -1)
        errExit("opening file %s", pathname);

    while ((numRead = read(STDIN_FILENO, buf, BUF_SIZE)) > 0) {
        if (write(outputFd, buf, numRead) != numRead) {
            fatal("couldn't write whole buffer");
        }
        if (write(STDOUT_FILENO, buf, numRead) != numRead) {
            fatal("couldn't write whole buffer");
        }
    }

    exit(EXIT_SUCCESS);
}
