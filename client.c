#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>


//il y a peut etre pas besoin de tous les import
#define PORT 6000
#define MAX_BUFFER 1000

const char *EXIT = "exit";

//entête des fonctions, a mettre dans un fichier .h a la fin



int main(int argc , char const *argv[]) {
  int fdSocket;
      int nbRecu;
      struct sockaddr_in coordonneesServeur;
      int longueurAdresse;
      char tampon[MAX_BUFFER];

      fdSocket = socket(AF_INET, SOCK_STREAM, 0);

      if (fdSocket < 0) {
          printf("socket incorrecte\n");
          exit(EXIT_FAILURE);
      }

      // On prépare les coordonnées du serveur
   longueurAdresse = sizeof(struct sockaddr_in);
   memset(&coordonneesServeur, 0x00, longueurAdresse);

   coordonneesServeur.sin_family = PF_INET;
   // adresse du serveur
   inet_aton("127.0.0.1", &coordonneesServeur.sin_addr);
   // toutes les interfaces locales disponibles
   coordonneesServeur.sin_port = htons(PORT);

   if (connect(fdSocket, (struct sockaddr *) &coordonneesServeur, sizeof(coordonneesServeur)) == -1) {
       printf("connexion impossible\n");
       exit(EXIT_FAILURE);
   }

   printf("connexion ok\n");


   close(fdSocket);

   return EXIT_SUCCESS;

}



