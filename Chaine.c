#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "Chaine.h"
#include "SVGwriter.h" // Inclure le fichier d'en-tête SVGwriter.h


//Création d'une Chaine
Chaines* creerChaine(int gamma, int nbChaines, CellChaine* cc){

    Chaines* C = (Chaines*) malloc(sizeof(Chaines));

    if (C == NULL) {
        fprintf(stderr, "Erreur d'allocation de mémoire pour Chaines\n");
        exit(EXIT_FAILURE);
    }

    C->nbChaines = nbChaines;
    C->gamma = gamma;
    C->chaines = cc; 
}

// Création d'une CellChaine
CellChaine* creerCellChaine(int num, CellPoint* cp){

    CellChaine* new_chaine = (CellChaine*) malloc(sizeof(CellChaine));
    
    if (new_chaine == NULL) {
        fprintf(stderr, "Erreur d'allocation de mémoire pour CellChaine\n");
        exit(EXIT_FAILURE);
    }
    
    new_chaine->numero = num;
    new_chaine->points = cp;
    new_chaine->suiv = NULL;

    return new_chaine;
}

// Création d'un CellPoint
CellPoint* creerCellPoint(float x, float y){

    CellPoint* new_cp = (CellPoint*) malloc(sizeof(CellPoint));

    if (new_cp == NULL) {
        fprintf(stderr, "Erreur d'allocation de mémoire pour CellPoint\n");
        exit(EXIT_FAILURE);
    }

    new_cp->x = x;
    new_cp->y = y;
    new_cp->suiv = NULL;

    return new_cp;
}

CellChaine* insererCellChaine(CellChaine* cc, CellPoint* cp, int numero){

    CellChaine* new_cc = creerCellChaine(numero, cp);
    new_cc->suiv = cc;

    return new_cc;
}

void insererNouvelleChaine(Chaines* C, CellChaine* nouvelleChaine, int numero) {
    
    nouvelleChaine->suiv = C->chaines;
    nouvelleChaine->numero = numero;
    C->chaines = nouvelleChaine;
}

CellPoint* insererCellPoint(CellPoint* cp, float x, float y){

    CellPoint* new_point = creerCellPoint(x, y);
    new_point->suiv = cp;

    return new_point;
}

void libererCellPoint(CellPoint* cp){

    while (cp){
        CellPoint* tmp = cp;
        cp = cp->suiv;
        free(tmp);
    }
}

void libererCellChaine(CellChaine* cc){

    while (cc){
        CellChaine* tmp = cc;
        cc = cc->suiv;
        libererCellPoint(tmp->points);
        free(tmp);
    }
}

void libererChaine(Chaines* C){

    libererCellChaine(C->chaines);
    free(C);
}

Chaines* lectureChaines(FILE *f) {

    Chaines *C = NULL;
    CellChaine *nouvelle_chaine = NULL;

    // Lecture des informations générales
    int gamma, nbChaines;
    fscanf(f, "NbChain : %d\n", &nbChaines);
    fscanf(f, "Gamma : %d\n", &gamma);
    //if ((nbChaines > 0) && (gamma > 0)){
    //    printf("oui\n");
    //}
    printf("%d %d\n", nbChaines, gamma);

    // Lecture des chaînes
    for (int i = 0; i < nbChaines; i++) {
        int nbPoints, numero;
        fscanf(f, "%d %d ", &numero, &nbPoints); // Lecture du numéro et du nombre de points de la chaîne

        // Lecture des coordonnées des points
        CellPoint *nouveau_point = NULL;
        for (int j = 0; j < nbPoints; j++) {
            float x, y;
            fscanf(f, "%f %f", &x, &y);
            nouveau_point = insererCellPoint(nouveau_point, x, y);
        }

        nouvelle_chaine = insererCellChaine(nouvelle_chaine, nouveau_point, numero);
        fscanf(f, "\n");
    }

    C = creerChaine(gamma, nbChaines, nouvelle_chaine);
    
    return C;
}

// Fonction pour compter le nombre de points dans une chaîne
int comptePoints(CellPoint *points) {
    int count = 0;
    for (CellPoint *point = points; point != NULL; point = point->suiv) {
        count++;
    }
    return count;
}

// Fonction pour écrire les chaînes dans un fichier
void ecrireChaines(Chaines *C, FILE *f) {

    // Écriture des informations générales
    fprintf(f, "NbChain : %d\n", C->nbChaines);
    fprintf(f, "Gamma : %d\n", C->gamma);
    CellChaine *chaine = C->chaines;
    CellPoint *point = chaine->points;

    // Parcours des chaînes dans l'ordre inverse et écriture de chaque chaîne
    while (chaine) {
        // Écriture du numéro de la chaîne et du nombre de points
        fprintf(f, "%d %d", chaine->numero, comptePoints(chaine->points));

        // Parcours des points de la chaîne et écriture des coordonnées
        while (point) {
            fprintf(f, " %lf %lf", point->x, point->y);
            point = point->suiv;
        }
        fprintf(f, "\n");
        chaine = chaine->suiv;
    }
}

