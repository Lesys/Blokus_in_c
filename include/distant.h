#ifndef DISTANT_H
#define DISTANT_H

#include "../include/commun.h"
#include "../include/couleur.h"
#include "../include/joueur.h"

int connexion(char * adresse, int port);
int accepter_connexion(int port);
void fermer_connexion(int sockfd);
int recup_type(unsigned char * buffer);
void envoyer_plateau(int sockfd, Couleur pl[TAILLE_PLATEAU][TAILLE_PLATEAU]);
void recevoir_plateau(unsigned char * buffer, Couleur pl[TAILLE_PLATEAU][TAILLE_PLATEAU]);
void envoyer_liste_joueurs(int sockfd, Joueur * j);
Joueur * recevoir_liste_joueurs(unsigned char * buffer);
void envoyer_abandon_joueur(int sockfd, Joueur * j);
void recevoir_abandon_joueur(unsigned char * buffer, Joueur * j);
void envoyer_pseudo(int sockfd, char * pseudo);
void recevoir_pseudo(unsigned char * buffer, char * pseudo);

Joueur * initialisation_partie_distant_sdl();

#endif
