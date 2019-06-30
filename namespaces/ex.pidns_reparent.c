/*************************************************************************\
*                  Copyright (C) Michael Kerrisk, 2019.                   *
*                                                                         *
* This program is free software. You may use, modify, and redistribute it *
* under the terms of the GNU General Public License as published by the   *
* Free Software Foundation, either version 3 or (at your option) any      *
* later version. This program is distributed without any warranty.  See   *
* the file COPYING.gpl-v3 for details.                                    *
\*************************************************************************/

/* pidns_reparent.c

   Given the following scenario with PID namespaces:

    +---------------------------------------------+
    | Initial PID NS                              |
    |                           +---------------+ |
    |  +-+                      | Inner PID NS  | |
    |  |1|                      |               | |
    |  +-+                      |    +-+        | |
    |                           |    |1|        | |
    |                           |    +-+        | |
    |                           |               | |
    |  +-+   setns(), fork()    |    +-+        | |
    |  |M|----------------------+--> |P|        | |
    |  +-+                      |    +-+        | |
    |                           |     | fork()  | |
    |                           |     v         | |
    |                           |    +-+        | |
    |                           |    |Q|        | |
    |                           |    +-+        | |
    |                           +---------------+ |
    +---------------------------------------------+

    In the above, the small boxes are processes inside the PID namespaces,
    and "1" is the init process that becomes the parent of orphaned children
    in the corresponding PID namespace. (On systems running systemd, this
    process may be a PID other than 1 in the initial PID namespace.)

    This program can be used to answer the following questions:
    * When M terminates, which of the two init processes is P reparented to?
    * When P terminates, which of the two init processes is Q reparented to?

    Usage:

    * Start a shell in a new PID namespace; for example, using

        sudo unshare -pf bash --norc

    * Discover the PID of that shell (ps x -C bash | grep "norc")

    * Verify that that PID has no descendants using pstree(1):

        pstree <PID-of-shell>

    * Run this program (with 'sudo') specifying the /proc/PID/ns/pid link
      of the above shell process.

    * Discover the PIDs and parent PIDs of the three processes created
      by the preceding step:

        ps af -o "pid ppid comm" | grep pidns_rep

    * The three PIDs discovered in the previous step correspond
      to M, P, and Q.

    * Now kill M, and rerun the previous ps(1) command.
      Which processo is now the parent of P?

    * Now kill P, and rerun the previous ps(1) command.
      Which processo is now the parent of Q?
*/
#define _GNU_SOURCE
#include <fcntl.h>
#include <sched.h>
#include <unistd.h>
#include <stdlib.h>
#include <limits.h>
#include <stdio.h>
#include <sys/wait.h>
#include "tlpi_hdr.h"

static void
display_symlink(char *pname, char *link)
{
    char path[PATH_MAX];
    ssize_t s;

    s = readlink(link, path, PATH_MAX - 1);
    if (s == -1)
        errExit("readlink");

    path[s] = '\0';
    printf("%s%s ==> %s\n", pname, link, path);
}

int
main(int argc, char *argv[])
{
    pid_t child_pid, grandchild_pid;
    long fd;

    if (argc != 2) {
        fprintf(stderr, "Usage: %s /proc/PID/ns/pid\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    printf("Parent: PID is %ld\n", (long) getpid());
    display_symlink("Parent: ", "/proc/self/ns/pid");
    display_symlink("Parent: ", "/proc/self/ns/pid_for_children");

    /* Open the PID namespace file specified on the command line */

    fd = open(argv[1], O_RDONLY);
    if (fd == -1)
        errExit("open");

    /* FIXME: Do a setns() to the PID namespace specified in argv[1] and then
       redisplay the value of the 'pid_for_children' symlink */

    /* FIXME: Create a child process, which in turn creates a grandchild
       process. For purposes of verification, the child and grandchild
       should display the value of their /proc/PID/ns_pid_for_children
       symlink.  All three processes should call pause() to suspend
       execution indefinitely. */

}
