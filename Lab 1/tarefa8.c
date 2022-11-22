#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

int main ( int argc , char * argv []) {
    pid_t childpid = 0;
    int i , n ;

    /* check for valid number of command - line arguments */
    n = atoi ( argv [1]);

    for ( i = 1; i < n ; i ++)
        if (( childpid = fork () ) == -1)
            break ;
        printf("I am process %ld and my father is %ld\n", (long)getpid(), (long)getppid());
    return 0;
}