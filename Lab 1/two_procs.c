#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
int glob = 6;
int main(void) {
	int var; 			/* external variable in initialized data */
	pid_t pid;			/* automatic variable on the stack */
	var = 88;
	int uid, pgid; /* modificações Luiza*/
	printf("before fork\n");

	if ( (pid = fork()) < 0){
		fprintf(stderr, "fork error\n");
		return 0;
	}
	else if (pid == 0) { 		/* ***child*** */
		glob++; 		/* modify variables */
		var++;
		pgid = setpgid(pid, pid); /* modificações Luiza*/
	}
	uid = getuid();  /* modificações Luiza*/
	pgid = getpgid(pid);  /* modificações Luiza*/
	sleep(15);		/* ***parent***; try to guarantee that child ends first*/
	printf("pid = %d, ppid = %d, glob = %d, var = %d, UID=%d, PGID=%d\n", getpid(), getppid(), glob, var, uid, pgid); /* modificações Luiza*/
	return 0;
}
