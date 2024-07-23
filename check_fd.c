#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <libproc.h>

#include "parser.h"

int check_fd(char *str) 
{
    pid_t pid = getpid();
    int bufferSize = proc_pidinfo(pid, PROC_PIDLISTFDS, 0, NULL, 0);
    
    if (bufferSize == -1) {
        perror("proc_pidinfo");
        return 1;
    }
    
    struct proc_fdinfo *fds = malloc(bufferSize);
    if (fds == NULL) {
        perror("malloc");
        return 1;
    }
    
    bufferSize = proc_pidinfo(pid, PROC_PIDLISTFDS, 0, fds, bufferSize);
    if (bufferSize == -1) {
        perror("proc_pidinfo");
        free(fds);
        return 1;
    }
    
    int numberOfFDs = bufferSize / PROC_PIDLISTFD_SIZE;
    
    fprintf(stderr, "%s(%5d): ", str, pid);
    for (int i = 0; i < numberOfFDs; i++) {
        fprintf(stderr, "%d ", fds[i].proc_fd);
    }
	fprintf(stderr, "\n");
    
    free(fds);
    return 0;
}