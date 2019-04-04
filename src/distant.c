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
#define h_addr h_addr_list[0]
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
#include "../include/gestion_partie.h"
#include "../include/gestion_partie_sdl.h"
#include "../include/son.h"

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
        fprintf(stderr, "Erreur resolution hote\n");
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
 * \fn int creer_socket_connexion(int port)
 * \brief Créé un socket le met sur écoute
 * \param port Port sur lequel créer le socket
 * \return Numéro du socket si réussi, 0 sinon
 */
int creer_socket_connexion(int port) {

    SOCKET sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd == INVALID_SOCKET)
    {
        fprintf(stderr, "Erreur socket()\n");
        return 0;
    }

    char enable = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int));

    SOCKADDR_IN sin = { 0 };

    sin.sin_addr.s_addr = htonl(INADDR_ANY); /* nous sommes un serveur, nous acceptons n'importe quelle adresse */

    sin.sin_family = AF_INET;

    sin.sin_port = htons(PORT_DEFAUT);

    if(bind (sockfd, (SOCKADDR *) &sin, sizeof sin) == SOCKET_ERROR)
    {
        perror("Erreur Bind()");
        fprintf(stderr, "Erreur bind()\n");
        return 0;
    }

    if(listen(sockfd, 1) == SOCKET_ERROR)
    {
        fprintf(stderr, "Erreur listen\n");
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
 * \fn int accepter_connexion(int sockfd);
 * \brief Accepte une connexion d'un joueur distant sur un socket d'écoute
 * \param sockfd Socket sur lequel écouter
 * \return Numéro du socket si connexion, -1 sinon
 */
int accepter_connexion(int sockfd) {

    SOCKADDR_IN csin = { 0 };
    SOCKET newsockfd;

    int sinsize = sizeof csin;

    newsockfd = accept(sockfd, (SOCKADDR *)&csin, &sinsize);

    if (newsockfd == -1) {
        return -1;
    }

    closesocket(sockfd);

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

/**
 * \fn void fermer_connexions_distantes(Joueur * j)
 * \brief Ferme les connexions à tout les joueurs distants
 * \param j Liste des joueurs
 */
void fermer_connexions_distantes(Joueur * j) {

    Joueur * init = j;

    do {
        if (j->sockfd) {
            fermer_connexion(j->sockfd);
            j->sockfd = 0;
        }
        j = joueur_suivant(j);
    } while (j !=  init);
}

/**
 * \fn int recevoir_buffer(int sockfd, unsigned char buffer[TAILLE_BUFF])
 * \brief Recoit un buffer
 * \param sockfd Socket depuis lequel lire
 * \param buffer Buffer dans lequel écrire
 * \param Nombre d'octets lus, -1 si erreur
 */
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
 * \return Le type (voir enum)
 */
int recup_type(unsigned char * buffer) {

    int type;

    // Récupération de la taille d'un int au début du buffer
    memcpy(&type, buffer, sizeof(int));

    return type;
}

/**
 * \fn int envoyer_plateau(int sockfd, Couleur pl[TAILLE_PLATEAU][TAILLE_PLATEAU], int id_piece);
 * \brief Envoie l'état du plateau
 * \param sockfd Numéro du socket sur lequel envoyer
 * \param pl Plateau courant
 * \param id_piece Id de la piece posée
 * \param Nombre d'octets envoyés, -1 si erreur 
 */
int envoyer_plateau(int sockfd, Couleur pl[TAILLE_PLATEAU][TAILLE_PLATEAU], int id_piece) {

    int type = PLATEAU;
    unsigned char buffer[500] = {0};
    unsigned char c;
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

    // Ecriture de l'id piece
    memcpy(buffer + offset, &id_piece, sizeof(int));
    offset += sizeof(int);

    // Vérification socket encore ouvert
    if (recv(sockfd, &c, 1, 0) == 0) {
        return -1;
    }

    // Envoi
    return send(sockfd, buffer, offset, 0);
}

/**
 * \fn int recevoir_plateau(unsigned char * buffer, Couleur pl[TAILLE_PLATEAU][TAILLE_PLATEAU]);
 * \brief Recoit l'état du plateau
 * \param buffer Buffer à lire
 * \param pl Plateau local (sera modifié)
 */
int recevoir_plateau(unsigned char * buffer, Couleur pl[TAILLE_PLATEAU][TAILLE_PLATEAU]) {

    int offset = sizeof(int);
    unsigned char tmp;
    int id_piece;

    // Lecture des cases une à une
    for(int i = 0; i < TAILLE_PLATEAU; i++) {
        for(int j = 0; j < TAILLE_PLATEAU; j++) {
            memcpy(&tmp, buffer + offset, sizeof(unsigned char));
            offset += sizeof(unsigned char);
            pl[i][j] = tmp;
        }
    }

    memcpy(&id_piece, buffer + offset, sizeof(int));

    return id_piece;
}

/**
 * \fn int envoyer_liste_joueurs(int sockfd, Joueur * j);
 * \brief Envoie la liste des joueurs au joueur distant
 * \param sockfd Numéro du socket à qui envoyer
 * \param j Liste des joueurs
 * \param Nombre d'octets envoyés, -1 si erreur
 */
int envoyer_liste_joueurs(int sockfd, Joueur * j) {

    int type = LISTE_JOUEURS;
    unsigned char buffer[TAILLE_PSEUDO*5] = {0};
    unsigned char c;
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

    // Vérification socket encore ouvert
    if (recv(sockfd, &c, 1, 0) == 0) {
        return -1;
    }

    // Envoi
    return send(sockfd, buffer, offset, 0);
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
 * \fn int envoyer_abandon_joueur(int sockfd, Joueur * j);
 * \brief Envoi un message annoncant l'abandon d'un joueur
 * \param sockfd Numéro du socket à qui envoyer
 * \param j Joueur qui abandonne
 * \param Nombre d'octets envoyés, -1 si erreur
 */
int envoyer_abandon_joueur(int sockfd, Joueur * j) {

    int type = ABANDON_JOUEUR;
    unsigned char buffer[TAILLE_PSEUDO*2] = {0};
    unsigned char c;
    int offset = 0;

    // Ecriture du type
    memcpy(buffer + offset, &type, sizeof(int));
    offset += sizeof(int);

    // Ecriture du pseudo
    memcpy(buffer + offset, joueur_pseudo(j), TAILLE_PSEUDO);
    offset += TAILLE_PSEUDO;

    // Vérification socket encore ouvert
    if (recv(sockfd, &c, 1, 0) == 0) {
        return -1;
    }

    // Envoi
    return send(sockfd, buffer, offset, 0);
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
 * \fn int envoyer_pseudo(int sockfd, char * pseudo);
 * \brief Envoie le pseudo choisi à l'hote
 * \param sockfd Numéro du socket à qui envoyer
 * \param pseudo Pseudo choisi
 * \param Nombre d'octets envoyés, -1 si erreur
 */
int envoyer_pseudo(int sockfd, char * pseudo) {

    int type = PSEUDO;
    unsigned char buffer[TAILLE_PSEUDO*2] = {0};
    unsigned char c;
    int offset = 0;

    // Ecriture du type
    memcpy(buffer + offset, &type, sizeof(int));
    offset += sizeof(int);

    // Ecriture du pseudo
    memcpy(buffer + offset, pseudo, TAILLE_PSEUDO);
    offset += TAILLE_PSEUDO;

    // Vérification socket encore ouvert
    if (recv(sockfd, &c, 1, 0) == 0) {
        return -1;
    }

    // Envoi
    return send(sockfd, buffer, offset, 0);
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

/**
 * \fn void envoyer_pret(int sockfd);
 * \brief Envoie un message disant que l'on est pret
 * \param sockfd Numéro du socket à qui envoyer
 * \param Nombre d'octets envoyés, -1 si erreur
 */
int envoyer_pret(int sockfd) {

    int type = PRET;
    unsigned char buffer[sizeof(int)] = {0};
    unsigned char c;

    // Ecriture du type
    memcpy(buffer, &type, sizeof(int));

    // Vérification socket encore ouvert
    if (recv(sockfd, &c, 1, 0) == 0) {
        return -1;
    }

    // Envoi
    return send(sockfd, buffer, sizeof(int), 0);
}

/**
 * \fn int erreur_reseau()
 * \brief Affiche un message d'erreur réseau avec un bouton retour
 * \return 2 si appui sur le bouton retour, 3 si appui sur la croix
 */
int erreur_reseau() {

    int r = 1;
    SDL_Event event;
    Bouton* b_retour = init_bouton_sdl(RETOUR);

    jouer_son(CLOCHE);

    while (r == 1) {

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                r = 3;
            }
            else if (event.type == SDL_MOUSEBUTTONDOWN && curs_hover_bouton(b_retour)) {
                r = 2;
                jouer_son(BOUTON_RETOUR);
            }
        }

        SDL_RenderClear(renderer);
        afficher_erreur_reseau();
        afficher_bouton_sdl(b_retour);
        SDL_RenderPresent(renderer);
    }

    return r;
}

/**
 * \fn int initialisation_partie_distant_sdl(Joueur ** j)
 * \brief Initialise la connexion avec un hote et attends le début de la partie
 * \param j Liste des joueurs
 * \return numéro de socket de l'hote si ok,  2 si appui sur le bouton retour, 3 si appui sur la croix
 */
int initialisation_partie_distant_sdl(Joueur ** j) {

	SDL_Event event;
	int continuer = 1;
    char adresse[TAILLE_PSEUDO] = {0};
    char pseudo[TAILLE_PSEUDO] = {0};
    int sockfd;
	Bouton* b_retour = init_bouton_sdl(RETOUR);
	
    // Saisie adresse
    SDL_StartTextInput();

    while(continuer){

        SDL_RenderClear(renderer);
        while(SDL_PollEvent(&event)){

            if(event.type == SDL_QUIT)
                return 3;

		//Si il appuis sur un bouton
		else if(event.type == SDL_MOUSEBUTTONDOWN){
			if(curs_hover_bouton(b_retour)) {
                jouer_son(BOUTON_RETOUR);
				return 2;
            }
		}
            else if(adresse > 0 && event.type == SDL_KEYDOWN
                    && (event.key.keysym.sym == SDLK_RETURN || event.key.keysym.sym == SDLK_KP_ENTER)) {
                jouer_son(BOUTON);
                continuer = 0;
            }

            else if(event.key.keysym.sym == SDLK_BACKSPACE
                    && event.type == SDL_KEYDOWN) {
                if (adresse > 0)
                    adresse[strlen(adresse) - 1] = '\0';
            }

            else if(event.type == SDL_TEXTINPUT && strlen(adresse) < TAILLE_PSEUDO) {
                strcat(adresse, event.text.text);
            }
        }
        afficher_bouton_sdl(b_retour);
        afficher_saisie_adresse_sdl(adresse);
        SDL_RenderPresent(renderer);
    }

    // Connexion
    sockfd = connexion(adresse, PORT_DEFAUT);

    if (sockfd == 0) {
        return erreur_reseau();
    }


    // Saisie du pseudo
    continuer = 1;
    while(continuer){

        SDL_RenderClear(renderer);
        while(SDL_PollEvent(&event)){

            if(event.type == SDL_QUIT)
                return 3;
	//Si il appuis sur un bouton
		else if(event.type == SDL_MOUSEBUTTONDOWN){
			if(curs_hover_bouton(b_retour)) {
                            jouer_son(BOUTON_RETOUR);
                            fermer_connexion(sockfd);
				return 2;
                        }
		}
            else if(pseudo > 0 && event.type == SDL_KEYDOWN
                    && (event.key.keysym.sym == SDLK_RETURN || event.key.keysym.sym == SDLK_KP_ENTER) ) {
                jouer_son(BOUTON);
                continuer = 0;
            }

            else if(event.key.keysym.sym == SDLK_BACKSPACE
                    && event.type == SDL_KEYDOWN) {
                if (pseudo > 0)
                    pseudo[strlen(pseudo) - 1] = '\0';
            }

            else if(event.type == SDL_TEXTINPUT && strlen(pseudo) < TAILLE_PSEUDO) {
                strcat(pseudo, event.text.text);
            }
        }
        afficher_bouton_sdl(b_retour);
        afficher_saisie_pseudo_distant_sdl(pseudo);
        SDL_RenderPresent(renderer);
    }

    // Envoi du pseudo
    envoyer_pseudo(sockfd, pseudo);

    SDL_StopTextInput();

    // Attente du début de la partie
    unsigned char buffer[TAILLE_BUFF];
    int r = 0;

    do {
        while(SDL_PollEvent(&event)){

            if(event.type == SDL_QUIT)
                return 3;
        }

        SDL_RenderClear(renderer);
        afficher_attente_debut_sdl();
        SDL_RenderPresent(renderer);
        r = recevoir_buffer(sockfd, buffer);
    } while (r == 0);
    if (r < 0) {
        return 3;
    }
    else {
        *j = recevoir_liste_joueurs(buffer);
        Joueur * init = *j;
        do {
            if (strcmp(joueur_pseudo(*j), pseudo)  == 0) {
                (*j)->sockfd = -1;
            }
            else {
                (*j)->sockfd = sockfd;
            }
            *j = joueur_suivant(*j);
        } while (*j  != init);
    }
	free_bouton_sdl(&b_retour);
    return sockfd;

}

// 1 si tout est ok, 2 si deconnexion, 3 si croix
int attente_nouvelle_partie_distant(int hote) {
	envoyer_pret(hote); // Envoyer_pret() à l'hote
    	int nb_recois = 0;
	int type;
	unsigned char buffer[TAILLE_BUFF];
	SDL_Event event;	// Afficher le message d'attente avec afficher_attente_nouvelle_partie()
        // Gestion des évènements (retour 3 si on appuie sur la croix)

	while(nb_recois == 0){
		while(SDL_PollEvent(&event)){
			if(event.type == SDL_QUIT)
				return 3;
		}
	        SDL_RenderClear(renderer);
		afficher_attente_debut_sdl();
		SDL_RenderPresent(renderer);
		nb_recois= recevoir_buffer(hote,buffer);
	}
	// Récupération d'un message dans le buffer avec int recevoir_buffer(int sockfd, unsigned char buffer[TAILLE_BUFF])
	// si la fonction renvoie -1 -> erreur de connexion on retourne 2

        if(nb_recois == -1){
		return 2;
	}

        // Sinon on utilises la fonction int recup_type(unsigned char * buffer) qui renvoie le type de message
        // Si le type de message = PRET on retourne 1
	else{
		type = recup_type(buffer);
		if(type == PRET)
			return 1;
		else
			return 2;
	}
}

/**
 * \fn int jouer_manche_distant_sdl(Couleur pl[TAILLE_PLATEAU][TAILLE_PLATEAU], Joueur * j, int hote)
 * \brief Joue une manche en tant que joueur distant
 * \param pl Plateau courant
 * \param j Liste des joueurs
 * \param hote Numéro  de socket de l'hote
 * \return 2 si retour ecran titre, 3 si fermeture directe
 */
int jouer_manche_distant_sdl(Couleur pl[TAILLE_PLATEAU][TAILLE_PLATEAU], Joueur * j, int hote) {

    int choix;
    Joueur * init;
    int r = 0;

    do{
        initialisation_manche(pl,&j);
        do {
            if(j->sockfd == -1) {
                jouer_son(CLOCHE);
                init = j;
                choix = jouer_tour_joueur_sdl(pl,&j);
                if(choix != 4) {
                    if (joueur_a_abandonne(init)) {
                        jouer_son(ABANDON);
                        r = envoyer_abandon_joueur(hote, init);
                    }
                    else {
                        jouer_son(POSE_PIECE);
                        r = envoyer_plateau(hote, pl, choix * -1);
                        printf("%d\n", r);
                    }
                }
                if (r == -1) {
                    fermer_connexion(hote);
                    return erreur_reseau();
                }
            }
            else {
                choix = jouer_tour_joueur_distant_sdl(pl,&j);
                if (choix < 0) {
                    jouer_son(POSE_PIECE);
                }
                else if (choix != 4) {
                    jouer_son(ABANDON);
                }
            }

          

            if(choix == 3) {
                fermer_connexion(hote);
                return choix;
            }

            choix=fin_de_partie_sdl(&j);

            if (choix == 1) {
                choix = attente_nouvelle_partie_distant(hote);
            }

        } while(!(choix));
    } while(choix == 1 );
    fermer_connexion(hote);
    return choix;
}
