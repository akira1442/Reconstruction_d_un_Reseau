#include "Arbre_Quat.h"

void chaineCoordMinMax(Chaines* C, double* xmin, double* ymin, double* xmax, double *ymax){

    CellChaine* cc = C->chaines;
    CellPoint* cp = NULL;

    *xmin = cc->points->x;
    *xmax = cc->points->x;
    *ymin = cc->points->y;
    *ymax = cc->points->y;

    while (cc){
        cp = cc->points;
        while (cp){
            if (cp->x > *xmin){
                cp->x = *xmin;
            }
            if (cp->x < *xmax){
                cp->x = *xmax;
            }
            if (cp->y > *ymin){
                cp->y = *ymin;
            }
            if (cp->y < *ymax){
                cp->y = *ymax;
            }
            cp = cp->suiv;
        }
        cc = cc->suiv;
    }
}

ArbreQuat* creerArbreQuat(double xc, double yc, double coteX, double coteY){

    ArbreQuat* new_node = (ArbreQuat*) malloc(sizeof(ArbreQuat));

    new_node->xc = xc;
    new_node->yc = yc;
    new_node->coteX = coteX;
    new_node->coteY = coteY;
    new_node->ne = NULL;
    new_node->no = NULL;
    new_node->se = NULL;
    new_node->so = NULL;
    new_node->noeud = NULL;
    
    return new_node;
}

void insererNoeudArbre(Noeud* n, ArbreQuat** a, ArbreQuat* parent){

    //Arbre vide
    if (*a == NULL){
        double xc, yc, coteX, coteY;

        if ((n->x >= parent->xc) && (n->y >= parent->yc)){ //Nord Est
            //calcule de coteX et coteY
            coteX = parent->coteX / 2;
            coteY = parent->coteY / 2;

            //calcule du centre du sous-arbre
            xc = coteX / 2;
            yc = coteY / 2;

            //insertion du sous-arbre
            (*a) = creerArbreQuat(xc, yc, coteX, coteY);
            (*a)->noeud = n;
            parent->ne = *a;
        }else if ((n->x < parent->xc) && (n->y >= parent->yc)){ //Nord Ouest
            //calcule de coteX et coteY
            coteX = parent->coteX / 2;
            coteY = parent->coteY / 2;

            //calcule du centre du sous-arbre
            xc = coteX / 2;
            yc = coteY / 2;

            //insertion du sous-arbre
            (*a) = creerArbreQuat(xc, yc, coteX, coteY);
            (*a)->noeud = n;
            parent->no = *a;
        }else if ((n->x >= parent->xc) && (n->y < parent->yc)){ //Sud Est
            //calcule de coteX et coteY
            coteX = parent->coteX / 2;
            coteY = parent->coteY / 2;

            //calcule du centre du sous-arbre
            xc = coteX / 2;
            yc = coteY / 2;

            //insertion du sous-arbre
            (*a) = creerArbreQuat(xc, yc, coteX, coteY);
            (*a)->noeud = n;
            parent->se = *a;
        }else if ((n->x < parent->xc) && (n->y < parent->yc)){ //Sud Ouest
            //calcule de coteX et coteY
            coteX = parent->coteX / 2;
            coteY = parent->coteY / 2;

            //calcule du centre du sous-arbre
            xc = coteX / 2;
            yc = coteY / 2;

            //insertion du sous-arbre
            (*a) = creerArbreQuat(xc, yc, coteX, coteY);
            (*a)->noeud = n;
            parent->so = *a;
        }
    }
    //Feuille de l'arbre
    else if ((*a)->noeud != NULL){
        Noeud* tmp = (*a)->noeud;
        insererNoeudArbre(tmp, NULL, (*a));
        (*a)->noeud = NULL;
        if ((n->x >= parent->xc) && (n->y >= parent->yc)){ //Nord Est
            (*a)->ne->noeud = n;
        }else if ((n->x < parent->xc) && (n->y >= parent->yc)){ //Nord Ouest
            (*a)->no->noeud = n;
        }else if ((n->x >= parent->xc) && (n->y < parent->yc)){ //Sud Est
            (*a)->se->noeud = n;
        }else if ((n->x < parent->xc) && (n->y < parent->yc)){ //Sud Ouest
            (*a)->so->noeud = n;
        }
    }
    //Cellule interne de l'arbre
    else if (((*a) != NULL) && ((*a)->noeud == NULL)){
        if ((n->x >= (*a)->xc) && (n->y >= (*a)->yc)){ //Nord Est
            insererNoeudArbre(n, &(*a)->ne, (*a));
        }else if ((n->x < (*a)->xc) && (n->y >= (*a)->yc)){ //Nord Ouest
            insererNoeudArbre(n, &(*a)->no, (*a));
        }else if ((n->x >= (*a)->xc) && (n->y < (*a)->yc)){ //Sud Est
            insererNoeudArbre(n, &(*a)->se, (*a));
        }else if ((n->x < (*a)->xc) && (n->y < (*a)->yc)){ //Sud Ouest
            insererNoeudArbre(n, &(*a)->so, (*a));
        }
    }
}

