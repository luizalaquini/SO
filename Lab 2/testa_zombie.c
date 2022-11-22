/* rodar em background */
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int main() 
{
   int pid ;
   printf("Eu sou o processo pai, PID =  %d, e eu vou criar um filho.\n",getpid()) ;
   pid = fork() ;
   if(pid == -1) /* erro */
  {
      perror("E impossivel criar um filho") ;
      exit(-1) ;
  }
  else if(pid == 0) /* filho */
  {
     printf("Eu sou o filho, PID = %d. Estou vivo mas vou dormir um pouco. Use o comando ps -lt para conferir o meu estado e o do meu pai. Daqui a pouco eu acordo.\n",getpid()) ;
     sleep(10) ;
     printf("Sou eu de novo, o filho. Acordei mas vou terminar agora. Use ps -lt novamente.\n") ;
     exit(0) ;
  }
  else /* pai */
  {
     printf("Bem, agora eu vou bloquear e esperar pelo término do meu filho.\n") ;
     //wait(NULL);  /* pai esperando pelo término do filho */
     printf("Pronto... meu filho terminou... agora vou terminar também! Tchau!\n") ;
     for(;;) ; /* pai bloqueado em loop infinito */

  }
}



