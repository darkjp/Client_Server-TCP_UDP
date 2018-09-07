#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#define LG_MESSAGE 256
#define TAILLE 3

char* getEN(char * motFR);
char* getFR(char * motEN);
char* fr[] = {"bleu","vert","rouge"};
char* en[] = {"blue","green","red"};
char** strsplit(const char* str, const char* delim, size_t* numtokens);

int main (int argc, char *argv[]) {
 char bufferW[LG_MESSAGE];
  char messageDuServeur[256];
  char messageDuClient[256];
  //Socket
  int serv_socket, clientSocket;
  //dico
  char** requete;
  size_t nbItems;



  // création du socket

  serv_socket = socket(AF_INET, SOCK_STREAM, 0);
  if(serv_socket < 0) {
      perror("socket");
      exit(1);
  }
  printf("Socket creee avec succes ! (%d)\n", serv_socket);

  // adresse du serveur
  struct sockaddr_in adresseServeur;
  adresseServeur.sin_family = AF_INET;
  adresseServeur.sin_port = htons(5000);
  adresseServeur.sin_addr.s_addr = INADDR_ANY;

  // bind du socket
  if(bind(serv_socket, (struct sockaddr*) &adresseServeur, sizeof(adresseServeur)) == -1) {
		perror("bind");
		exit(2);
	}
	printf("Socket attachee avec succes !\n");
	// La taille de la file d’attente est fixee a 5
	if(listen(serv_socket, 5) < 0) {
		perror("listen");
		exit(3);
	}

  clientSocket = accept(serv_socket, NULL, NULL);


  // envoie du message


  while(1){

    recv(clientSocket, &messageDuClient, sizeof(messageDuClient),0);
memset(bufferW, 0x00, LG_MESSAGE*sizeof(char));
sprintf(bufferW,messageDuClient);
int i;
for(i=0;i<=strlen(bufferW);i++)
if(bufferW[i]>=65&&bufferW[i]<=92)
bufferW[i]=bufferW[i]+32;
	sprintf(messageDuClient,bufferW);

memset(bufferW, 0x00, LG_MESSAGE*sizeof(char));
strncpy(bufferW, messageDuClient, 2);
if(strcmp(bufferW,"en")!=0&&strcmp(bufferW,"fr")!=0&&strcmp(bufferW,"li")!=0)
strcpy(messageDuServeur,"Error: vous avez entré un message incorrect");
 else
{
    // récupération du mot et traitement
    requete = strsplit(messageDuClient, " ", &nbItems);

    printf("%s\n", requete[1]);
    // renvoie de la traduction

    // DICO !!

          if(strcmp(requete[0],"en")==0)
            strcpy(messageDuServeur, getEN(requete[1]));
          else if(strcmp(requete[0],"fr")==0)
            strcpy(messageDuServeur, getFR(requete[1]));
          else if(strcmp(requete[0],"list")==0)

          if(strcmp(requete[1],"fr")==0) {
int i;
            for(i=0;i<TAILLE;i++) {
            strcat(messageDuServeur, fr[i]);
            strcat(messageDuServeur, ",");
            }
          }
          else if(strcmp(requete[1],"en")==0) {
int i;
            for(i=0;i<TAILLE;i++) {
            strcat(messageDuServeur, en[i]);
            strcat(messageDuServeur, ",");
            }
          }

          if(strcmp(messageDuServeur,"??")==0)
            sprintf(messageDuServeur, "?? %s",requete[1]);
      }

    //sprintf(messageDuServeur, "le message %s est passe !!\n", messageDuClient);
    send(clientSocket,messageDuServeur, sizeof(messageDuServeur),0);
  }

  // fermer le socket
  close(serv_socket);

  return EXIT_SUCCESS;
}


// LES FONCTIONS DE TRADUCTION

char * getEN(char * motFR)
{
  int i;
for (i=0; i<TAILLE; i++)
 {
  if (!strcmp(motFR,fr[i])) break;
  }
 if (i==TAILLE)
return "??";
 else
return en[i];
}



char * getFR(char * motEN) {
int i;
for (i=0; i<TAILLE; i++)
{
   if (!strcmp(motEN,en[i]))
 break;
}
 if (i==TAILLE)
 return "??" ;
  else
 return fr[i];
 }

char ** strsplit(const char* str, const char* delim, size_t* numtokens)
{
     char *s = strdup(str);
    size_t tokens_alloc = 1;
     size_t tokens_used = 0;
     char **tokens = calloc(tokens_alloc, sizeof(char*));
     char *token, *strtok_ctx;
       for (token = strtok_r(s, delim, &strtok_ctx);
  token != NULL;
 token = strtok_r(NULL, delim, &strtok_ctx))
{
         if (tokens_used == tokens_alloc)
 {
   tokens_alloc *= 2;
           tokens = realloc(tokens, tokens_alloc * sizeof(char*));
   }
         tokens[tokens_used++] = strdup(token);
  }
       if (tokens_used == 0)
 {
         free(tokens);
         tokens = NULL;
  }
 else {
       tokens = realloc(tokens, tokens_used * sizeof(char*));
  }
     *numtokens = tokens_used;
    free(s);
    return tokens;
 }

/*
	Fonction de découpage de chaine en fonction d'un (ou plusieurs) caractère(s) séparateur(s)
	Les chaines résultantes du découpage seront rangées dans un tableau (alloué) de taille nbItems

	Déclarations nécessaires :
	char **requete;
	size_t nbItems;

	Exemple d'utilisation:
	requete = strsplit("FR blue", " ", &nbItems);
	printf("%d\n",nbItems); // 2
	printf("%s\n",requete[0]); // FR
	printf("%s\n",requete[1]); // blue
*/
