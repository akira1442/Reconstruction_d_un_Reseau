#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Reseau.h"
#include "SVGwriter.h"
#include "Chaine.h"
//#include "Hachage.h"

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s input_filename method\n", argv[0]);
        fprintf(stderr, "method: 1 - Liste, 2 - Table de hachage, 3 - Arbre\n");
        return EXIT_FAILURE;
    }

    // Lecture du fichier .cha
    FILE *input_file = fopen(argv[1], "r");
    if (input_file == NULL) {
        fprintf(stderr, "Erreur lors de l'ouverture du fichier %s\n", argv[1]);
        return EXIT_FAILURE;
    }

    Chaines *chaines = lectureChaines(input_file);
    fclose(input_file);

    if (chaines == NULL) {
        fprintf(stderr, "Erreur lors de la lecture des chaines\n");
        return EXIT_FAILURE;
    }

    // Choix de la méthode de reconstitution du réseau
    int method = atoi(argv[2]);
    Reseau *reseau;
    switch (method) {
        case 1:
            printf("Utilisation de la méthode Liste\n");
            reseau = reconstitueReseauListe(chaines);
            break;
        case 2:
            printf("Utilisation de la méthode Table de hachage\n");
            // Implémentation de la méthode utilisant une table de hachage
            // Création de la table de hachage
            //reseau = reconstitueReseauTableHachage(chaines);
            break;
        case 3:
            printf("Utilisation de la méthode Arbre\n");
            // Implémentation de la méthode utilisant un arbre
            break;
        default:
            fprintf(stderr, "Méthode non valide\n");
            return EXIT_FAILURE;
    }

    // Écriture du réseau dans un fichier
    FILE *output_file = fopen("reseau.txt", "w");
    if (output_file == NULL) {
        fprintf(stderr, "Erreur lors de la création du fichier reseau.txt\n");
        return EXIT_FAILURE;
    }

    ecrireReseau(reseau, output_file);
    fclose(output_file);

    // Affichage du réseau sous forme de SVG
    afficheReseauSVG(reseau, "reseau.html");

    printf("Le réseau a été reconstruit avec succès et écrit dans reseau.txt.\n");

    return EXIT_SUCCESS;
}