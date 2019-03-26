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
        unsigned char buffer[1000];

        recv(sockfd, buffer, 1000, 0);

        printf("%d\n", recup_type(buffer));
        if(recup_type(buffer) == 1) {
            Joueur * j = recevoir_liste_joueurs(buffer);
            printf("%s\n", j->pseudo);
            afficher_scores(j);
            recv(sockfd, buffer, 1000, 0);
            recevoir_abandon_joueur(buffer, j);
            
            Joueur * init =  j;
            do {
                if(j->abandon) {
                    printf("Le joueur %s a abandonne\n", joueur_pseudo(j));
                }
                j = joueur_suivant(j);
            } while(j != init);
        }
        else if(recup_type(buffer) == 2) {
            recevoir_plateau(buffer,  pl);
            afficher_plateau(pl);
        }

        // Fermeture sockets
        fermer_connexion(sockfd);

        return 0; 
    }
    else return 1;
}
