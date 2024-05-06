#ifndef __ARBRE_QUAT_H__
#define __ARBRE_QUAT_H__
#include "Reseau.h"
#include <stdlib.h>
#include <stdio.h>

typedef struct arbreQuat{
    double xc, yc;
    double coteX;
    double coteY;
    Noeud* noeud;
    struct arbreQuat *so;
    struct arbreQuat *se;
    struct arbreQuat *no;
    struct arbreQuat *ne;
}ArbreQuat;


#endif

void chaineCoordMinMax(Chaines* C, double* xmin, double* ymin, double* xmax, double *ymax);
ArbreQuat* creerArbreQuat(double xc, double yc, double coteX, double coteY);
void insererNoeudArbre(Noeud* n, ArbreQuat** a, ArbreQuat* parent);
void liberer_ArbreQuat(ArbreQuat *abr);