/*************************************************************************\
*                  Copyright (C) Michael Kerrisk, 2019.                   *
*                                                                         *
* This program is free software. You may use, modify, and redistribute it *
* under the terms of the GNU General Public License as published by the   *
* Free Software Foundation, either version 3 or (at your option) any      *
* later version. This program is distributed without any warranty.  See   *
* the file COPYING.gpl-v3 for details.                                    *
\*************************************************************************/

/* zombie.c

   Demonstrate the creation of zombie child process(es).

   Usage: zombie [nzombies]

   If 'nzombies' is omitted, the default is 1.
*/
#include <sys/wait.h>
#include "tlpi_hdr.h"

int
main(int argc, char *argv[])
{
    int nzombies = (argc > 1) ? atoi(argv[1]) : 1;

    setbuf(stdout, NULL);       /* Disable buffering of stdout */

    printf("Parent (PID %ld)\n", (long) getpid());

    for (int j = 0; j < nzombies; j++) {
        switch (fork()) {
        case -1:
            errExit("fork-%d", j);

        case 0:         /* Child: exits to become zombie */
            printf("Child  (PID %ld) exiting\n", (long) getpid());
            exit(EXIT_SUCCESS);

        default:        /* Parent continues in loop */
            break;
        }
    }
    sleep(600);                 /* Children are zombies during this time */
    while (wait(NULL) > 0)      /* Reap zombie children */
        continue;

    exit(EXIT_SUCCESS);
}
