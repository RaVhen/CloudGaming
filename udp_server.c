#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>


int create_udp_server(int port)
{
	int socket_id;
	struct sockaddr_in sockname;
	int optval;
	
	
	// création d'une socket
	if (-1 == (socket_id = socket(PF_INET, SOCK_DGRAM, 0))) {
		fprintf(stderr, "création de socket impossible\n");
		exit(EXIT_FAILURE);
	}
	
	// chargement d'un paramètre de la socket pour la réutiliser après sa fermeture
	optval = 1;
	setsockopt(socket_id,SOL_SOCKET,SO_REUSEADDR,&optval,sizeof(int));
	
	
	// affectation d'une adresse IP
	memset((char *) &sockname, 0, sizeof(struct sockaddr_in));
	sockname.sin_family = AF_INET;
	sockname.sin_port = htons(port); // ????
	sockname.sin_addr.s_addr = (htonl(INADDR_ANY));
	// sockname.sin_addr.s_addr = (inet_addr("127.0.0.1"));		// localhost
	
	if(0 > bind(socket_id, (struct sockaddr *) &sockname, sizeof(struct sockaddr_in))) {
		fprintf(stderr, "problème avec bind\n");
		exit(EXIT_FAILURE);	
	}


	return socket_id;

}


int main(int argc, char **argv)
{
	
	int socket_id;
	char buffer[4096];
	int sockaddr_in_length = sizeof(struct sockaddr_in);
	struct sockaddr_in sockname;
	int r;
	int value;



	socket_id = create_udp_server(2000);
	
	// attente d'un message
	if(-1 == (r = recvfrom(socket_id, buffer, sizeof(buffer) - 1, 0, (struct sockaddr*) &sockname, (socklen_t *)&sockaddr_in_length))) {
		
		fprintf(stderr,"problème avec recvfrom\n");
		exit(EXIT_FAILURE);
	}
	
	
	// génération du message à renvoyer
	buffer[r+1] = '\0';
	value = strtol(buffer, NULL, 10);
	snprintf(buffer, sizeof(buffer), "%d", value* value);
	
	
	// envoi du message
	r = sendto(socket_id, buffer, strlen(buffer), 0, (struct sockaddr*) &sockname, sockaddr_in_length);
	shutdown(socket_id, 2);
	close(socket_id);
	exit(EXIT_SUCCESS);	
		
			
}	 

