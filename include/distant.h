#ifndef DISTANT_H
#define DISTANT_H

#include "../include/commun.h"
#include "../include/couleur.h"
#include "../include/joueur.h"

typedef enum type_message {
    LISTE_JOUEURS = 1,
    PLATEAU,
    ABANDON_JOUEUR,
    PSEUDO,
    PRET
} Type_message;

int connexion(char * adresse, int port);
int creer_socket_connexion(int port);
int accepter_connexion(int sockfd);
void fermer_connexion(int sockfd);
void fermer_connexions_distantes(Joueur * j);
int recevoir_buffer(int sockfd, unsigned char buffer[TAILLE_BUFF]);
int recup_type(unsigned char * buffer);
int envoyer_plateau(int sockfd, Couleur pl[TAILLE_PLATEAU][TAILLE_PLATEAU], int id_piece);
int recevoir_plateau(unsigned char * buffer, Couleur pl[TAILLE_PLATEAU][TAILLE_PLATEAU]);
int envoyer_liste_joueurs(int sockfd, Joueur * j);
Joueur * recevoir_liste_joueurs(unsigned char * buffer);
int envoyer_abandon_joueur(int sockfd, Joueur * j);
void recevoir_abandon_joueur(unsigned char * buffer, Joueur * j);
int envoyer_pseudo(int sockfd, char * pseudo);
void recevoir_pseudo(unsigned char * buffer, char * pseudo);
int envoyer_pret(int sockfd);

int erreur_reseau();

int initialisation_partie_distant_sdl(Joueur ** j);
int jouer_manche_distant_sdl(Couleur pl[TAILLE_PLATEAU][TAILLE_PLATEAU], Joueur * j, int hote);

#endif
