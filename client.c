#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include "header.h"

#define MAX_TICKET 100

typedef struct t Ticket;

struct Ticket{
    char idTicket[20];//char au lieu de int car fprintf attend comme argument un char*
    char nom[100];
    char prenom[100];
} t;

#define PORT 6000
#define MAX_BUFFER 1000

const char *EXIT = "exit";

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

    menuClient();

    close(fdSocket);

    return EXIT_SUCCESS;

}

//corps des fonctions

void ajouterTicket(){
    //ouverture ou creation du fichier
    FILE* fichier=NULL;
    fichier=fopen("test.txt","a");//a+ =droits en écriture à partir de la fin du fichier, cree le fichier s'il n'existe pas
    struct Ticket t1;

    if(fichier != NULL){
        //le no de ticket doit etre genere par le serveur => void genererNumeroDossier()
        printf("Saisir un numéro de ticket:\n");
        scanf("%s",t1.idTicket);

        printf("Saisir le nom:\n");
        scanf("%s",t1.nom);

        printf("Saisir le prénom:\n ");
        scanf("%s",t1.prenom);

        //ajout des informations du ticket dans le fichier txt
        fprintf(fichier,"id: %s ", t1.idTicket);
        fprintf(fichier, "nom: %s ", t1.nom);
        fprintf(fichier, "prenom: %s \n", t1.prenom);

        fclose(fichier);
    }
    else{
        printf("impossible d'ouvrir le fichier");
    }
}

void afficherListeTickets(){
    FILE* fichier=NULL;
    fichier=fopen("test.txt","r");//r=lecture seule

    char str[1000];//1000=nb max de caracteres du fichier
   
    if(fichier != NULL){
        while(fgets(str,1000,fichier)!=NULL){
            printf("%s", str);
        }     
    }
    else{
        printf("impossible d'ouvrir le fichier");
    }

    fclose(fichier);
}

void menuClient(){
    int choix=0;
    printf("Bienvenue dans l'application de gestion des places de concert.\nQue voulez vous faire?\n1: consulter la liste des billets vendus\t 2: acheter un billet.\n");
    scanf("%d", &choix);

    if(choix==1){
        printf("Vous avez choisi d'afficher la liste des billets vendus.\n");
        afficherListeTickets();
    }

    else if(choix==2){
        printf("Vous avez choisi d'acheter un billet.\n");
        ajouterTicket();
    }

    else{
        printf("Choix invalide, vous allez être redirigé vers l'accueil.\n");
        menuClient();
    }
}



