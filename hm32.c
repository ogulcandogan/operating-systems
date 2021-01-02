
/**
 * Created by OGULCAN DOGAN on 9/29/20.
 * Solution to exercise 3.2
 *
 * The answer is easily determined by counting
 * the number of unique processes which are output
 * by the call to getpid() - which is 8 unique processes.
 */

#include <stdio.h>
#include <unistd.h>

int main()
{
    printf("%d\n",getpid());
    fork();
    printf("%d\n",getpid());


    fork();
    printf("%d\n",getpid());

    fork();
    printf("%d\n",getpid());

    return 0;
}
