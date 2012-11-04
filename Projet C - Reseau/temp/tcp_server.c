#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#include <pthread.h>

#define PORT 		8081
#define HOST 		INADDR_ANY
#define NB_MAX_CONN 	5


int counterThreadClients = 0;
pthread_t * tThreadList[NB_MAX_CONN];

void * waitForClient(void * ctx);
void * handleClient(void * ctx);


int main (){
     int * pSocket = malloc(sizeof (int));
     *pSocket = cree_serveur_tcp(PORT,NB_MAX_CONN);
       
    // On crée un thread
    
    pthread_t threadWait;
    printf("Création du thread d'attente :");
    // Permet d'exécuter le fonction waitForClient en parallèle
    int retThread = pthread_create(&threadWait, NULL, waitForClient, (void *)pSocket);
    			
    if(retThread == 0)
    {
    	printf("\t\t[REUSSI]\n");
    	/* On stoke l'identifiant du thread dans le tableau */
    	//tThreadList[counterThreadClients] = &thread;
    	//counterThreadClients++;
    }
    else
    {
    	printf("\t\t[ECHOUE] PTHREAD ERROR :%i\n",retThread);
    }
    while (1);
    return 0;
    			
}

void * waitForClient(void * ctx)
{
	if(ctx == NULL)
	{
		printf("-- -- Erreur de contexte dans la fonction [waitForClient] ! -- --\n");
		return NULL;
	}
	
	while(1)
	{
		/* Prépare les strucutures clientes */
		struct sockaddr_in cli_addr;
      	int cli_len = sizeof cli_addr;
      
		/** Récuperer la socket principale **/
		int * pSocketPrincipale = (int *)ctx;
	
		/* Affichage d'attente */
		printf("Attente d'un client sur la socket numéro : %i !\n",*pSocketPrincipale);
		fflush(stdout);
	
	
		/** Attendre l'arrivée d'un client **/
		int * pSocketClient;
		*pSocketClient = accept (*pSocketPrincipale, (struct sockaddr *) &cli_addr, (socklen_t *) &cli_len) ; 
	
		printf("Nouveau client arrivé ! \n\tCréation d'un nouveau thread pour le traitement de celui-ci !\n");
		fflush(stdout);
	
	  	printf("Création du thread client :");
	  	/* S'il reste un thread de disponnible **/
	  	if(counterThreadClients < NB_MAX_CONN)
	  	{
	  		// On crée un thread
    			pthread_t threadClient;
    
    			// Permet d'exécuter le fonction maFonction en parallèle
    			int retThread = pthread_create(&threadClient, NULL, handleClient, pSocketClient);
    			
    			if(retThread == 0)
    			{
    				printf("\t\t[REUSSI]\n");
    				/* On stoke l'identifiant du thread dans le tableau */
    				tThreadList[counterThreadClients] = &threadClient;
    				counterThreadClients++;
    			}
    			else
    			{
    				printf("\t\t[ECHOUE] PTHREAD ERROR :%i\n",retThread);
    				return;
    			}
    			
    			
	  	}
	  	else
	  	{
	  		printf("\t\t[ECHOUE] MAX_CLIENT_LIMIT\n");
	  		return;
	  	}
	  	fflush(stdout);	
    	}
}
void * handleClient(void * ctx)
{
	int * SocketClient = ctx;
	char * buff[100];

	
	while ( (n = read (SocketClient, buff, sizeof(buff)) )) { // Bloque le thread jusqu'a la reception d'un message
        	printf ("J'ai lu %s et je le lui repete\n", buff) ;
        	//write (s_dial, buf, n) ;
        	pthread_t threadClient;
		int retThread = pthread_create(&threadClient, NULL, sendMessageAllClients, pSocketClient);
        	fflush(stdout);
	}
	
	close (SocketClient);
}


void * sendMessageAllClients(void * ctx)
{

}


int cree_serveur_tcp (int port, int nb_max_clients) {
    	int s_ecoute ;
    	struct sockaddr_in serv_addr ;
    
   	 /* on designe l'adresse+port qu'on va ouvrir */
    	serv_addr.sin_addr.s_addr = htonl (HOST) ; /* on attend sur toutes nos adresses */
    	serv_addr.sin_port = htons (port) ; 
    	memset (&serv_addr.sin_zero, 0, sizeof(serv_addr.sin_zero));
    
   	 /* on cree la socket d'ecoute et on l'associe au couple (adresse,port) defini ci-dessus */
    	s_ecoute = socket (PF_INET, SOCK_STREAM, 0) ;
    	if( s_ecoute != 0)
    	{
    		printf("Creation de la socket \t\t\t[REUSSI]\n");
    	}
    	else
    	{
    		printf("Creation de la socket \t\t\t[ERREUR] socket : %i \n",s_ecoute);
    	}
    	
    	fflush(stdout);
    	if(bind (s_ecoute, (struct sockaddr *)&serv_addr, sizeof serv_addr) == 0)
    	{
    		printf("Ouverture du port : %i \t\t[REUSSI]\n",port);
    	}
    	else 
   	{
   		printf("Ouverture du port : %i \t\t[ERREUR]\n",port);
   	}
   	   fflush(stdout);
   	if( listen (s_ecoute, nb_max_clients) == 0)/* on definit la socket s_ecoute, comme etant une socket d'ecoute*/
   	{
   		printf("Passage de la socket en ecoute \t\t[REUSSI]\n");
   	}	 
   	else
   	{
   		printf("Passage de la socket en ecoute \t\t[ERREUR]\n");
   	}
       fflush(stdout);
	return s_ecoute ;
}
