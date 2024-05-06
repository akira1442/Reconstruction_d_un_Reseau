#include <stdio.h>
#include <stdlib.h>
#include "Chaine.h"
#include "SVGwriter.h"

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s input_filename output_filename\n", argv[0]);
        return EXIT_FAILURE;
    }

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

    FILE *output_file = fopen(argv[2], "w");
    if (output_file == NULL) {
        fprintf(stderr, "Erreur lors de la création du fichier %s\n", argv[2]);
        return EXIT_FAILURE;
    }

    ecrireChaines(chaines, output_file);
    fclose(output_file);

    printf("Les données ont été lues depuis %s et réécrites dans %s avec succès.\n", argv[1], argv[2]);

    // Affichage graphique des instances au format SVG
    afficheChainesSVG(chaines, "output_svg");

    // Calcul de la longueur totale des chaines
    double longueur_totale = longueurTotale(chaines);
    printf("La longueur totale des chaines sont : %.2f\n", longueur_totale);

    // Calcul du nombre total de points
    int total_points = comptePointsTotal(chaines);
    printf("Le nombre total de points est : %d\n", total_points);

    return EXIT_SUCCESS;
}
