//entete des différentes fonctions utilisees
void ajouterTicket();
void afficherListeTickets();
void menuClient();
void genererNumeroDossier(char buffer[]);

typedef struct t Ticket;

struct Ticket{
    char idTicket[20];//char au lieu de int car fprintf attend comme argument un char*
    char nom[100];
    char prenom[100];
} t;
