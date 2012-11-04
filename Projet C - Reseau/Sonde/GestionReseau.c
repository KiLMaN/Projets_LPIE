#include "stdafx.h"



int counterThreadClients = 0;
pthread_t * tThreadList[NB_MAX_CONN];

void * waitForClient(void * ctx);
void * handleClient(void * ctx);

MySocket * cree_serveur_tcp (int port,MySocket * sock);
void * sendMessageAllClients(void * ctx);

pthread_t threadWait;

//int * pSocket;

int mainTest()
{
    #ifdef WIN32
        WSADATA WSAData;
        WSAStartup(MAKEWORD(2,0), &WSAData);
    #endif
    MySocket * pMainSock;
    pMainSock = (MySocket*)malloc(sizeof(MySocket));
    pMainSock = cree_serveur_tcp(CONFG_PORT,pMainSock);
     // On crée un thread


    // Permet d'exécuter le fonction waitForClient en parallèle
    int retThread = pthread_create(&threadWait, NULL, waitForClient, pMainSock);

    if(retThread == 0)
    {
        printf("Création du thread d'attente :\t\t[REUSSI]\n");
    }
    else
    {
        printf("Création du thread d'attente :\t\t[ECHOUE] PTHREAD ERROR :%i\n",retThread);
    }
    while (1)
    {


    }

    #ifdef WIN32
       WSACleanup();
    #endif
    return 0;
}

void * waitForClient(void * ctx)
{
   if(ctx == NULL)
	{
		printf("-- -- Erreur de contexte dans la fonction [waitForClient] ! -- --\n");
		return NULL;
	}
	// Récuperer la socket principale //
    MySocket * pSocketPrincipale = (MySocket *)ctx;
    // Affichage d'attente //
    printf("Attente sur : %i !\n",*pSocketPrincipale);
    fflush(stdout);


	while(1)
	{
		// Prépare les strucutures clientes //
		struct sockaddr_in cli_addr;
      	int cli_len = sizeof cli_addr;

		// Attendre l'arrivée d'un client ///
		MySocket * pSocketClient = malloc(sizeof(int));
		*pSocketClient = accept (*pSocketPrincipale, (struct sockaddr *) &cli_addr, &cli_len) ;

		printf("Nouveau Client \n");
		fflush(stdout);


	  	// S'il reste un thread de disponnible //
	  	if(counterThreadClients < NB_MAX_CONN)
	  	{
	  	    printf("Création du thread client :");
            // On crée un thread
            pthread_t threadClient;

            // Permet d'exécuter le fonction maFonction en parallèle
            int retThread = pthread_create(&threadClient, NULL, handleClient, pSocketClient);

            if(retThread == 0)
            {
                printf("\t\t[REUSSI]\n");
                // On stoke l'identifiant du thread dans le tableau //
                tThreadList[counterThreadClients] = &threadClient;
                counterThreadClients++;
            }
            else
            {
                printf("\t\t[ECHOUE] PTHREAD ERROR :%i\n",retThread);
                closesocket(*pSocketClient);
                close(*pSocketClient);
                //return;
            }
        }
        else
        {
            printf("\t\t[ECHOUE] MAX_CLIENT_LIMIT\n");
            closesocket(*pSocketClient);
            close(*pSocketClient);
            //return;
        }
	  	fflush(stdout);
    }
}
void * handleClient(void * ctx)
{
	MySocket * SocketClient = (MySocket *)ctx;
	char buff[100];
    int n;


	while ( (n = recv (*SocketClient, buff, sizeof(buff),0) )) { // Bloque le thread jusqu'a la reception d'un message
        	printf ("J'ai lu %s et je le lui repete\n", buff) ;
        	//send (*SocketClient, buff, n,0) ;
        	pthread_t threadClient;
            //int retThread =
            pthread_create(&threadClient, NULL, sendMessageAllClients, SocketClient);
        	fflush(stdout);
	}

	closesocket (*SocketClient);
	return NULL;
}
void * sendMessageAllClients(void * ctx)
{
    return NULL;
}
MySocket * cree_serveur_tcp (int port,MySocket * sock)
{
    	MySocket * s_ecoute = sock;
    	struct sockaddr_in serv_addr ;

   	 // on designe l'adresse+port qu'on va ouvrir //
    	serv_addr.sin_addr.s_addr = INADDR_ANY; // on attend sur toutes nos adresses //
    	serv_addr.sin_port = htons (port) ;
    	serv_addr.sin_family = AF_INET;
    	memset (&serv_addr.sin_zero, 0, sizeof(serv_addr.sin_zero));

   	 // on cree la socket d'ecoute et on l'associe au couple (adresse,port) defini ci-dessus //
    	*s_ecoute = socket (PF_INET, SOCK_STREAM, 0) ;
    	if( *s_ecoute != 0)
    	{
    		printf("Creation de la socket \t\t\t[REUSSI]\n");
    	}
    	else
    	{
    		printf("Creation de la socket \t\t\t[ERREUR] socket : %i \n",*s_ecoute);
    	}
    	int ret = bind (*s_ecoute, (struct sockaddr *)&serv_addr, sizeof serv_addr);
    	if(ret == 0)
    	{
            printf("Ouverture du port : %i \t\t\t[REUSSI]\n",port);

    	}
    	else
        {
               		printf("Ouverture du port : %i \t\t\t[ERREUR] %i\n",port,ret);
        }

        if( listen (*s_ecoute, 0) == 0)//on definit la socket s_ecoute, comme etant une socket d'ecoute
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


