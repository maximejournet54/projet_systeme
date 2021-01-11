#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <time.h>
#include "header.h"//fichier contenant l'entete des fonctions

#define PORT 6000
#define MAX_BUFFER 1000
#define MAX_CLIENTS 100
#define MAX_TICKET 100
/*
typedef struct t Ticket;

struct Ticket{
    char idTicket[10];//char au lieu de int car fprintf attend comme argument un char*
    char nom[100];
    char prenom[100];
} t;
*/

const char *EXIT = "exit";

int main(int argc , char const *argv[]) {
    int fdSocketAttente;
    int fdSocketCommunication;
    struct sockaddr_in coordonneesServeur;
    struct sockaddr_in coordonneesAppelant;
    char tampon[MAX_BUFFER];
    int nbRecu;
    int longueurAdresse;
    int pid;

    fdSocketAttente = socket(PF_INET, SOCK_STREAM, 0);

    if (fdSocketAttente < 0) {
        printf("socket incorrecte\n");
        exit(EXIT_FAILURE);
    }

    // On prépare l’adresse d’attachement locale
    longueurAdresse = sizeof(struct sockaddr_in);
    memset(&coordonneesServeur, 0x00, longueurAdresse);

    coordonneesServeur.sin_family = PF_INET;
    // toutes les interfaces locales disponibles
    coordonneesServeur.sin_addr.s_addr = htonl(INADDR_ANY);
    // toutes les interfaces locales disponibles
    coordonneesServeur.sin_port = htons(PORT);

    if (bind(fdSocketAttente, (struct sockaddr *) &coordonneesServeur, sizeof(coordonneesServeur)) == -1) {
        printf("erreur de bind\n");
        exit(EXIT_FAILURE);
    }

    if (listen(fdSocketAttente, 5) == -1) {
        printf("erreur de listen\n");
        exit(EXIT_FAILURE);
    }

    socklen_t tailleCoord = sizeof(coordonneesAppelant);

    int nbClients = 0;

    while (nbClients < MAX_CLIENTS) {
        if ((fdSocketCommunication = accept(fdSocketAttente, (struct sockaddr *) &coordonneesAppelant,&tailleCoord)) == -1) {
            printf("erreur de accept\n");
            exit(EXIT_FAILURE);
        }

        printf("Client connecté - %s:%d\n",inet_ntoa(coordonneesAppelant.sin_addr),ntohs(coordonneesAppelant.sin_port));

        nbClients++;
    }

    close(fdSocketCommunication);
    close(fdSocketAttente);
    printf("Fin du programme.\n");

    return EXIT_SUCCESS;
}

void genererNumeroDossier(char buffer[]){
    long max=9999999999, min=1000000000;
    srand(time(NULL));//initialisation de la graine
    long no=(rand() % (max-min+1)) +min;//genere un nombre pseudo-aléatoire de 10 chiffres
    snprintf(buffer, MAX_BUFFER, "%lld", no);//pb de format: format ‘%lld’ expects argument of type ‘long long int’, but argument 4 has type ‘long int’
}


