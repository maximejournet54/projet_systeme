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
/*
typedef struct t Ticket;
struct Ticket{
  char idTicket[0];//char au lieu de int car fprintf attend comme argument un char*
  char nom[100];
  char prenom[100];
} t;
*/

#define PORT 6000
#define MAX_BUFFER 1000

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
  fichier=fopen("tickets.txt","a");//a =droits en écriture à partir de la fin du fichier, cree le fichier s'il n'existe pas
  struct Ticket t1;

  int choix=0;

  if(fichier != NULL){
      //le no de ticket doit etre genere par le serveur => void genererNumeroDossier()



      int fdSocket;
      int nbRecu;
      char tampon[MAX_BUFFER];
      char buff[1000];
      // on attend la réponse du serveur
      nbRecu = recv(fdSocket, tampon, MAX_BUFFER, 0);
      if (nbRecu > 0) {
          tampon[nbRecu] = 0;
          printf("Voici le no de dossier : %s\n", tampon);
      }

      //recuperer no de dossier du serveur et l'affecter a t1.idTicket
      printf("Saisir un numéro de ticket a 10 chiffres:\n");
      scanf("%s",t1.idTicket);


      printf("Saisir le nom:\n");
      scanf("%s",t1.nom);

      printf("Saisir le prénom:\n ");
      scanf("%s",t1.prenom);

      /*fprintf(fichier,"id: %s ", t1.idTicket);//a modifier
      fprintf(fichier, "nom: %s ", t1.nom);
      fprintf(fichier, "prenom: %s \n", t1.prenom);*/

      if (t1.idTicket != fread(buff,3,1,fichier)) {//warning: comparison between pointer and integer
        //ajout des informations du ticket dans le fichier txt
        fprintf(fichier,"id: %s ", t1.idTicket);//a modifier
        fprintf(fichier, "nom: %s ", t1.nom);
        fprintf(fichier, "prenom: %s \n", t1.prenom);
      }else{
        printf("\nCe numéro de place est déjà attribué\n");
        printf("\nRetour à l'accueil\n");
        menuClient();
      }

      fclose(fichier);

      printf("Pour revenir à l'accueil, tapez 1.\n");
      scanf("%d", &choix);
      if(choix==1){
          printf("\nRetour à l'accueil.\n");
          menuClient();
      }else{
          exit(1);
      }

  }
  else{
      printf("impossible d'ouvrir le fichier");
  }
}

void afficherListeTickets(){
  FILE* fichier=NULL;
  fichier=fopen("tickets.txt","r");//r=lecture seule

  int choix=0;

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

  printf("\n");
  scanf("%d", &choix);
  if(choix==1){
      printf("\nRetour à l'accueil.\n");
      menuClient();
  }else{
      exit(1);
  }

}

/*void annulerReservation() {
FILE* fichier=NULL;
FILE* fichiertemp=NULL;
char temp[] = "fichiertemp.txt";
char nouveaunom[] = "tickets.txt";
fichiertemp=fopen(temp, "w");
fichier=fopen("tickets.txt","r");
struct Ticket t1;
char str[1000];
int noL = 0;
int idTicket;

int choix=0;

if (fichier != NULL) {
  printf("Veuillez saisir votre id de ticket: \n");
  scanf("%s\n", idTicket);
  printf("bonjour\n");
  while (fgets(str,1000,fichier)!=NULL) {
    if (idTicket != fgets(str,1000,fichier)) { //modif ça
      printf("Ce ticket existe, suppression en cours\n");
      while (!feof(fichier)) {
        strcpy(str, "\0");
        fgets(str, 1000, fichier);
        if (!feof(fichier)) {
          noL++;
          if (idTicket != strstr(fichier, idTicket)) {
            fprintf(temp, "%s", str);
          }
        }
      }
    }
  }
}
fclose(fichier);
fclose(temp);
remove("tickets.txt");
rename(temp, nouveaunom);
}*/

void annulerReservation() {
FILE* fichier=NULL;
FILE* fichiertemp=NULL;
//char temp[] = "fichiertemp.txt";
fichiertemp=fopen("fichiertemp.txt", "w");
fichier=fopen("tickets.txt","a");
struct Ticket t1;
char str[1000];
int noL = 0;
int noT;

int choix=0;

if (fichier != NULL) {
  printf("Numéro du ticket à supprimer : ");
      scanf("%d", &noT);
	noT++;
      // copier le fichier dans le fichier temporaire sauf la ligne correspondant au ticket à supprimer
      while (!feof(fichier))
      {
          strcpy(str, "\0");
          fgets(str, 1000, fichier);
          if (!feof(fichier))
          {
              noL++;
              /* sauter la ligne contenant le n° rentré */
             if (noT != noL)
              {
                  fprintf(fichiertemp, "%s", str);//pb ici refuse de copier le texte dans l'autre fichier
              }
          }
      }
      fclose(fichier);
      fclose(fichiertemp);
      remove("tickets.txt");  		// supprimer le fichier de base
    rename("fichiertemp.txt", "tickets.txt"); 	// renomer le fichier temporaire
}
exit(1);
}


void menuClient(){
  int choix=0, choix2=0;
  printf("Bienvenue dans l'application de gestion des places de concert.\nQue voulez vous faire?\n1: consulter la liste des billets vendus\t 2: acheter un billet.\t 3: quitter l'application\t 4: Annuler une réservation.\n");
  scanf("%d", &choix);

  if(choix==1){
      printf("Vous avez choisi d'afficher la liste des billets vendus. Pour revenir à l'accueil, tapez 1.\n");
      afficherListeTickets();
  }

  else if(choix==2){
      printf("Vous avez choisi d'acheter un billet.\n");
      ajouterTicket();
  }

  else if(choix==3){
      printf("Vous avez choisi de quitter l'application.\n");
      exit(1);
  }

  else if(choix==4){
    printf("Vous avez choisi d'annuler une réservation. Pour revenir à l'accueil , tapez 1.\n");
    annulerReservation();
  }

  else{
      printf("\nChoix invalide, vous allez être redirigé vers l'accueil.\n");
      menuClient();
  }
}
