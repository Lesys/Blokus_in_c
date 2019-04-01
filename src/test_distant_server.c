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
        unsigned char buffer[TAILLE_BUFF];

        while(1) {

            if (recevoir_buffer(sockfd, buffer) > 0) {
                if (recup_type(buffer) == 0) return 1;

                printf("Reception communication type %d\n", recup_type(buffer));
                if(recup_type(buffer) == 1) {
                    Joueur * j = recevoir_liste_joueurs(buffer);
                    afficher_scores(j);
                }
                else if(recup_type(buffer) == 2) {
                    recevoir_plateau(buffer,  pl);
                    afficher_plateau(pl);
                }
                else if(recup_type(buffer) == 3) {
                    char pseudo[TAILLE_PSEUDO];
                    recevoir_pseudo(buffer, pseudo);
                    printf("Le joueur a choisi le pseudo %s\n", pseudo);
                }
            }

        }


        // Fermeture sockets
        fermer_connexion(sockfd);

        return 0; 
    }
    else return 1;
}
