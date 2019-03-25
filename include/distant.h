#ifndef DISTANT_H
#define DISTANT_H

#include "../include/commun.h"
#include "../include/couleur.h"
#include "../include/joueur.h"

int recup_type(unsigned char * buffer);
void envoyer_plateau(int sockfd, Couleur pl[TAILLE_PLATEAU][TAILLE_PLATEAU]);
void recevoir_plateau(unsigned char * buffer, Couleur pl[TAILLE_PLATEAU][TAILLE_PLATEAU]);
void envoyer_liste_joueurs(int sockfd, Joueur * j);
Joueur * recevoir_liste_joueurs(unsigned char * buffer);
void envoyer_abandon_joueur(int sockfd, Joueur * j);
void recevoir_abandon_joueur(unsigned char * buffer, Joueur * j);

int serializef(unsigned char** buff, char* format, ...);
int deserializef(unsigned char* buff, char* format, ...);

#endif
