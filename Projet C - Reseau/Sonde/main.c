#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>



void * waitForClient(void * ctx)
{
    while(1)
    {
        printf("\t\t[LOOOL]\n");
        fflush(stdout);
    }

    return NULL;
}

int main()
{
    pthread_t threadWait;
    int retThread = pthread_create(&threadWait, NULL, waitForClient, NULL);

    if(retThread == 0)
    {
    	printf("\t\t[REUSSI]\n");
    	/* On stoke l'identifiant du thread dans le tableau */
    	//tThreadList[counterThreadClients] = &thread;
    	//counterThreadClients++;
    }

    system("PAUSE");
    return 0;
}

