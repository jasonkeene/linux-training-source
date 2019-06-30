/*************************************************************************\
*                  Copyright (C) Michael Kerrisk, 2019.                   *
*                                                                         *
* This program is free software. You may use, modify, and redistribute it *
* under the terms of the GNU General Public License as published by the   *
* Free Software Foundation, either version 3 or (at your option) any      *
* later version. This program is distributed without any warranty.  See   *
* the file COPYING.gpl-v3 for details.                                    *
\*************************************************************************/

/* demo_file_seals.c
*/
#define _GNU_SOURCE         /* See feature_test_macros(7) */
#include <fcntl.h>
#include <sys/mman.h>
#include "tlpi_hdr.h"

int
main(int argc, char *argv[])
{
    char *name, *str;
    ssize_t len;
    int fd;

    if (argc < 3)
        usageErr("%s name length string\n", argv[0]);

    name = argv[1];
    len = atoi(argv[2]);
    str = argv[3];

    /* Create an anonymous file in tmpfs; allow seals to be
       placed on the file */

    fd = memfd_create(name, MFD_ALLOW_SEALING);
    if (fd == -1)
        errExit("memfd_create");

    /* Size the file as specified on the command line */

    if (ftruncate(fd, len) == -1)
        errExit("truncate");

    /* Don't allow the file to grow */

    if (fcntl(fd, F_ADD_SEALS, F_SEAL_GROW) == -1)
        errExit("fcntl");

    /* Write string specified on command line to file.  This
       will fail if we write a string greater than the length
       given to the ftruncate() call above. */

    if (write(fd, str, strlen(str)) == -1)
        errExit("write");

    exit(EXIT_SUCCESS);
}
