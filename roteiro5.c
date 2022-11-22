#include <stdio.h>
#include <unistd.h>     // For the syscall functions.
#include <sys/wait.h>   // For wait and related macros.

pid_t pidA = -1; 
pid_t pidB = -1; 

int qtdPidA = 3; 
int qtdPidB = 3; 

void sigInt(int sig){
    printf("[%ld]-PING\n", (long int)getpid());
}

void sigPong(int sig){
    printf("[%ld]-PONG\n", (long int)getpid());
}

void sigParent(int sig){
    if(sig == SIGUSR1)
        kill(pidB, SIGUSR1);
    else if(sig == SIGUSR2)
        kill(pidA, SIGUSR1);
}

int main() {
    pidA = fork(); // Create child A

    if(pidA > 0){ // Parent
        pidB = fork(); // Create child B
    }

    if(pidA == 0){ // Child A
        struct sigaction act;

        act.sa_handler = &sigInt;
        act.sa_flags = 0;
        
        sigaction(SIGUSR1, &act, NULL);

        printf("Sou o filho A com PID %ld\n", (long int)getpid());
        while(1){
            pause();
            sleep(1);
            kill(getppid(), SIGUSR1);
            if(qtdPidA == 0)
                break;
            qtdPidA--;
        }
    }

    if(pidB == 0){ // Child B
        struct sigaction act2;

        act2.sa_handler = &sigPong;
        act2.sa_flags = 0;

        sigaction(SIGUSR1, &act2, NULL);

        printf("Sou o filho B com PID %ld\n", (long int)getpid());
        while(1){
            pause();
            sleep(1);
            kill(getppid(),SIGUSR2);
            if(qtdPidB == 0)
                break;
            qtdPidB--;
        }
    }
    
    if(pidA > 0 && pidB > 0){
        struct sigaction act_parent;

        act_parent.sa_handler = &sigParent;
        act_parent.sa_flags = 0;

        sigaction(SIGUSR2, &act_parent, NULL);
        sigaction(SIGUSR1, &act_parent, NULL);

        printf("Sou o pai com PID %ld\n", (long int)getpid());

        sleep(1);

        kill(pidA, SIGUSR1);

        while(1){
            int status;
            int result =  waitpid(-1, &status, WNOHANG);
            
            if(result == -1){
                printf("Não tem Filhos\n");
                break;
            }
            else if(result > 0){
                if(result == pidA){
                    kill(pidB, SIGKILL);
                    printf("O filho com PID %d morreu\n", pidA);
                    break;
                }
                if(result == pidB){
                    kill(pidA, SIGKILL);
                    printf("O filho com PID %d morreu\n", pidB);
                    break;
                }
            }
        }
    }
    
    return 0;
}