#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <netdb.h>

#define PORT "8081"
#define HOST "127.0.0.1"

int main (int argc, char *argv []){
 	struct addrinfo *res ;
    	//int sock ;
    	/* On indique l'addresse et le port du serveur */
    	char* port = PORT;
    	char* host = HOST; 
    	/* Declaration du N° de socket */    
    	int s_cli ;
    	char buf [80] ;
    	
    	/* Reupere les informations sur le serveur à joindre */
    	struct addrinfo hints = { 0, AF_UNSPEC, SOCK_STREAM, 0, 0, NULL, NULL, NULL };
    	getaddrinfo (host, port, &hints, &res);
    	
    	/* Créer la socket */
    	s_cli = socket (res->ai_family, res->ai_socktype, res->ai_protocol);
    	printf("Connection : %i \n",s_cli);
    	/* Se connecte */
    	int connectState = connect (s_cli, res->ai_addr, res->ai_addrlen) ;
    	printf("connectState : %i ",connectState);
    	/* Libere les infos serveur */
    	freeaddrinfo (res);
    	
    	/* Envoi d'un message */
    	bzero (buf, 80) ;
    	write (s_cli, "Coucou, ici client", 19) ;
	/* Attente du retour du serveur */
	read (s_cli, buf, 80) ; 
    	printf ("J'ai lu %s\n", buf) ;
    	
    	/* Fermeture socket */
    	close (s_cli) ;
    	return (0) ;
}
