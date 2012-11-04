#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>


int verifAdresseIP(char * addIp)
{
	if(addIp == NULL)
		return 0;
		
	char * tmp = malloc(sizeof(addIp));
	strcpy(tmp,addIp);
		
	int count = 0;
	int bInRange = 1;
	
	char * pch;
	pch = strtok (tmp,".");
	while (pch != NULL)
	{
		count++;
   		if( atoi(pch) >= 256 || atoi(pch) < 0)
   			bInRange = 0;
   	 	
   	 	pch = strtok (NULL, ".");
  	}

	if(count == 4)
	{
		if(bInRange == 1)
			return 1;
	}
	free(tmp);
	return 0;
}


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
		//exp_addr.sin_port = htons (8083) ;
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

	char * buff = malloc(1000);
	
	char adresseSaisie[20];
	int port = 0;
	int i = 0;
	int sock ;
	


	/** Saisie d'une adresse par les digits */
	
	/*for (i = 0 ; i < 4 ; i++)
	{
		char tmp[4];
		printf("Saisir le %i chiffre de l'adresse du serveur : ",i+1);
		scanf("%s",tmp);

		
		strcat(adresseSaisie , tmp);
		if(i < 3)
			strcat(adresseSaisie,".");
		
	}*/

	/** Saisie d'une adresse en une seule ligne */
	char tmp[20];
	printf("Saisir l'adresse du serveur : ");
	scanf("%s",tmp);
	strcat(adresseSaisie , tmp);

	

	printf("Saisir le numéro de port du serveur: ");
	scanf("%i",&port);
	
	printf("Adresse : %s:%i\n",adresseSaisie,port);
	
	if(port <= 0 || port >= 65535)
	{
		printf("Erreur dans le port : %i ; le port doit être compris entre 0 et 65535 !\n\n",port);
		return;
	}
	
	if( !verifAdresseIP(adresseSaisie))
	{
		printf("L'adresse saisie n'est pas correcte !\n\n");
		return;
	}
	
	printf("Adresse : %s:%i\n",adresseSaisie,port);
	
	struct sockaddr_in exp_addr;
	int exp_lenth= sizeof(exp_addr);
	bzero (&exp_addr, sizeof exp_addr) ;
	
	sock = creerSocketUdpClient();
	if(sock > 0)
	{
		printf("Creation de la socket réussie\n");
	}

    	
	
	while (1)
	{
		printf("Message : ");
		scanf("%s",buff);
	
	
		int ret = sendUdpMessageTo(sock , buff , sizeof(buff) , adresseSaisie , sizeof(adresseSaisie) , port );
	
	
    		memset(buff,0, sizeof(buff)) ;
    	
		int n  = recvfrom (sock, buff, sizeof(buff),0,(struct sockaddr *)&exp_addr,&exp_lenth);
    	
    		char * ip = malloc(sizeof(inet_ntoa (exp_addr.sin_addr.s_addr)));
    		sprintf ( ip, "%s", inet_ntoa (exp_addr.sin_addr.s_addr));
    		
    		int  port = ntohs(exp_addr.sin_port);

    		printf("Message recu : %s:%i => (%i)(%i)[%s]\n",ip,port,n,strlen(buff),buff);
   

    		free(ip);
    			
    		fflush(stdout);
    		
	}
	
    	close (sock) ;
    	return 0 ;
}



