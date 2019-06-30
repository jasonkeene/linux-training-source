/*************************************************************************\
*                  Copyright (C) Michael Kerrisk, 2019.                   *
*                                                                         *
* This program is free software. You may use, modify, and redistribute it *
* under the terms of the GNU General Public License as published by the   *
* Free Software Foundation, either version 3 or (at your option) any      *
* later version. This program is distributed without any warranty.  See   *
* the file COPYING.gpl-v3 for details.                                    *
\*************************************************************************/

/* t_get_seals.c
*/
#define _GNU_SOURCE         /* See feature_test_macros(7) */
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define errExit(msg)    do { perror(msg); exit(EXIT_FAILURE); \
                        } while (0)

int
main(int argc, char *argv[])
{
    unsigned int seals;
    int fd;

    if (argc != 2) {
        fprintf(stderr, "%s /proc/PID/fd/FD\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    fd = open(argv[1], O_RDWR);
    if (fd == -1)
        errExit("open");

    seals = fcntl(fd, F_GET_SEALS);
    if (seals == -1)
        errExit("fcntl");

    printf("Existing seals:");
    if (seals & F_SEAL_WRITE)
        printf(" WRITE");
    if (seals & F_SEAL_GROW)
        printf(" GROW");
    if (seals & F_SEAL_SHRINK)
        printf(" SHRINK");
    if (seals & F_SEAL_SEAL)
        printf(" SEAL");
    printf("\n");

    /* Code to map the file and access the contents of the
       resulting mapping omitted */

    exit(EXIT_SUCCESS);
}
