#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>


#define PORTTCP 2000

int create_tcp_server(int port, int nb_max_clients){

	int socket_id;
	int optval = 1;
	
	struct sockaddr_in sockname;
	
	//créatioin de la socket
	if(-1== (socket_id=socket(PF_INET, SOCK_STREAM, 0))){
		fprintf(stderr, "probleme creation socket\n");
		exit(EXIT_FAILURE);
	}
	
	//changement d'un parametre dans la socket pour pouvoir la reutiliser j uste apres sa fermeture
	setsockopt(socket_id, SOL_SOCKET, SO_REUSEADDR, &optval, szeof(int));
	
	//affectation d'une addresse
	memset((char*)&sockname, 0, sizeof(struct sockaddr_in));
	
	sockname.sin_family = AF_INET;
	sockname.sin_port = htons(port);
	sockname.sin_addr.s_addr = htonl(INADDR_ANY);
	
	if (-1 == (bind(socket_id, struct sockaddr*)&sockname), sizeof(struct sockaddr_in)){
		fprintf(stderr, "probleme avec bind\n");
		exit(EXIT_FAILURE);
	}
	
	//socket a l'ecoute
	if(-1==(listen(socket_id, nb_max_clients))){
		fprintf(stderr, "probleme avec listen\n");
		exit(EXIT_FAILURE);
	}	
	
	return(socket_id);
	
}

int main(int argc, char** argv){
	int socket_id;
	int client_id;
	char hello[] = "vous etes connecte\n";
	socket_id = create_tcp_server(PORTTCP, 1);
	if(-1 ==(client_id = accept(socket_id, NULL, 0))){
		fprintf(stderr, "Probleme avec accept\n");
		exit(EXIT_FAILURE);
	}
	shutdown(socket_id, 2);
	close(socket_id);
	
	write(client_id, hello, strlen(hello));
	shutdown(client_id, 2);
	close(client_id);
	
	exit(EXIT_SUCCESS);
	
}	


