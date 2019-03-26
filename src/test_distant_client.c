#include <stdio.h>
#include <stdlib.h>

#include "../include/distant.h"
#include "../include/couleur.h"
#include "../include/affichage.h"
#include "../include/joueur.h"

int main(int argc, char *argv[]) {

    int sockfd = connexion(argv[1], atoi(argv[2]));
    
    if (sockfd) {
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
        fermer_connexion(sockfd);

        return 0;
    }
    else return 1;
}
