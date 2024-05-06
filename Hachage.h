#ifndef HACHAGE_H
#define HACHAGE_H
#include "Reseau.h"
#include "Chaine.h"


// Structure de la table de hachage
typedef struct {
    int tailleMax;       // Taille maximale de la table de hachage
    CellNoeud **T;       // Tableau de pointeurs vers des listes de noeuds
    int nbElement;       // Nombre d'élement dans le tableau
} TableHachage;

int fonctionHachage(int cle, int tailleTable);
int hachage(int k, int M);
int clef(double x, double y);
TableHachage* creerTableHachage(int tailleMax);
void libererTableHachage(TableHachage* table);
CellNoeud* rechercherDansTableHachage(TableHachage *table, int num);
Reseau *reconstitueReseauTableHachage(Chaines *C);
Reseau* reconstitueReseauHachage(Chaines *C, int M);

#endif /* HACHAGE_H */

