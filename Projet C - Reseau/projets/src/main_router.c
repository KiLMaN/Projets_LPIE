#include "stdafx.h"

int main()
{
    initReseau();

    MySocket SocketEcoute;
   	char buff[100];
    int n ;

	struct sockaddr_in exp_addr;
    memset (&exp_addr,0, sizeof exp_addr) ;
	int exp_lenth= sizeof(exp_addr);


    SocketEcoute = creerSocketUdpServer (CONFIG_PORT_ECOUTE) ;

    if(SocketEcoute > 0)
    {
        printf("Creation de la socket d'écoute sur le port %i reussie !\n",CONFIG_PORT_ECOUTE);
    }
    else
    {
        printf("Erreur lors de la creation de la socket ! %i",SocketEcoute);
        return -1;
    }

    while (1)
    {
        //memset (&buff,0, sizeof(buff)) ;
        //buff[sizeof(buff)-1] = '\0';
        //memset (&buff,0, strlen(buff)) ;
        n = recvfrom (SocketEcoute, buff, sizeof(buff),0,(struct sockaddr *)&exp_addr,(socklen_t *)&exp_lenth);
        int err = 0; //WSAGetLastError();
        char * ip = ( char * ) malloc(sizeof(char) * 20);
        sprintf ( ip, "%s", inet_ntoa (exp_addr.sin_addr));
        int port = ntohs(exp_addr.sin_port);


        printf("Message recu : %s:%i => (%i)[%s] %i\n",ip,port,n,buff,err);
        //printf("valeur : %f\n",atof(buff));
        fflush(stdout);

        /*int ret = sendUdpMessageTo(sock , buff , sizeof(buff) , ip , sizeof(ip) , port );
        if(ret == -1)
        {
            printf("Erreur lors de l'envoi du message ! (%i)\n",ret);
        }*/

        free(ip);
        //bzero (buff, 80) ;
        //fflush(stdout);

    }

    deInitReseau();
    return 0;
}

