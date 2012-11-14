#include "stdafx.h"

void initReseau()
{
#ifdef WIN32
    WSADATA WSAData;
    WSAStartup(MAKEWORD(2,0), &WSAData);
#endif
}

void deInitReseau()
{
#ifdef WIN32
    WSACleanup();
#endif
}

MySocket creerSocketUdpClient (int * portBind)
{
    MySocket sock = 0;
    struct sockaddr_in exp_addr ;

    sock = socket (PF_INET, SOCK_DGRAM, 0) ;

    if(sock > 0)
    {
        memset ((char *) &exp_addr,0, sizeof exp_addr) ;
        exp_addr.sin_family = AF_INET ;
        exp_addr.sin_addr.s_addr = INADDR_ANY ;
        if(bind (sock, (struct sockaddr *)&exp_addr, sizeof exp_addr) == 0)
        {
            if(portBind != NULL)
            {
                int expLen = sizeof(exp_addr);
                getsockname(sock, (struct sockaddr *)&exp_addr, &expLen);
                *portBind = ntohs(exp_addr.sin_port);
            }

            return sock;
        }
        else
            return 0;
    }
    else
        return 0;
}

MySocket creerSocketUdpServer (int portEcoute)
{
    MySocket sock = 0;
    struct sockaddr_in exp_addr ;

    sock = socket (PF_INET, SOCK_DGRAM, 0) ;

    if(sock > 0)
    {
        memset ((char *) &exp_addr,0, sizeof exp_addr) ;
        exp_addr.sin_family = AF_INET ;
        exp_addr.sin_addr.s_addr = INADDR_ANY ;
        exp_addr.sin_port = htons (portEcoute) ;
        if(bind (sock, (struct sockaddr *)&exp_addr, sizeof exp_addr) == 0)
        {
            return sock;
        }
        else
            return 0;
    }
    else
        return 0;
}

int sendUdpMessageTo(MySocket socket , char * buffer , char * ipAddress , int port )
{
    struct sockaddr_in recv_addr;

    //bzero ((char *) &recv_addr, sizeof recv_addr) ;
    recv_addr.sin_family = AF_INET ;
    recv_addr.sin_addr.s_addr = inet_addr (ipAddress) ;
    recv_addr.sin_port = htons (port) ;

    int buffer_size = strlen(buffer)+1;

    return sendto (socket, buffer, buffer_size, 0, (struct sockaddr *)&recv_addr, sizeof recv_addr) ;
}

/*
int main (){


    	int sock;
   	char * buff = malloc(1000);
    	int n ;

	struct sockaddr_in exp_addr;

	int exp_lenth= sizeof(exp_addr);

    	bzero (&exp_addr, sizeof exp_addr) ;
    	sock = creerSocketUdpServer (PORT_LOCAL) ;

    	if(sock > 0)
    	{
    		printf("Création de la socket d'écoute sur le port %i réussie !\n",PORT_LOCAL);
    	}


    	bzero (buff, 80) ;

    	int nb_mess = 0;
	while (n = recvfrom (sock, buff, sizeof buff,0,(struct sockaddr *)&exp_addr,&exp_lenth))
    	{

    		char * ip = malloc(20);
    		sprintf ( ip, "%s", inet_ntoa (exp_addr.sin_addr.s_addr));
    		int  port = ntohs(exp_addr.sin_port);

    		printf("Message numéro %i recu : %s:%i => (%i)[%s]\n",++nb_mess,ip,port,n,buff);


    		fflush(stdout);

    		int ret = sendUdpMessageTo(sock , buff , sizeof(buff) , ip , sizeof(ip) , port );
    		if(ret == -1)
    		{
    			printf("Erreur lors de l'envoi du message ! (%i)\n",ret);
    		}

    		free(ip);
    		bzero (buff, 80) ;
    		fflush(stdout);

	}




    	close (sock) ;
    	return 0 ;
}
*/
