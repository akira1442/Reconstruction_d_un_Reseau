#include "Hachage.h"
#include "Reseau.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "Hachage.h"

// Fonction de hachage avec la méthode de la multiplication et la constante A
int hachage(int k, int M) {
    const double A = (sqrt(5) - 1) / 2;
    return floor(M * (k * A - floor(k * A)));
}

int clef(double x, double y){

    return (int)(y + (x + y) * (x + y + 1) / 2);
}

TableHachage* creerTableHachage(int tailleMax){

    TableHachage* new_th = (TableHachage*) malloc(sizeof(TableHachage));

    new_th->tailleMax = tailleMax;
    new_th->nbElement = 0;
    new_th->T = (CellNoeud**) malloc(sizeof(CellNoeud*)*tailleMax);

    for (int i = 0; i < tailleMax; ++i) {
        new_th->T[i] = NULL;
    }
    return new_th;
}

void libererTableHachage(TableHachage* table){

    CellNoeud* curr = NULL;
    CellNoeud* prec;
    for (int i = 0; i < table->tailleMax; i++){
        curr = table->T[i];
        prec = NULL;
        while (curr){
            prec = curr;
            curr = curr->suiv;
            free(prec);
        }
    }
    free(table->T);
    free(table);
}

Noeud* rechercheCreeNoeudHachage(Reseau* R, TableHachage* H, double x, double y) {
    int key = clef(x, y); // Calcul de la clé pour les coordonnées (x, y)
    int index = hachage(key, H->tailleMax); // Obtention de l'index dans la table de hachage
    
    // Parcours de la liste chainée à cet index pour vérifier si le noeud existe déjà
    CellNoeud* current = H->T[index];
    while (current != NULL) {
        if ((current->nd->x == x) && (current->nd->y == y)) {
            // Le noeud existe déjà dans la table de hachage, retourne ce noeud
            return current->nd;
        }
        current = current->suiv;
    }
    
    // Le noeud n'existe pas, on le crée et l'ajoute à la table de hachage et au réseau
    Noeud* new_node = creerNoeud(R->nbNoeuds, x, y);
    
    // Ajout du nouveau noeud à la table de hachage
    CellNoeud* new_cell = creerCellNoeud(new_node);
    new_cell->suiv = H->T[index];
    H->T[index] = new_cell;
    
    // Ajout du nouveau noeud à la liste des noeuds du réseau
    CellNoeud* new_cell2 = creerCellNoeud(new_node);
    new_cell2->suiv = R->noeuds;
    R->noeuds = new_cell2;
    
    // Mise à jour du nombre de noeuds dans le réseau
    R->nbNoeuds++;
    
    return new_node;
}

Reseau* reconstitueReseauHachage(Chaines *C, int M) {
    
    if (!C || (M <= 0)) return NULL; // Vérification des paramètres
    
    // Initialisation de la table de hachage et du Réseau
    TableHachage* TH = creerTableHachage(M);
    Reseau* R = creerReseau(C->gamma);
    
    // Parcours de la liste des chaînes
    CellChaine* cchain = C->chaines;
    while (cchain) {        
        CellPoint* cp = cchain->points;

        Noeud* node_head = rechercheCreeNoeudHachage(R, TH, cp->x, cp->y);
        Noeud* node_tail = NULL;

        while (cp){
            CellPoint* cp2 = cchain->points;
            Noeud* n = rechercheCreeNoeudHachage(R, TH, cp->x, cp->y);

            while (cp2){
                if (cp2->suiv == cp){
                    Noeud* n2 = rechercheCreeNoeudHachage(R, TH, cp2->x, cp2->y);
                    inserer_voisin(n, n2);
                    inserer_voisin(n2, n);
                }
                cp2 = cp2->suiv;
            }
            node_tail = rechercheCreeNoeudHachage(R, TH, cp->x, cp->y);
            cp = cp->suiv;
        }
        //Commodite
        CellCommodite* cm = creerCellCommodite(node_head, node_tail);
        insererCommodite(R, cm);
        
        cchain = cchain->suiv;
    }

    // Libération de la mémoire utilisée par la table de hachage
    libererTableHachage(TH);
    
    return R;
}

