/**
 * \file distant.c
 * \brief Fonctions pour le jeu multijoueur
 * \details Fonction qui gèrent les connexions des joueurs
 * distants ainsi que la logique du jeu pour ces joueurs
 * \author JODEAU Alexandre et BASTIDE Robin
 */

#include <stdlib.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <stdarg.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 

#include "../include/couleur.h"
#include "../include/commun.h"
#include "../include/joueur.h"

/**
 * \fn int connexion(char * adresse, int port);
 * \brief Effectue une connexion à un hote distant
 * \param adresse Adresse de l'hote (ip ou nom domaine)
 * \param port Port sur lequel se connecter
 * \return Numéro du socket si connexion, 0 sinon
 */
int connexion(char * adresse, int port) {

    int sockfd;
    struct sockaddr_in serv_addr;
    struct hostent * server;
    
    // Ouverture du socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        fprintf(stderr, "Erreur lors de l'ouverture du socket\n");
        return 0;
    }

    // Résolution du nom de domaine
    server = gethostbyname(adresse);
    if (server == NULL) {
        fprintf(stderr,"Erreur, host introuvable\n");
        return 0;
    }

    // Mise à zéro de la structure de l'adresse du serveur   
    bzero(&serv_addr, sizeof(serv_addr));

    // IPv4
    serv_addr.sin_family = AF_INET;
    
    // Copie de l'adresse dans la structure
    bcopy((char *)server->h_addr_list[0], 
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);

    // Copie du port dans la structure
    serv_addr.sin_port = htons(port);

    // Connexion
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) { 
        fprintf(stderr, "Erreur lors de la connexion\n");
        return 0;
    }

    return sockfd;
}

/**
 * \fn int accepter_connexion(int port);
 * \brief Accepte une connexion d'un joueur distant
 * \param port Port sur lequel écouter
 * \return Numéro du socket si connexion, 0 sinon
 */
int accepter_connexion(int port) {

    int sockfd, newsockfd;
    socklen_t clilen;
    struct sockaddr_in serv_addr, cli_addr;

    // Ouverture du socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd < 0) { 
        fprintf(stderr,"Erreur lors de l'ouverture du socket\n");
        return 0;
    }
    
    // Mise à zéro de la structure de l'adresse du serveur
    bzero((char *) &serv_addr, sizeof(serv_addr));

    // IPv4
    serv_addr.sin_family = AF_INET;
    
    // Réception sur toutes les interfaces
    serv_addr.sin_addr.s_addr = INADDR_ANY;

    // Copie du port dans la structure
    serv_addr.sin_port = htons(port);

    // Copie de l'adresse dans la structure
    if (bind(sockfd, (struct sockaddr *) &serv_addr, 
        sizeof(serv_addr)) < 0) { 
            fprintf(stderr,"Erreur lors du bind\n");
            return 0;
    }

    // Ecoute sur le socket créé
    listen(sockfd,1);

    // Taille adresse client 
    clilen = sizeof(cli_addr);

    // Acceptation de la connexion
    newsockfd = accept(sockfd, 
                (struct sockaddr *) &cli_addr, 
                &clilen);
    
    if (newsockfd < 0) {
        fprintf(stderr, "Erreur lors de l'acceptation d'un socket\n");
        return 0;
    }

    close(sockfd);

    return newsockfd;
}

/**
 * \fn void fermer_connexion(int sockfd);
 * \brief Ferme une connexion distante
 * \param sockfd Numéro du socket à fermer
 */
void fermer_connexion(int sockfd) {
    close(sockfd);
}

/**
 * \fn int recup_type(unsigned char * buffer);
 * \brief Récupère le type de communication contenue dans le buffer
 * \param buffer Buffer à lire
 * \return 0-Erreur, 1-Liste joueurs, 2-Etat plateau, 3-Abandon joueur, 4-Pseudo
 */
int recup_type(unsigned char * buffer) {

    int type;

    // Récupération de la taille d'un int au début du buffer
    memcpy(&type, buffer, sizeof(int));

    return type;
}

