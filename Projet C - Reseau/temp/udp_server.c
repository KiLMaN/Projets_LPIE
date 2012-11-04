#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>


#define HOST_LOCAL 	INADDR_ANY
#define PORT_LOCAL	8082


int creerSocketUdpClient ()
{
	int sock = -1;
	struct sockaddr_in exp_addr ;
	
	sock = socket (PF_INET, SOCK_DGRAM, 0) ;
	
	if(sock > 0)
	{
		bzero ((char *) &exp_addr, sizeof exp_addr) ;
		exp_addr.sin_family = AF_INET ;
		exp_addr.sin_addr.s_addr = INADDR_ANY ;
		if(bind (sock, (struct sockaddr *)&exp_addr, sizeof exp_addr) == 0)
		{
			return sock;
		}
		else
			return -1;
	}
	else 
		return -1;
}
 
int creerSocketUdpServer (int portEcoute)
{
	int sock = -1;
	struct sockaddr_in exp_addr ;
	
	sock = socket (PF_INET, SOCK_DGRAM, 0) ;
	
	if(sock > 0)
	{
		bzero ((char *) &exp_addr, sizeof exp_addr) ;
		exp_addr.sin_family = AF_INET ;
		exp_addr.sin_addr.s_addr = INADDR_ANY ;
		exp_addr.sin_port = htons (portEcoute) ;
		if(bind (sock, (struct sockaddr *)&exp_addr, sizeof exp_addr) == 0)
		{
			return sock;
		}
		else
			return -1;
	}
	else 
		return -1;
}

int sendUdpMessageTo(int socket , char * buffer , int buffer_size , char * ipAddress , int ipAddress_size , int port )
{
	struct sockaddr_in recv_addr;
	
	bzero ((char *) &recv_addr, sizeof recv_addr) ;
	recv_addr.sin_family = AF_INET ;
	recv_addr.sin_addr.s_addr = inet_addr (ipAddress) ;
	recv_addr.sin_port = htons (port) ;
	
	
	return sendto (socket, buffer, buffer_size, 0, (struct sockaddr *)&recv_addr, sizeof recv_addr) ;
}


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
