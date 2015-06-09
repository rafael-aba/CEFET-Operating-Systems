#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
 
int main (int argc, char *argv[], char *envp[])
{
   int retval ;
 
   printf ("Ola, sou o processo %5d\n", getpid()) ;
 
   retval = fork () ;
 
   printf ("[retval: %5d] sou %5d, filho de %5d\n",
           retval, getpid(), getppid()) ;
 
   if ( retval < 0 )
   {
      perror ("Erro: ") ;
      exit (1) ;
   }
   else 
      if ( retval > 0 )
         wait (0) ;
      else
      {
         execve ("/bin/date", argv, envp) ;
         perror ("Erro") ;
      }
 
   printf ("Tchau de %5d!\n", getpid()) ;
 
   exit (0) ;
}