/**
 * \fn void envoyer_plateau(int sockfd, Couleur pl[TAILLE_PLATEAU][TAILLE_PLATEAU]); 
 * \brief Envoie l'état du plateau
 * \param sockfd Numéro du socket sur lequel envoyer
 * \param pl Plateau courant
 */
void envoyer_plateau(int sockfd, Couleur pl[TAILLE_PLATEAU][TAILLE_PLATEAU]) {
    
    int type = 2;
    unsigned char buffer[500] = {0};
    int offset = 0;
    unsigned char tmp;

    // Ecriture du type
    memcpy(buffer, &type, sizeof(int));
    offset += sizeof(int);

    // Ecriture des cases une à une
    for(int i = 0; i < TAILLE_PLATEAU; i++) {
        for(int j = 0; j < TAILLE_PLATEAU; j++) {
            tmp = pl[i][j];
            memcpy(buffer + offset, &tmp, sizeof(unsigned char));
            offset += sizeof(unsigned char);
        }
    }

    // Envoi
    send(sockfd, buffer, offset, 0);
}

/**
 * \fn void recevoir_plateau(unsigned char * buffer, Couleur pl[TAILLE_PLATEAU][TAILLE_PLATEAU]);
 * \brief Recoit l'état du plateau
 * \param buffer Buffer à lire
 * \param pl Plateau local (sera modifié)
 */
void recevoir_plateau(unsigned char * buffer, Couleur pl[TAILLE_PLATEAU][TAILLE_PLATEAU]) {
    
    int offset = sizeof(int);
    unsigned char tmp;

    // Lecture des cases une à une
    for(int i = 0; i < TAILLE_PLATEAU; i++) {
        for(int j = 0; j < TAILLE_PLATEAU; j++) {
            memcpy(&tmp, buffer + offset, sizeof(unsigned char));
            offset += sizeof(unsigned char);
            pl[i][j] = tmp;
        }
    }
}

/**
 * \fn void envoyer_liste_joueurs(int sockfd, Joueur * j);
 * \brief Envoie la liste des joueurs au joueur distant
 * \param sockfd Numéro du socket à qui envoyer
 * \param j Liste des joueurs
 */
void envoyer_liste_joueurs(int sockfd, Joueur * j) {
    
    int type = 1;
    unsigned char buffer[TAILLE_PSEUDO*5] = {0};
    int offset = 0;

    // Ecriture du type
    memcpy(buffer + offset, &type, sizeof(int));
    offset += sizeof(int);

    // Calcul du nombre de joueurs
    Joueur * init = j;
    int nb_joueurs =  0;

    do {
        nb_joueurs++;
        j = joueur_suivant(j);
    } while (j != init);

    // Ecriture du nombre de joueurs
    memcpy(buffer + offset, &nb_joueurs, sizeof(int));
    offset += sizeof(int);

    // Ecriture des pseudos
    do {
        memcpy(buffer + offset, joueur_pseudo(j), TAILLE_PSEUDO);
        offset += TAILLE_PSEUDO;
        j = joueur_suivant(j);
    } while(j != init);

    // Envoi
    send(sockfd, buffer, offset, 0);
}

/**
 * \fn joueur * recevoir_liste_joueurs(unsigned char * buffer);
 * \brief recoit la liste des joueurs et en créer une local
 * \param buffer buffer à lire
 * \return Liste des joueurs local
 */
Joueur * recevoir_liste_joueurs(unsigned char * buffer) {
    
    int offset = sizeof(int);
    int nb_joueurs =  0;

    // Récupération du nombre de joueurs
    memcpy(&nb_joueurs, buffer + offset, sizeof(int));
    offset += sizeof(int);

    // Création de la liste des joueurs
    Joueur * j = joueur_liste_creation(nb_joueurs);

    // Récupération des pseudos
    for (int i = 0; i < nb_joueurs; i++) {
        memcpy(j->pseudo, buffer + offset, TAILLE_PSEUDO);
        offset += TAILLE_PSEUDO;
        j = joueur_suivant(j);
    }

    return j;
}