Noeud* rechercheCreeNoeudArbre(Reseau* R, ArbreQuat** a, ArbreQuat* parent, double x, double y){
    
    //Arbre NULL
    if (*a == NULL){
        Noeud* new_node = creerNoeud(R->nbNoeuds+1, x, y);
        CellNoeud* cn = (CellNoeud*) malloc(sizeof(CellNoeud));

        cn->nd = new_node;
        cn->suiv = R->noeuds;
        R->noeuds = cn;
        R->nbNoeuds++;
        insererNoeudArbre(new_node, a, parent);
        return new_node;
    }
    //Feuille de l'arbre
    else if ((*a)->noeud != NULL){
        if ((x == (*a)->xc) && (y == (*a)->yc)){
            return (*a)->noeud;
        }
    }
    //Cellule interne
    else if (((*a)->noeud == NULL) && (*a != NULL)){
        if ((x >= (*a)->xc) && (y >= (*a)->yc)){ //Nord Est
            rechercheCreeNoeudArbre(R, &(*a)->ne, (*a), x, y);
        }else if ((x < (*a)->xc) && (y >= (*a)->yc)){ //Nord Ouest
            rechercheCreeNoeudArbre(R, &(*a)->no, (*a), x, y);
        }else if ((x >= (*a)->xc) && (y < (*a)->yc), x, y){ //Sud Est
            rechercheCreeNoeudArbre(R, &(*a)->se, (*a), x, y);
        }else if ((x < (*a)->xc) && (y < (*a)->yc)){ //Sud Ouest
            rechercheCreeNoeudArbre(R, &(*a)->so, (*a), x, y);
        }
    }
}

Reseau* reconstitueReseauArbre(Chaines* C){
    
    double xmin, xmax, ymin, ymax;
    chaineCoordMinMax(C, &xmin, &ymin, &xmax, &ymax);
    double coteX = xmax - xmin;
    double coteY = ymax - ymin;

    ArbreQuat *aq = creerArbreQuat(xmin+(coteX/2), ymin+(coteY/2), coteX, coteY);
    Reseau *R = creerReseau(C->gamma);

    CellChaine * cc = C->chaines;
    
    while (cc != NULL){     //On parcourt toutes les CellChaine de la Chaine
        CellPoint * cp = cc->points;

        Noeud* node_head = rechercheCreeNoeudArbre(R, &aq, NULL, cp->x, cp->y);  //nDebut et nFin permettent de creer la commodite de cette CellChaine
        Noeud* node_tail;

        while(cp != NULL){      //On parcourt tous les points de la CellChaine
            CellPoint* cp2 = cc->points;
            Noeud * n = rechercheCreeNoeudArbre(R, &aq, NULL, cp->x, cp->y);  //On prend le noeud, soit il existe soit on le creee
            
            while (cp2 != NULL){    //On parcourt une seconde fois les points de la CellChaine afin de creer les voisins des points
                if (cp2->suiv == cp){
                    Noeud * n2 = rechercheCreeNoeudArbre(R, &aq, NULL, cp2->x, cp2->y);
                    inserer_voisin(n, n2);
                    inserer_voisin(n2, n);
                }
                cp2 = cp2->suiv;
            }
            node_tail = rechercheCreeNoeudArbre(R, &aq, NULL, cp->x, cp->y);
            cp = cp->suiv;
        }
        CellCommodite * cm = creerCellCommodite(node_head, node_tail);  //On creee la CellCommodite
        insererCommodite(R, cm);

        cc = cc->suiv;
    }
    liberer_ArbreQuat(aq);

    return R;
}

void liberer_ArbreQuat(ArbreQuat *abr) {
    
    if (abr == NULL) {
        return;
    }
    else if (abr->noeud == NULL) {
        if (abr->ne != NULL) {
            liberer_ArbreQuat(abr->ne);
            free(abr->ne);
            free(abr->no);
            free(abr->se);
            free(abr->so);
        }
        if (abr->no != NULL) {
            liberer_ArbreQuat(abr->no);
            free(abr->ne);
            free(abr->no);
            free(abr->se);
            free(abr->so);
        }
        if (abr->se != NULL) {
            liberer_ArbreQuat(abr->se);
            free(abr->ne);
            free(abr->no);
            free(abr->se);
            free(abr->so);
        }
        if (abr->so != NULL) {
            liberer_ArbreQuat(abr->so);
            free(abr->ne);
            free(abr->no);
            free(abr->se);
            free(abr->so);
        }
        free(abr);
    }
    
}