// Fonction pour calculer la distance entre deux points
double distance(double x1, double y1, double x2, double y2) {
    
    double x = x2 - x1;
    double y = y2 - y1;
    return sqrt(x*x + y*y);
}

// Fonction pour calculer la longueur d'une chaîne
double longueurChaine(CellChaine *c) {
    double longueur = 0.0;
    CellPoint *p = c->points;
    
    // Parcours de la chaîne pour calculer la longueur totale
    while (p != NULL && p->suiv != NULL) {
        longueur += distance(p->x, p->y, p->suiv->x, p->suiv->y);
        p = p->suiv;
    }

    return longueur;
}

// Fonction pour calculer la longueur totale de toutes les chaînes
double longueurTotale(Chaines *C) {
    double longueurTotale = 0.0;
    CellChaine *ccour = C->chaines;
    
    // Parcours de toutes les chaînes pour calculer la longueur totale
    while (ccour != NULL) {
        longueurTotale += longueurChaine(ccour);
        ccour = ccour->suiv;
    }

    return longueurTotale;
}

void afficheChainesSVG(Chaines *C, char* nomInstance) {
    int i;
    double maxx=0,maxy=0,minx=1e6,miny=1e6;
    CellChaine *ccour;
    CellPoint *pcour;
    double precx,precy;
    SVGwriter svg;
    ccour=C->chaines;
    while (ccour!=NULL){
        pcour=ccour->points;
        while (pcour!=NULL){
            if (maxx<pcour->x) maxx=pcour->x;
            if (maxy<pcour->y) maxy=pcour->y;
            if (minx>pcour->x) minx=pcour->x;
            if (miny>pcour->y) miny=pcour->y;  
            pcour=pcour->suiv;
        }
        ccour=ccour->suiv;
    }
    SVGinit(&svg,nomInstance,500,500);
    ccour=C->chaines;
    while (ccour!=NULL){
        pcour=ccour->points;
        SVGlineRandColor(&svg);
        SVGpoint(&svg,500*(pcour->x-minx)/(maxx-minx),500*(pcour->y-miny)/(maxy-miny)); 
        precx=pcour->x;
        precy=pcour->y;  
        pcour=pcour->suiv;
        while (pcour!=NULL){
            SVGline(&svg,500*(precx-minx)/(maxx-minx),500*(precy-miny)/(maxy-miny),500*(pcour->x-minx)/(maxx-minx),500*(pcour->y-miny)/(maxy-miny));
            SVGpoint(&svg,500*(pcour->x-minx)/(maxx-minx),500*(pcour->y-miny)/(maxy-miny));
            precx=pcour->x;
            precy=pcour->y;    
            pcour=pcour->suiv;
        }
        ccour=ccour->suiv;
    }
    SVGfinalize(&svg);
}
// Fonction pour compter le nombre total de points
int comptePointsTotal(Chaines *C) {
    int totalPoints = 0;
    CellChaine *chaine = C->chaines;
    CellPoint *point = NULL;

    // Parcours de toutes les chaînes
    while (chaine != NULL) {
        // Parcours de tous les points de la chaîne actuelle
        point = chaine->points;
        printf("%d\n", chaine->numero);
        while (point != NULL) {
            totalPoints++;
            point = point->suiv;
        }
        chaine = chaine->suiv;
    }

    return totalPoints;
}
Chaines* generationAleatoire(int nbChaines, int nbPointsChaine, int xmax, int ymax) {
    // Allocation de mémoire pour la structure Chaines
    Chaines* C = (Chaines*)malloc(sizeof(Chaines));
    if (C == NULL) {
        fprintf(stderr, "Erreur d'allocation de mémoire pour Chaines\n");
        exit(1);
    }

    // Initialisation des champs de la structure Chaines
    C->gamma = 0; // Supposant que gamma n'est pas pertinent pour les chaînes générées aléatoirement
    C->nbChaines = nbChaines;
    C->chaines = NULL;

    // Boucle pour générer chaque chaîne
    for (int i = 0; i < nbChaines; ++i) {
        // Allocation de mémoire pour une nouvelle CellChaine
        CellChaine* nouvelleChaine = (CellChaine*)malloc(sizeof(CellChaine));
        if (nouvelleChaine == NULL) {
            fprintf(stderr, "Erreur d'allocation de mémoire pour CellChaine\n");
            exit(1);
        }
        nouvelleChaine->points = NULL;

        // Boucle pour générer chaque point de la chaîne
        for (int j = 0; j < nbPointsChaine; ++j) {
            // Génération de coordonnées aléatoires pour chaque point dans les limites xmax et ymax
            double x = ((double)rand() / RAND_MAX) * xmax; // Génère une coordonnée x aléatoire
            double y = ((double)rand() / RAND_MAX) * ymax; // Génère une coordonnée y aléatoire

            // Insertion du point généré dans la chaîne
            nouvelleChaine->points = insererCellPoint(nouvelleChaine->points, x, y);
        }

        // Insertion de la nouvelle chaîne dans la structure Chaines
        insererNouvelleChaine(C, nouvelleChaine, i);
    }

    return C;
}