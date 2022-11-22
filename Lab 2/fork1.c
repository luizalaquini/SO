#include <stdio.h>
#include <unistd.h>     // For the syscall functions.
#include <sys/wait.h>   // For wait and related macros.

int main() {
    pid_t pid = fork(); // Fork a child process.

    if (pid < 0) { // Error occurred.
        fprintf(stderr, "Fork failed!\n");
        return 1;
    } else if (pid == 0) { // Child process.
        printf("PID = %ld\n", (long int) getpid());
        sleep(15);
        //printf("[CHILD]: I'm finished.");
        //return 42;
    } else { // Parent process.
        printf("[PARENT]: Waiting on child.\n");
        int wstatus;
        //wait(&wstatus);
        
        /*if (WIFEXITED(wstatus)) {
            printf("[PARENT]: Child returned with code %d.\n",
                    WEXITSTATUS(wstatus));
        }*/
        
        /*if (WIFSIGNALED(wstatus)){  
            printf("[PARENT]: Child ended by signal. Signal number = %d.\n)", 
                    WTERMSIG(wstatus));
        }*/ // TEM ALGO ERRADO ACONTECENDO SE DESCOMENTAR ESSE IF

        if (waitpid(pid, &wstatus, WUNTRACED) != -1){
            printf("[PARENT]: Child suspended. Signal number = %d.\n)", 
                    WSTOPSIG(wstatus));
        }
    }
    return 0;
}
