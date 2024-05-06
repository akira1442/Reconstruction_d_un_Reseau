#include "Chaine.h"
#include "ArbreQuat.h"
#include "Hachage.h"
#include "SVGwriter.h"
#include <time.h>
#include <stdlib.h>
#include <stdio.h>


int main(){

    srand(time(NULL));
    
    FILE* f = fopen("05000_USA-road-d-NY.cha", "r");
    Chaines* c = lectureChaines(f);

    clock_t clock_init, clock_final;
    
    // Liste Chainée 
    clock_init = clock();
    Reseau* Rl = reconstitueReseauListe(c);
    clock_final = clock();
    printf("temps de calcule pour reconstitueReseauListe(): %f seconde(s)\n", (float)(clock_final - clock_init) / CLOCKS_PER_SEC);
    libererReseau(Rl);

    // Table de Hachage
    int M = 5, i = 0;
    Reseau* Rh = NULL;
    
    while (i < 3){
        clock_init = clock();
        Rh = reconstitueReseauHachage(c, M);
        clock_final = clock();
        printf("temps de calcule pour reconstitueReseauHachage() pour une table de taille %d: %f\n", M, (float)(clock_final - clock_init) / CLOCKS_PER_SEC);
        M = (int)((rand() % 15) + 1);
        i++;
        libererReseau(Rh);
    }

    // Arbre Quaternaire

    clock_init = clock();
    Reseau* Ra = reconstitueReseauArbre(c);
    clock_final = clock();
    printf("temp de calcule pour reconstitueReseauArbre(): %f\n", (float)(clock_final - clock_init) / CLOCKS_PER_SEC);
    
    libererReseau(Ra);

    libererChaine(c);
    
    fclose(f);
    return 1;
}