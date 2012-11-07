#include "stdafx.h"


int main()
{

    initReseau();
    time_t LastSend = 0; // Heure du last send
    MySRand(time(0)); // Init du random
    double temperature = 0; // Stocke la température
    int sleepTime = (CONFIG_TEMPS_MESURE/5 * 1000 ) ;
    char * buffer = malloc(80);
    char * Heure = malloc(80);

    MySocket SocketRouteur = creerSocketUdpClient();

	if( SocketRouteur > 0)
	{
	    char ip[] = "127.0.0.1";
	    int port = CONFIG_PORT_SERVER;


		printf("Routeur : %s:%i -- \n",ip,port);
		printf("Socket N :%i -- \n",SocketRouteur);


	    while(1)
	    {
		int NextSend = (int)MyRand(-1 * CONFIG_TEMPS_RAND,CONFIG_TEMPS_RAND) + CONFIG_TEMPS_MESURE + LastSend;
		if(NextSend <= time(NULL))
		{

		    Heure = getStrTime(Heure);
		    LastSend = time(NULL);
		    temperature = mesure_temperature();
		    sprintf(buffer,"T,%f,E",temperature);


		    printf("%s Envoi d'une mesure  : %f ->",Heure,temperature);

		    int ret = sendUdpMessageTo(SocketRouteur , buffer , ip , port );
		    if(ret == -1)
		    {
		        printf(" Erreur lors de l'envoi du message ! (%i)\n",ret);
		    }
		    else
		    {
		    	printf(" OK \n",ret);
		    }
		fflush(stdout);

		}
		MySleep(sleepTime);
	    }
	}

    free(buffer);
    deInitReseau();
    return 0;
}
