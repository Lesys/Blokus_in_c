#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <arpa/inet.h>

#include "../include/couleur.h"
#include "../include/commun.h"
#include "../include/joueur.h"

int recup_type(unsigned char * buffer) {

    int type;

    memcpy(&type, buffer, sizeof(int));

    return type;
}

void envoyer_plateau(int sockfd, Couleur pl[TAILLE_PLATEAU][TAILLE_PLATEAU]) {
    
    int type = 2;
    unsigned char buffer[1000] = {0};
    int offset = 0;
    unsigned char tmp;

    memcpy(buffer, &type, sizeof(int));
    offset += sizeof(int);

    for(int i = 0; i < TAILLE_PLATEAU; i++) {
        for(int j = 0; j < TAILLE_PLATEAU; j++) {
            tmp = pl[i][j];
            memcpy(buffer + offset, &tmp, sizeof(unsigned char));
            offset += sizeof(unsigned char);
        }
    }

    send(sockfd, buffer, offset, 0);
}

void recevoir_plateau(unsigned char * buffer, Couleur pl[TAILLE_PLATEAU][TAILLE_PLATEAU]) {
    
    int offset = sizeof(int);
    unsigned char tmp;

    for(int i = 0; i < TAILLE_PLATEAU; i++) {
        for(int j = 0; j < TAILLE_PLATEAU; j++) {
            memcpy(&tmp, buffer + offset, sizeof(unsigned char));
            offset += sizeof(unsigned char);
            pl[i][j] = tmp;
        }
    }
}

void envoyer_liste_joueurs(int sockfd, Joueur * j) {
    
    int type = 1;
    unsigned char buffer[1000] = {0};
    int offset = 0;

    memcpy(buffer + offset, &type, sizeof(int));
    offset += sizeof(int);

    Joueur * init = j;
    int nb_joueurs =  0;

    do {
        nb_joueurs++;
        j = joueur_suivant(j);
    } while (j != init);

    memcpy(buffer + offset, &nb_joueurs, sizeof(int));
    offset += sizeof(int);

    do {
        memcpy(buffer + offset, joueur_pseudo(j), TAILLE_PSEUDO);
        offset += TAILLE_PSEUDO;
        j = joueur_suivant(j);
    } while(j != init);

    send(sockfd, buffer, offset, 0);
}

Joueur * recevoir_liste_joueurs(unsigned char * buffer) {
    
    int offset = sizeof(int);
    int nb_joueurs =  0;

    memcpy(&nb_joueurs, buffer + offset, sizeof(int));
    offset += sizeof(int);

    Joueur * j = joueur_liste_creation(nb_joueurs);

    for (int i = 0; i < nb_joueurs; i++) {
        memcpy(j->pseudo, buffer + offset, TAILLE_PSEUDO);
        offset += TAILLE_PSEUDO;
        j = joueur_suivant(j);
    }

    return j;
}

void envoyer_abandon_joueur(int sockfd, Joueur * j) {
    
    int type = 3;
    unsigned char buffer[1000] = {0};
    int offset = 0;

    memcpy(buffer + offset, &type, sizeof(int));
    offset += sizeof(int);

    memcpy(buffer + offset, joueur_pseudo(j), TAILLE_PSEUDO);
    offset += TAILLE_PSEUDO;

    send(sockfd, buffer, offset, 0);
}

void recevoir_abandon_joueur(unsigned char * buffer, Joueur * j) {
    
    int offset = sizeof(int);
    char pseudo[TAILLE_PSEUDO];

    memcpy(pseudo, buffer + offset, TAILLE_PSEUDO);

    Joueur * init = j;

    do {
        if(!strcmp(pseudo, joueur_pseudo(j))) {
            joueur_abandonne(j);
        }
        j = joueur_suivant(j);
    } while(j != init);
}
