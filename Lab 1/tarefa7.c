#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

int main (void){
    int n = 3;
    

    for (int i=0; i<n; i++){
        pid_t pid = fork();
        if(pid == 0){
            printf("I am process %ld and my father is %ld\n", (long)getpid(), (long)getppid());
            break;
        }
    }
    sleep(2);
    return 0;
}