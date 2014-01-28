#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>

#define PORTTCP 2000

int create_tcp_client(const char* hostname, int port){
	struct hostent* host_address;
	struct sockaddr_in sockname;
	int optval;
	int socket_id;
	
	//recupere l'adresse IP de la machine a laquelle on souhaite se connecter
	
	if(NULL==(host_address = gethostbyname(hostname))){
		fprintf(stderr, "machine %s inconnue\n", hostname);
		return(EXIT_FAILURE);
	}
	
	//creation de la socket
	if(-1==(socket_id=socket(PF_INET, SOCK_STREAM, 0))){
		fprintf(stderr, "creation de socket impossible\n");
		return(EXIT_FAILURE);
	}
	
	//changement d'un parametre de la socket pour la reutiliser juste apres sa fermeture
	optval=1;
	setsockopt(socket_id, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(int));
	
	//connexion de la socket au serveur TCP distant
	sockname.sin_family = host_address->h->addrtype;
	sockname.sin_port = htons(port);
	memcpy((char*)&(sockname.sin_addr.s_addr), host_address->h_addr, host_address->h_length);
	
	if(-1==(connect(socket_id, (struct sockaddr*)&sockname, sizeof(struct sockaddr_in)))){
		fprintf(stderr, "connexion impossible au serveur %s\n", hostname);
		return(EXIT_FAILURE);
	}
	
	return(socket_id);
}

int main(int argc, char** argv){
	int socket_id;
	char c;
	
	socket_id = create_tcp_client("localhost", TCPPORT);
	if(-1==socket_id)
		exit(EXIT_FAILURE);
		
	while(1==read(socket_id, &c, sizeof(char)))
		printf("%c",c);
	
	shutdown(socket_id, 2);
	close(socket_id);
	exit(EXIT_SUCCESS);
}
