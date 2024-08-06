#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <libproc.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

int is_pipe(int fd) {
    struct stat statbuf;
    if (fstat(fd, &statbuf) == -1) {
        perror("fstat");
        return -1;
    }

    // 파일 타입이 FIFO(파이프)인지 확인
    if (S_ISFIFO(statbuf.st_mode)) {
        return 1;  // 파이프
    } else {
        return 0;  // 파이프 아님
    }
}

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
    
    fprintf(stderr, "%s(%5d):\n", str, pid);
    for (int i = 0; i < numberOfFDs; i++) 
    {
        if (i == 2)
            continue;
        fprintf(stderr, "%d : ", fds[i].proc_fd);
        if (is_pipe(fds[i].proc_fd))
        {
            fprintf(stderr, "is pipe\n");
        }
        else
        {
            char path[1024];
            if (fcntl(fds[i].proc_fd, F_GETPATH, path) != -1) 
            {
                fprintf(stderr, "File path: %s\n", path);
            }
        }
    }
	fprintf(stderr, "\n");
    
    free(fds);
    return 0;
}