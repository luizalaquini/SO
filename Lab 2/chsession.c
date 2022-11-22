#include <stdio.h>
#include <unistd.h>     // For the syscall functions.
#include <sys/wait.h>   // For wait and related macros.


// For the syscall functions.
int main() {
	pid_t ch_pid, shell_pid, father_pid;
	char word[256];
	shell_pid = getppid();
	father_pid = getpid();
	if ( (ch_pid = fork()) < 0)
		fprintf(stderr, "fork error\n");
	else if (ch_pid == 0) { 		/* ***child*** */
		ch_pid = getpid();
		printf("Hi, I'm CHILD process... here are some IDs:\n");
		printf("\t My PID=%ld \tMy father's PID=%ld \tShell's PID=%ld\n", (long int) ch_pid, (long int) father_pid, (long int) shell_pid );
		printf("\t My GID=%d \tMy father's GID=%d \tShell's GID=%d\n", getpgid(ch_pid), getpgid(father_pid), getpgid(shell_pid));
		printf("\t My SID=%d \tMy father's SID=%d \tShell's SID=%d\n", getsid(ch_pid), getsid(father_pid), getsid(shell_pid));
		sleep(1);
		printf("\nNote that my father and I are both in a Foreground group... accordingly, we receive any SIGNAL from the Controlling Terminal. For ex: if you do \"Ctrl-c\" we both recive a signal SIGINT and die! Try it or press [ENTER]...\n");
		fgets(word, sizeof(word), stdin);
		printf("\nAs you didn't want to kill me, I'm going use setsid() to move out from my current session.\n");
		sleep(2);
		setsid();
		printf("... Ok so far... let's check the IDs again:\n");
		printf("\t My PID=%ld \t\tMy father's PID=%ld \tShell's PID=%ld\n", (long int) ch_pid, (long int) father_pid, (long int) shell_pid );
		printf("\t My new GID=%d \tMy father's GID=%d \tShell's GID=%d\n", getpgid(ch_pid), getpgid(father_pid), getpgid(shell_pid));
		printf("\t My new SID=%d \tMy father's SID=%d \tShell's SID=%d\n", getsid(ch_pid), getsid(father_pid), getsid(shell_pid));

		sleep(3);

		printf("\nNow try to do \"Ctrl-c\" again... you'll kill only my father...\n");
		printf("After that you can find me typing \"ps lax\"\n");

		sleep(200);
	}
	else {
		sleep(500);		/* ***parent***; sleeps forever...*/
	}

	return 0;
}


