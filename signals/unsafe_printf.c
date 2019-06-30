/*************************************************************************\
*                  Copyright (C) Michael Kerrisk, 2019.                   *
*                                                                         *
* This program is free software. You may use, modify, and redistribute it *
* under the terms of the GNU General Public License as published by the   *
* Free Software Foundation, either version 3 or (at your option) any      *
* later version. This program is distributed without any warranty.  See   *
* the file COPYING.gpl-v3 for details.                                    *
\*************************************************************************/

/* unsafe_printf.c

    A program that can be used to demonstrate that calling printf() from
    both the main program and a signal handler is unsafe.

    Run this program as follows, and then hold down the control-C key
    immediately after starting the program:

        ./unsafe_printf > out.txt

    After a while, it is likely that the program will hang (most likely
    because of a deadlock inside the stdio library), and in out.txt there will
    be strings other than "mmmmm\n" and "sssss\n". After killing the program
    with control-\, the latter point can be checked using the command:

        egrep -n -v '^(mmmmm|sssss)$' < out.txt
*/
#include <signal.h>
#include "tlpi_hdr.h"

#define SIG SIGINT

static void
handler(int sig)
{
    printf("sssss\n");
}

int
main(int argc, char *argv[])
{
    struct sigaction sa;

    sa.sa_handler = handler;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);
    if (sigaction(SIG, &sa, NULL) == -1)
        errExit("sigaction");

    pause();    /* Wait until user first presses control-C */

    for (;;)
        printf("mmmmm\n");
}
