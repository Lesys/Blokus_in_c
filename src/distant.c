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
#include <fcntl.h>
#include <errno.h>
#ifndef WINDOWS
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#define INVALID_SOCKET -1
#define SOCKET_ERROR -1
#define closesocket(s) close(s)
typedef int SOCKET;
typedef struct sockaddr_in SOCKADDR_IN;
typedef struct sockaddr SOCKADDR;
typedef struct in_addr IN_ADDR;
#endif
#ifdef WINDOWS
#include <winsock2.h>
#include <ws2tcpip.h>
#endif

#include "../include/distant.h"
#include "../include/couleur.h"
#include "../include/commun.h"
#include "../include/joueur.h"
#include "../include/affichage_sdl.h"

// Variables globales externes
extern SDL_Renderer * renderer;

/**
 * \fn int connexion(char * adresse, int port);
 * \brief Effectue une connexion à un hote distant
 * \param adresse Adresse de l'hote (ip ou nom domaine)
 * \param port Port sur lequel se connecter
 * \return Numéro du socket si connexion, 0 sinon
 */
int connexion(char * adresse, int port) {
/*
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
    memset(&serv_addr, 0, sizeof(serv_addr));

    // IPv4
    serv_addr.sin_family = AF_INET;
    
    // Copie de l'adresse dans la structure
    memcpy(server->h_addr_list[0], &serv_addr.sin_addr.s_addr, server->h_length);

    // Copie du port dans la structure
    serv_addr.sin_port = htons(port);

    // Connexion
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) { 
        fprintf(stderr, "Erreur lors de la connexion\n");
        return 0;
    }*/

    SOCKET sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd == INVALID_SOCKET)
    {
        fprintf(stderr, "Erreur socket()\n");
        return 0;
    }

    struct hostent *hostinfo = NULL;
    SOCKADDR_IN sin = { 0 }; /* initialise la structure avec des 0 */

    hostinfo = gethostbyname(adresse); /* on récupère les informations de l'hôte auquel on veut se connecter */
    if (hostinfo == NULL) /* l'hôte n'existe pas */
    {
        fprintf(stderr, "Erreur résolution hote\n");
        return 0;
    }

    sin.sin_addr = *(IN_ADDR *) hostinfo->h_addr; /* l'adresse se trouve dans le champ h_addr de la structure hostinfo */
    sin.sin_port = htons(port); /* on utilise htons pour le port */
    sin.sin_family = AF_INET;

    if(connect(sockfd,(SOCKADDR *) &sin, sizeof(SOCKADDR)) == SOCKET_ERROR)
    {
        fprintf(stderr, "Erreur connect()\n");
        return 0;
    }

    // Mise du socket en non bloquant
    #ifndef WINDOWS
    int flags = fcntl(sockfd, F_GETFL);
    fcntl(sockfd, F_SETFL, flags | O_NONBLOCK);
    #endif
    #ifdef WINDOWS
    unsigned long mode = 1;
    ioctlsocket(sockfd, FIONBIO, &mode);
    #endif

    return sockfd;
}

/**
 * \fn int accepter_connexion(int port);
 * \brief Accepte une connexion d'un joueur distant
 * \param port Port sur lequel écouter
 * \return Numéro du socket si connexion, 0 sinon
 */
