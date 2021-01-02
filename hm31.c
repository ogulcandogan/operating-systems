/*
 * Created by OGULCAN DOGAN on 9/29/20.
 * Solution to question 3.1
 *
 * Answer is 5 as the child and parent processes
 * each have their own copy of value.
 * In other words
 * The result is still 5 as the child updates its copy of value.
 * When control returns to the parent, the value gonna remain at 5.
 */

#include <stdio.h>
#include <unistd.h>
#include<sys/wait.h>

int value = 5;

int main()
{
    pid_t pid;

    pid = fork();

    if (pid == 0) { /* child process */
        value += 15;
        return 0;
    }
    else if (pid > 0) { /* parent process */
        wait(NULL);
        printf ("PARENT: value = %d\n",value);
        return 0;
    }
}