#CFLAGS = -g -Wall -Wextra -pedantic -Wno-unused-parameter
CFLAGS = -g -Wno-unused-parameter
CC = gcc

PROGRAMS = ChaineMain ReconstitueReseau Main Temps

.PHONY:	all clean

all: $(PROGRAMS)

#Fichier de l'ex 1
ChaineMain: Chaine.o SVGwriter.o ChaineMain.o Reseau.o
	$(CC) -o $@ $(CFLAGS) $^ -lm

ReconstitueReseau: Reseau.o Chaine.o Hachage.o Arbre_Quat.o SVGwriter.o ReconstitueReseau.o
	$(CC) -o $@ $(CFLAGS) $^ -lm

Main: Hachage.o Arbre_Quat.o Reseau.o Main.o SVGwriter.o Chaine.o
	$(CC) -o $@ $(CFLAGS) $^ -lm

Temps: Hachage.o Arbre_Quat.o Reseau.o SVGwriter.o Chaine.o Temps.o
	$(CC) -o $@ $(CFLAGS) $^ -lm

ChaineMain.o: ChaineMain.c
	$(CC) -c $(CFLAGS) ChaineMain.c -lm

ReconstitueReseau.o: ReconstitueReseau.c
	$(CC) -c $(CFLAGS) ReconstitueReseau.c -lm

Chaine.o: SVGwriter.o Chaine.c 
	$(CC) -c $(CFLAGS) Chaine.c -lm

SVGwriter.o: SVGwriter.c
	$(CC) -c $(CFLAGS) SVGwriter.c -lm

Reseau.o: Reseau.c 
	$(CC) -c $(CFLAGS) Reseau.c -lm

Hachage.o: Hachage.c 
	$(CC) -c $(CFLAGS) Hachage.c 

ArbreQuat.o: Arbre_Quat.c
	$(CC) -c $(CFLAGS) Arbre_Quat.c

Main.o: ArbreQuat.c Hachage.c Reseau.c SVGwriter.c Chaine.c main.c
	$(CC) -c $(CFLAGS) main.c ArbreQuat.c Hachage.c Reseau.c Chaine.c SVGwriter.c -lm

Temps.o: ArbreQuat.c Hachage.c Reseau.c SVGwriter.c Chaine.c Temps.c
	$(CC) -c $(CFLAGS) Temps.c ArbreQuat.c Hachage.c Reseau.c Chaine.c SVGwriter.c -lm

clean:
	rm -f *.o *~ *.txt *.html $(PROGRAMS)