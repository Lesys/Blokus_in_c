#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdarg.h>
#include "../include/distant.h"

int main(int argc, char *argv[]) {

    int sockfd, newsockfd, portno;
    socklen_t clilen;
    struct sockaddr_in serv_addr, cli_addr;

    // Arguments invalides
    if (argc < 2) {
        fprintf(stderr,"Erreur, pas de port en paramètre\n");
        return 1;
    }

    // Ouverture du socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd < 0) { 
        fprintf(stderr,"Erreur lors de l'ouverture du socket\n");
    }
    
    // Mise à zéro de la structure de l'adresse du serveur
    bzero((char *) &serv_addr, sizeof(serv_addr));

    // Récupération en int du numéro de port
    portno = atoi(argv[1]);

    // IPv4
    serv_addr.sin_family = AF_INET;
    
    // Réception sur toutes les interfaces
    serv_addr.sin_addr.s_addr = INADDR_ANY;

    // Copie du port dans la structure
    serv_addr.sin_port = htons(portno);

    // Copie de l'adresse dans la structure
    if (bind(sockfd, (struct sockaddr *) &serv_addr, 
        sizeof(serv_addr)) < 0) { 
            fprintf(stderr,"Erreur lors du bind\n");
    }

    // Ecoute sur le socket créé
    listen(sockfd,5);

    // Taille adresse client 
    clilen = sizeof(cli_addr);

    // Acceptation de la connexion
    newsockfd = accept(sockfd, 
                (struct sockaddr *) &cli_addr, 
                &clilen);
    
    if (newsockfd < 0) {
        fprintf(stderr, "Erreur lors de l'acceptation d'un socket\n");
    }

    unsigned char buff[1000];
    char * str;
    short a; 
    int b;

    recv(newsockfd, buff, 1000, 0);
    int n = deserializef(buff, "%s%h%i", &str, &a, &b);

    printf("%d) %s - %d - %d\n", n, str, a, b);

    // Fermeture sockets
    close(newsockfd);
    close(sockfd);

    return 0; 
}
