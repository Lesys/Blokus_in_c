#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>

#include "../include/distant.h"
#include "../include/affichage.h"
#include "../include/joueur.h"

int main(int argc, char *argv[]) {

    int sockfd = accepter_connexion(atoi(argv[1]));

    if (sockfd) {
        Couleur pl[TAILLE_PLATEAU][TAILLE_PLATEAU] = {0};

        while(1) {

            unsigned char buffer[1000] = {0};
            recv(sockfd, buffer, 1000, 0);

            printf("Reception communication type %d\n", recup_type(buffer));
            if(recup_type(buffer) == 1) {
                Joueur * j = recevoir_liste_joueurs(buffer);
                afficher_scores(j);
            }
            else if(recup_type(buffer) == 2) {
                recevoir_plateau(buffer,  pl);
                afficher_plateau(pl);
            }
            else if(recup_type(buffer) == 4) {
                char pseudo[TAILLE_PSEUDO];
                recevoir_pseudo(buffer, pseudo);
                printf("Le joueur a choisi le pseudo %s\n", pseudo);
            }
        }


        // Fermeture sockets
        fermer_connexion(sockfd);

        return 0; 
    }
    else return 1;
}
