#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>


int create_udp_client(const char *hostname, int port, struct sockaddr_in* sockname)
{
		
	struct hostent* host_address;
	int optval = 1;
	int socket_id;
	
	
	// obtention de l'adresse IP distante
	if(NULL == (host_address = gethostbyname(hostname))) {
		fprintf(stderr,"machine %s inconnue\n",hostname);
		exit(EXIT_FAILURE);
	}
	
	
	// création d'une socket
	if(-1 == (socket_id = socket(PF_INET, SOCK_DGRAM, 0))) {
		fprintf(stderr,"création socket impossible\n");
		exit(EXIT_FAILURE);
	}
	
	
	// changement paramètres pour réutilisation après fermeture
	setsockopt(socket_id, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(int));
	
	
	// affectation d'une adresse IP
	memset((char *)sockname, 0, sizeof(struct sockaddr_in));
	sockname->sin_family = AF_INET;
	sockname->sin_port = htons(port);
	sockname->sin_addr.s_addr = *((unsigned long *)(host_address->h_addr_list[0]));
	
	
	return socket_id;
}



int main(int argc, char **argv)
{

	int socket_id;
	char buffer[4096];
	int value = 11;
	struct sockaddr_in sockname;
	int sockaddr_in_length = sizeof(struct sockaddr_in);
	int r;
	socket_id = create_udp_client("localhost", 2000, &sockname);
	
	
	// génération du message à envoyer
	snprintf(buffer, sizeof(buffer), "%d", value);
	
	
	
	// envoi du message
	if(-1 == (r = sendto(socket_id, buffer, strlen(buffer), 0, (struct sockaddr *) &sockname, sockaddr_in_length))) {
		fprintf(stderr, "envoi au serveur impossible\n");
		shutdown(socket_id, 2);
		close(socket_id);
		exit(EXIT_FAILURE);
	}
	
	
	// attente du message de retour
	if(-1 == (r = recvfrom(socket_id, buffer, sizeof(buffer)-1, 0, (struct sockaddr*) &sockname, (socklen_t *)&sockaddr_in_length))) {
		fprintf(stderr, "problème de réception\n");
		shutdown(socket_id, 2);
		close(socket_id);
		exit(EXIT_FAILURE);
	}
	
	
	// affichage du résultat
	buffer[r+1] = '\0';
	printf("%d * %d = %ld\n", value, value, strtol(buffer, NULL, 10));
	shutdown(socket_id, 2);
	close(socket_id);
	
	exit(EXIT_SUCCESS);
	
}