/**
 * \fn void envoyer_abandon_joueur(int sockfd, Joueur * j);
 * \brief Envoi un message annoncant l'abandon d'un joueur
 * \param sockfd Numéro du socket à qui envoyer
 * \param j Joueur qui abandonne
 */
void envoyer_abandon_joueur(int sockfd, Joueur * j) {
    
    int type = 3;
    unsigned char buffer[TAILLE_PSEUDO*2] = {0};
    int offset = 0;

    // Ecriture du type
    memcpy(buffer + offset, &type, sizeof(int));
    offset += sizeof(int);

    // Ecriture du pseudo
    memcpy(buffer + offset, joueur_pseudo(j), TAILLE_PSEUDO);
    offset += TAILLE_PSEUDO;

    // Envoi
    send(sockfd, buffer, offset, 0);
}

/**
 * \fn void recevoir_abandon_joueur(unsigned char * buffer, Joueur * j);
 * \brief Recoit un message d'abandon d'un joueur et effectue les
 * changements dans la liste des joueurs 
 * \param buffer Buffer à lire
 * \param j Liste des joueurs
 */
void recevoir_abandon_joueur(unsigned char * buffer, Joueur * j) {
    
    int offset = sizeof(int);
    char pseudo[TAILLE_PSEUDO];

    // Récuperation du pseudo
    memcpy(pseudo, buffer + offset, TAILLE_PSEUDO);

    // Changement dans la liste des joueurs local
    Joueur * init = j;

    do {
        if(!strcmp(pseudo, joueur_pseudo(j))) {
            joueur_abandonne(j);
        }
        j = joueur_suivant(j);
    } while(j != init);
}

/**
 * \fn void envoyer_pseudo(int sockfd, char * pseudo);
 * \brief Envoie le pseudo choisi à l'hote
 * \param sockfd Numéro du socket à qui envoyer
 * \param pseudo Pseudo choisi
 */
void envoyer_pseudo(int sockfd, char * pseudo) {

    int type = 4;
    unsigned char buffer[TAILLE_PSEUDO*2] = {0};
    int offset = 0;

    // Ecriture du type
    memcpy(buffer + offset, &type, sizeof(int));
    offset += sizeof(int);

    // Ecriture du pseudo
    memcpy(buffer + offset, pseudo, TAILLE_PSEUDO);
    offset += TAILLE_PSEUDO;

    // Envoi
    send(sockfd, buffer, offset, 0);
}

/**
 * \fn void recevoir_pseudo(unsigned char * buffer, char * pseudo);
 * \brief Recoit un pseudo d'un joueur distant et le place dans pseudo
 * \param buffer Buffer à lire
 * \param pseudo Pseudo à affecter
 */
void recevoir_pseudo(unsigned char * buffer, char * pseudo) {

    int offset = sizeof(int);
    // Récuperation du pseudo
    memcpy(pseudo, buffer + offset, TAILLE_PSEUDO);
}

/*
Joueur * initialisation_partie_distant_sdl() {
    
        // Saisie du pseudo
	SDL_Event event;
	int continuer = 1;
        char pseudo[TAILLE_PSEUDO];

        SDL_StartTextInput();

        while(continuer){

            SDL_RenderClear(renderer);
            while(SDL_PollEvent(&event)){

                if(event.type == SDL_QUIT)
                    return 3;

                else if(pseudo > 0 && event.type == SDL_KEYDOWN
                        && (event.key.keysym.sym == SDLK_RETURN || event.key.keysym.sym == SDLK_KP_ENTER) )
                    continuer = 0;

                else if(event.key.keysym.sym == SDLK_BACKSPACE
                        && event.type == SDL_KEYDOWN) {
                    if (pseudo > 0)
                        pseudo[strlen(pseudo) - 1] = '\0';
                }

                else if(event.type == SDL_TEXTINPUT && strlen(pseudo) < TAILLE_PSEUDO) {
                    strcat(pseudo, event.text.text);
                }
            }

            afficher_saisie_pseudo_distant_sdl(pseudo);
            SDL_RenderPresent(renderer);
        }

        // Envoi du pseudo

        SDL_StopTextInput();
}*/
