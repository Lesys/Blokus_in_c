#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <unistd.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 

#include "../include/distant.h"
#include "../include/couleur.h"
#include "../include/affichage.h"
#include "../include/joueur.h"

int main(int argc, char *argv[]) {

    int sockfd, portno;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    // Arguments invalides
    if (argc < 4) {
       fprintf(stderr,"usage %s hostname port n°test\n", argv[0]);
       exit(0);
    }

    // Récupération en int du numéro de port
    portno = atoi(argv[2]);
    
    // Ouverture du socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        fprintf(stderr, "Erreur lors de l'ouverture du socket\n");
    }

    // Résolution du nom de domaine
    server = gethostbyname(argv[1]);
    if (server == NULL) {
        fprintf(stderr,"Erreur, host introuvable\n");
        exit(0);
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
    serv_addr.sin_port = htons(portno);

    // Connexion
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) { 
        fprintf(stderr, "Erreur lors de la connexion\n");
    }
    
    // Envoi des paquets de test selon l'argument
    if (argv[3][0] == '1') {
        Joueur * j = joueur_liste_creation(4);
        
        j->pseudo = "AZDUAHZD";
        j = joueur_suivant(j);
        j->pseudo = "Aazduhieuhfazdaa";
        j = joueur_suivant(j);
        j->pseudo = "dsfoijsdfji";
        j = joueur_suivant(j);
        j->pseudo = "azodiazgzgz";
        j = joueur_suivant(j);

        envoyer_liste_joueurs(sockfd, j);

        printf("Test envoi liste joueurs + abandon:\n");
        afficher_scores(j);

        joueur_abandonne(j);
        printf("Le joueur %s a abandonné\n", joueur_pseudo(j));
        envoyer_abandon_joueur(sockfd, j);
    }
    else if (argv[3][0] == '2') {
        Couleur pl[TAILLE_PLATEAU][TAILLE_PLATEAU] = {0};
        pl[0][0] = JAUNE;

        printf("Test envoi plateau :\n");
        afficher_plateau(pl);
        envoyer_plateau(sockfd, pl);
    }

    // Fermeture du socket
    close(sockfd);

    return 0;
}
