#include <stdio.h>
#include <unistd.h>     // For the syscall functions.
#include <sys/wait.h>   // For wait and related macros.


// For the syscall functions.
int main() {
	pid_t pid = fork(); // Fork a child process.
	if (pid < 0) { // Error occurred.
		fprintf(stderr, "Fork failed!\n");
		return 1;
	} else if (pid == 0) { // Child process.
		printf("[CHILD]: PID: %ld - PPID: %ld ... I'm going to sleep now...\n", (long int) getpid(), (long int) getppid());
		sleep(2);
		printf("\n[CHILD]: PID: %ld - new-PPID: %ld ... I'm up again... but who is my new father?!?\n\n", (long int) getpid(), (long int) getppid());
	} else { // Parent process.
		printf("[PARENT]: PID: %ld - PPID: %ld ... Oh, I've lived for too long... good bye folks!\n", (long int) getpid(), (long int) getppid());
	}
	return 0;
}
