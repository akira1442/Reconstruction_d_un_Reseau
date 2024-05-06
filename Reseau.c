#include <stdlib.h>
#include <stdio.h>
#include "Reseau.h"
#include "SVGwriter.h"

Reseau* creerReseau(int gamma){

    Reseau* r = (Reseau *) malloc(sizeof(Reseau));
    
    r->nbNoeuds = 0;
    r->gamma = gamma;
    r->noeuds = NULL;
    r->commodites = NULL;

    return r;
}

Noeud* creerNoeud(int num, double x, double y){

    Noeud* new_node = (Noeud*) malloc(sizeof(Noeud));
    if (new_node == NULL){
        printf("Erreur d'allocation de la mémoire lors de creerNoeud()\n");
        exit(EXIT_FAILURE);
    }

    new_node->num = num+1;
    new_node->x = x;
    new_node->y = y;
    new_node->voisins = NULL;

    return new_node;
}

CellNoeud* creerCellNoeud(Noeud* nd){

    CellNoeud* new_cell = (CellNoeud*) malloc(sizeof(CellNoeud));

    if (new_cell == NULL) {
        fprintf(stderr, "Erreur d'allocation de mémoire pour une nouvelle cellule de noeud\n");
        exit(EXIT_FAILURE);
    }

    new_cell->nd = nd;
    new_cell->suiv = NULL;

    return new_cell;
}

CellCommodite* creerCellCommodite(Noeud* nTete, Noeud* nQueue){

    CellCommodite* new_cell = (CellCommodite*)malloc(sizeof(CellCommodite));

    new_cell->extrA = nTete;
    new_cell->extrB = nQueue;
    new_cell->suiv = NULL;

    return new_cell;
}

void insererCommodite(Reseau* R, CellCommodite* cm){

    cm->suiv = R->commodites;
    R->commodites = cm;
}

void inserer_voisin(Noeud* node, Noeud* voisin){

    if (!node || !voisin){
        fprintf(stderr, "Erreur impossible d'ajouter voisin, noeud vide\n");
        exit(EXIT_FAILURE);
    }

    CellNoeud* Lvoisin = node->voisins;
    
    while (Lvoisin != NULL){
        if (Lvoisin->nd == voisin){
            return;
        }
        Lvoisin = Lvoisin->suiv;
    }

    if (!Lvoisin){
        Lvoisin = (CellNoeud*) malloc(sizeof(CellNoeud));
        Lvoisin->nd = voisin;
        Lvoisin->suiv = node->voisins;
        node->voisins = Lvoisin;
    }
}

Noeud* rechercheCreeNoeudListe(Reseau *R, double x, double y) {
    
    // Recherche du noeud correspondant aux coordonnées (x, y)
    CellNoeud *current = R->noeuds;
    while (current != NULL) {
        if ((current->nd->x == x) && (current->nd->y == y)) {
            return current->nd; // Le noeud existe déjà, retourne le noeud existant
        }
        current = current->suiv;
    }
    
    // Le noeud n'existe pas, on le crée et l'ajoute à la liste des noeuds du réseau
    Noeud *new_node = creerNoeud(R->nbNoeuds, x, y);
    
    // Ajout du nouveau noeud à la liste des noeuds du réseau
    CellNoeud *new_cell = creerCellNoeud(new_node);
    new_cell->suiv = R->noeuds;
    R->noeuds = new_cell;
    
    // Mise à jour du nombre de noeuds dans le réseau
    R->nbNoeuds++;
    
    return new_node;
}

Reseau* reconstitueReseauListe(Chaines* C){
    // Vérification si la liste de chaînes est vide
    if (!C) 
        return NULL;

    // Initialisation du réseau
    Reseau* R = creerReseau(C->gamma);

    // Parcours de chaque chaîne dans la liste des chaînes
    CellChaine* cc = C->chaines;
    while(cc) {
        CellPoint* cp = cc->points;

        // Commodite
        Noeud* node_head = rechercheCreeNoeudListe(R, cp->x, cp->y);
        Noeud* node_tail = NULL;

        //Parcours des points dans CellChaine
        while (cp){
            CellPoint* tmp_CP = cc->points;
            Noeud* n1 = rechercheCreeNoeudListe(R, cp->x, cp->y);

            //Création des voisins
            while (tmp_CP){
                if (tmp_CP->suiv == cp){
                    Noeud* n2 = rechercheCreeNoeudListe(R, tmp_CP->x, tmp_CP->y);
                    inserer_voisin(n1, n2);
                    inserer_voisin(n2, n1);
                }
                tmp_CP = tmp_CP->suiv;
            }
            node_tail = rechercheCreeNoeudListe(R, cp->x, cp->y);
            cp = cp->suiv;
        }
        CellCommodite* cmdt = creerCellCommodite(node_head, node_tail);
        insererCommodite(R, cmdt);
        cc = cc->suiv;
    }
    return R;
}

