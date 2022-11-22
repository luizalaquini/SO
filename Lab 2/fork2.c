//
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
// For the syscall functions.
// For wait and related macros.
int main() {
   pid_t pid = fork(); // Fork a child process.

   if (pid < 0) { // Error occurred.
      fprintf(stderr, "Fork failed!\n");
      return 1;
   } else if (pid == 0) { // Child process.
      printf("[CHILD]: About to load command.\n");
      execlp("ls", "ls", "-l", (char*) NULL);  
      printf("[CHILD]: Great! It worked!\n"); /*se executar aqui é pq deu errado*/
   } else { // Parent process.
      printf("[PARENT]: Waiting on child.\n");
      wait(NULL);
      printf("[PARENT]: Child finished.\n");
   }
   return 0;
}
