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

int main(int argc, char *argv[]) {

    int sockfd, portno;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    // Arguments invalides
    if (argc < 3) {
       fprintf(stderr,"usage %s hostname port\n", argv[0]);
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
    
    // Envoi des paquets de test
    unsigned char * buffer = malloc(sizeof(unsigned char) * 1000);
    char str[] = "Hello world !";

    serializef(&buffer, "%s%h%i", str, 12, 999);
    send(sockfd, buffer, 1000, 0);

    // Fermeture du socket
    close(sockfd);

    return 0;
}
