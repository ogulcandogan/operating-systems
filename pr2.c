#include  <stdio.h>
#include  <signal.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

void INThandler(int);               /* Ctrl-C handler           */

/* ---------------------------------------------------------------- */
/*                   main program starts here                       */
/* ---------------------------------------------------------------- */

void  main(void)
{

     signal(SIGINT, INThandler);        /* install Ctrl-C handler   */
     while (1)                          /* loop forever and wait    */
          pause();                      /* for Ctrl-C to come       */
}

/* ---------------------------------------------------------------- */
/* FUNCTION  INThandler:                                            */
/*    This function handles the SIGINT (Ctrl-C) signal.             */
/* ---------------------------------------------------------------- */

void  INThandler(int sig)
{
     pid_t pid;
     char  c;
     char *args[80 / 2 + 1];
     //args[0]="history";
     char *command="tail -10 $HOME/.bash_history";
     //args[1]="-al";
     signal(sig, SIG_IGN);              /* disable Ctrl-C           
     pid = fork();

        if (pid == 0)
        {

            execvp(args[0], args);
	    printf("0");
        }
        else if (pid > 0)
        {
	     printf("Output :\n ");
	     printf("0");
        }*/
        
     system(command);

     signal(SIGINT, INThandler);        /* install Ctrl-C handler   */

        
 }
 

