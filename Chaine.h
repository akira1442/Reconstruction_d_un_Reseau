#ifndef __CHAINE_H__
#define __CHAINE_H__	
#include <stdio.h>

/* Liste chainee de points */
typedef struct cellPoint{
  double x,y;                   /* Coordonnees du point */
  struct cellPoint *suiv;       /* Cellule suivante dans la liste */
} CellPoint;

/* Celllule d une liste (chainee) de chaines */
typedef struct cellChaine{
  int numero;                   /* Numero de la chaine */
  CellPoint *points;            /* Liste des points de la chaine */
  struct cellChaine *suiv;      /* Cellule suivante dans la liste */
} CellChaine;

/* L'ensemble des chaines */
typedef struct {
  int gamma;                    /* Nombre maximal de fibres par cable */
  int nbChaines;                /* Nombre de chaines */
  CellChaine *chaines;          /* La liste chainee des chaines */
} Chaines;

Chaines* creerChaine(int gamma, int nbChaines, CellChaine* cc);
CellChaine* creerCellChaine(int num, CellPoint* cp);
void insererNouvelleChaine(Chaines* C, CellChaine* nouvelleChaine, int numero);
CellPoint* creerCellPoint(float x, float y);
CellChaine* insererCellChaine(CellChaine* cc, CellPoint* cp, int numero);
CellPoint* insererCellPoint(CellPoint* cp, float x, float y);
void libererCellChaine(CellChaine* cc);
void libererCellPoint(CellPoint* cp);
void libererChaine(Chaines* C);
Chaines* lectureChaines(FILE *f);
void ecrireChaines(Chaines *C, FILE *f);
void afficheChainesSVG(Chaines *C, char* nomInstance);
double longueurTotale(Chaines *C);
int comptePointsTotal(Chaines *C);
Chaines* generationAleatoire(int nbChaines, int nbPointsChaine, int xmax, int ymax);

#endif	