void libererCellNoeud(CellNoeud* cn){

    CellNoeud* curr = cn;
    CellNoeud* tmp = NULL;

    while (curr){
        tmp = curr;
        curr = curr->suiv;
        if (tmp->nd){
            libererNoeud(tmp->nd);
        }
        free(tmp);
    }
}

void libererNoeud(Noeud* node){

    
    free(node);
}

void libererCellCommodite(CellCommodite* cc){

    while(cc){
        CellCommodite* tmp = cc;
        cc = cc->suiv;
        free(tmp);
    }
    //free(cc);
}

void libererReseau(Reseau* R){

    CellCommodite* cc = R->commodites;

    while (cc){
        CellCommodite* tmp_cc = cc;
        cc = cc->suiv;
        free(tmp_cc);
    }

    CellNoeud* cn = R->noeuds;

    while (cn){
        Noeud* n = cn->nd;
        CellNoeud* tmp_cn = cn;
        CellNoeud* voisin = n->voisins;
        while (voisin){
            CellNoeud* tmp_voisin = voisin;
            voisin = voisin->suiv;
            free(tmp_voisin);
        }
        cn = cn->suiv;
        free(tmp_cn);
        free(n);
    }
    free(R);
}


void ecrireReseau(Reseau *R, FILE *f) {
    
    // Écriture du nombre de noeuds, de liaisons, de commodités et de gamma
    fprintf(f, "NbNoeuds: %d\n", R->nbNoeuds);
    fprintf(f, "NbLiaisons: %d\n", nbLiaisons(R));
    fprintf(f, "NbCommodites: %d\n", nbCommodites(R));
    fprintf(f, "Gamma: %d\n\n", R->gamma);

    // Écriture des coordonnées de chaque noeud
    CellNoeud *current = R->noeuds;
    while (current != NULL) {
        fprintf(f, "v %d %.6f %.6f\n", current->nd->num, current->nd->x, current->nd->y);
        current = current->suiv;
    }

    // Écriture des liaisons entre les noeuds
    current = R->noeuds;
    while (current != NULL) {
        CellNoeud *voisin = current->nd->voisins;
        while (voisin != NULL) {
            // Pour éviter de réécrire deux fois la même liaison, on vérifie que l'extrémité A est de numéro inférieur à B
            if (current->nd->num < voisin->nd->num) {
                fprintf(f, "l %d %d\n", current->nd->num, voisin->nd->num);
            }
            voisin = voisin->suiv;
        }
        current = current->suiv;
    }

    // Écriture des commodités
    CellCommodite *commodite = R->commodites;
    while (commodite != NULL) {
        fprintf(f, "k %d %d\n", commodite->extrA->num, commodite->extrB->num);
        commodite = commodite->suiv;
    }
}

// Fonction pour compter le nombre de commodites dans un reseau
int nbCommodites(Reseau *R) {
    int count = 0;
    // Parcours de la liste des commodites
    CellCommodite *commodite = R->commodites;
    while (commodite != NULL) {
        count++;
        commodite = commodite->suiv;
    }
    return count;
}

// Fonction pour compter le nombre de liaisons dans un reseau
int nbLiaisons(Reseau *R) {
    int count = 0;
    // Parcours de chaque noeud du reseau pour compter les liaisons
    CellNoeud *cell = R->noeuds;
    while (cell != NULL) {
        Noeud *nd = cell->nd;
        // Parcours de chaque voisin du noeud
        CellNoeud *voisin = nd->voisins; 
        while (voisin != NULL) {
            count++;
            voisin = voisin->suiv;
        }
        cell = cell->suiv;
    }
    // Chaque liaison est comptée deux fois, donc on divise par 2
    return count / 2;
}

void afficheReseauSVG(Reseau *R, char* nomInstance){
    CellNoeud *courN,*courv;
    SVGwriter svg;
    double maxx=0,maxy=0,minx=1e6,miny=1e6;

    courN=R->noeuds;
    while (courN!=NULL){
        if (maxx<courN->nd->x) maxx=courN->nd->x;
        if (maxy<courN->nd->y) maxy=courN->nd->y;
        if (minx>courN->nd->x) minx=courN->nd->x;
        if (miny>courN->nd->y) miny=courN->nd->y;
        courN=courN->suiv;
    }
    SVGinit(&svg,nomInstance,500,500);
    courN=R->noeuds;
    while (courN!=NULL){
        SVGpoint(&svg,500*(courN->nd->x-minx)/(maxx-minx),500*(courN->nd->y-miny)/(maxy-miny));
        courv=courN->nd->voisins;
        while (courv!=NULL){
            if (courv->nd->num<courN->nd->num)
                SVGline(&svg,500*(courv->nd->x-minx)/(maxx-minx),500*(courv->nd->y-miny)/(maxy-miny),500*(courN->nd->x-minx)/(maxx-minx),500*(courN->nd->y-miny)/(maxy-miny));
            courv=courv->suiv;
        }
        courN=courN->suiv;
    }
    SVGfinalize(&svg);
}

