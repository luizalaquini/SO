#include <stdio.h>
#include <unistd.h>     // For the syscall functions.
#include <sys/wait.h>   // For wait and related macros.

int main() {
    for(int i=0; i<3; i++){
        pid_t pid = fork(); // Fork a child process.

        if (pid < 0) { // Error occurred.
            fprintf(stderr, "Fork failed!\n");
            return 1;
        } 
        else if (pid == 0) { // Child process.
            sleep(2);
        } 
        else { // Parent process.

            while(1){
                sleep(2);
                int wstatus;
                int result = waitpid(-1, &wstatus, WNOHANG);

                if(result > 0){ // child finished
                    printf("Meu filho pid=%d terminou\n", result);
                }
                else if(result == 0){ // All childrens alive!
                    printf("Nenhum filho terminou\n");
                }
                else{ // All childrens finished!
                    printf("Todos os filhos terminaram\n");
                    break;
                }
            }
        }
    }
    

    return 0;
}