int accepter_connexion(int port) {

    /*int sockfd, newsockfd;
    socklen_t clilen;
    struct sockaddr_in serv_addr, cli_addr;

    // Ouverture du socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd < 0) { 
        fprintf(stderr,"Erreur lors de l'ouverture du socket\n");
        return 0;
    }
    
    // Mise à zéro de la structure de l'adresse du serveur
    memset(&serv_addr, 0, sizeof(serv_addr));

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

    closesocket(sockfd);*/

    SOCKET sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd == INVALID_SOCKET)
    {
        fprintf(stderr, "Erreur socket()\n");
        return 0;
    }

    SOCKADDR_IN sin = { 0 };

    sin.sin_addr.s_addr = htonl(INADDR_ANY); /* nous sommes un serveur, nous acceptons n'importe quelle adresse */

    sin.sin_family = AF_INET;

    sin.sin_port = htons(PORT_DEFAUT);

    if(bind (sockfd, (SOCKADDR *) &sin, sizeof sin) == SOCKET_ERROR)
    {
        fprintf(stderr, "Erreur bind()\n");
        return 0;
    }

    if(listen(sockfd, 1) == SOCKET_ERROR)
    {
        fprintf(stderr, "Erreur listen\n");
        return 0;
    }

    SOCKADDR_IN csin = { 0 };
    SOCKET newsockfd;

    int sinsize = sizeof csin;

    newsockfd = accept(sockfd, (SOCKADDR *)&csin, &sinsize);

    closesocket(sockfd);

    if(newsockfd == INVALID_SOCKET)
    {
        fprintf(stderr, "Erreur accept()\n");
        return 0;
    }

    // Mise du socket en non bloquant
    #ifndef WINDOWS
    int flags = fcntl(newsockfd, F_GETFL);
    fcntl(newsockfd, F_SETFL, flags | O_NONBLOCK);
    #endif
    #ifdef WINDOWS
    unsigned long mode = 1;
    ioctlsocket(newsockfd, FIONBIO, &mode);
    #endif


    return newsockfd;
}

/**
 * \fn void fermer_connexion(int sockfd);
 * \brief Ferme une connexion distante
 * \param sockfd Numéro du socket à fermer
 */
void fermer_connexion(int sockfd) {
    closesocket(sockfd);
}

int recevoir_buffer(int sockfd, unsigned char buffer[TAILLE_BUFF]) {
    int nb_lus = 0;

    while (nb_lus < TAILLE_BUFF) {
        int n = recv(sockfd, buffer + nb_lus, 1, 0);

        if (n > 0) {
            nb_lus++;
        }
        else if (n < 0) {
            return nb_lus;
        }
        else {
            return -1;
        }
    }
    return nb_lus;
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

int initialisation_partie_distant_sdl(Joueur ** j) {
    
	SDL_Event event;
	int continuer = 1;
        char adresse[TAILLE_PSEUDO] = {0};
        char pseudo[TAILLE_PSEUDO] = {0};
        int sockfd;
        
        // Saisie adresse
        SDL_StartTextInput();

        while(continuer){

            SDL_RenderClear(renderer);
            while(SDL_PollEvent(&event)){

                if(event.type == SDL_QUIT)
                    return 3;

                else if(adresse > 0 && event.type == SDL_KEYDOWN
                        && (event.key.keysym.sym == SDLK_RETURN || event.key.keysym.sym == SDLK_KP_ENTER) )
                    continuer = 0;

                else if(event.key.keysym.sym == SDLK_BACKSPACE
                        && event.type == SDL_KEYDOWN) {
                    if (adresse > 0)
                        adresse[strlen(adresse) - 1] = '\0';
                }

                else if(event.type == SDL_TEXTINPUT && strlen(adresse) < TAILLE_PSEUDO) {
                    strcat(adresse, event.text.text);
                }
            }

            afficher_saisie_adresse_sdl(adresse);
            SDL_RenderPresent(renderer);
        }

        // Connexion
        sockfd = connexion(adresse, PORT_DEFAUT);
        

        // Saisie du pseudo
        continuer = 1;
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
        envoyer_pseudo(sockfd, pseudo);
        
        SDL_StopTextInput();

        // Attente du début de la partie
        unsigned char buffer[TAILLE_BUFF];
        int r;

        while (r = recevoir_buffer(sockfd, buffer)) {
            SDL_RenderClear(renderer);
            afficher_attente_debut_sdl();
            SDL_RenderPresent(renderer);
        }
        if (r < 0) {
            return 3;
        }
        else {
            *j = recevoir_liste_joueurs(buffer);
        }

        return sockfd;

}

// Renvoie 2 si ecran titres, 3 si croix
int jouer_manche_distant_sdl(Couleur pl[TAILLE_PLATEAU][TAILLE_PLATEAU], Joueur * j) {

}
