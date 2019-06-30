/*************************************************************************\
*                  Copyright (C) Michael Kerrisk, 2019.                   *
*                                                                         *
* This program is free software. You may use, modify, and redistribute it *
* under the terms of the GNU General Public License as published by the   *
* Free Software Foundation, either version 3 or (at your option) any      *
* later version. This program is distributed without any warranty.  See   *
* the file COPYING.gpl-v3 for details.                                    *
\*************************************************************************/

/* unsafe_malloc.c

    A program that can be used to demonstrate that calling malloc() and
    free() from both the main program and a signal handler is unsafe.

    Run this program, and then hold down the control-C key until you see
    the program crash with a message noting a corruption error from
    malloc() or free().
*/
#include <signal.h>
#include "tlpi_hdr.h"

#define SIG SIGINT

static void
doMalloc(int nmalloc)
{
    void *list[nmalloc];
    size_t bsize;

    /* Allocate multiple blocks of memory, in a range of sizes, and
       then free them */

    for (int j = 0; j < nmalloc; j++) {
        bsize = 1 << (4 + j % 10);
        list[j] = malloc(bsize);
    }

    for (int j = 0; j < nmalloc; j++)
        free(list[j]);
}

static void
handler(int sig)
{
    doMalloc(10);
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

    for (;;)
        doMalloc(10);
}
