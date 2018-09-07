#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>


int main () {

	//creation de socket
	int net_socket;

	net_socket = socket(AF_INET, SOCK_STREAM, 0);

	// adresse du net_socket
	struct sockaddr_in adresseClient;
	adresseClient.sin_family = AF_INET;
	adresseClient.sin_port = htons(5000);
	adresseClient.sin_addr.s_addr = INADDR_ANY;

	int connexion = connect(net_socket, (struct sockaddr *) &adresseClient, sizeof(adresseClient));
	// check des erreurs
	if (connexion == -1){
			printf("il y a eu une erreur lors de la connexion au serveur");
	}

	//recevoir les données du serveur
	char reponseDuServeur[256];
	char aTraduire[256];
	char messagePourLeServeur[256];

	//afficher la réponse du adresseServeur
	printf("%s\n", reponseDuServeur);

	while (1) {
			//afficher la réponse du adresseServeur


			gets(aTraduire);
			sprintf(messagePourLeServeur, "%s", aTraduire);
			send(net_socket, messagePourLeServeur, sizeof(messagePourLeServeur), 0);

			recv(net_socket, &reponseDuServeur, sizeof(reponseDuServeur), 0);
			printf("%s\n", reponseDuServeur);
  }

	//fermer le socket
	close(net_socket);

	return EXIT_SUCCESS;
}
