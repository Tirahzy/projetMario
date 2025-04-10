#ifndef FONCTION_H
#define FONCTION_H
#include <stdlib.h>
#include <stdio.h>

typedef struct {
    int x;
    int y;
    int w;
    int h;
    int vitesse;
    int sauter;
    int contact;
    int vie;
    int points;
    int pieces;
} Mario;



void clearScreen();
void initialiserMario(Mario *mario);
void afficherGrille(char grille[20][40], Mario *mario);
void initialiserGrille(char grille[20][40]);
void deplacement(Mario *mario, char direction);
void miseAJourSaut(Mario *mario);
void sauter(Mario *mario);
void verifierCollectePieces(Mario *mario, char grille[20][40]);
void verifierPlantes(Mario *mario, char grille[20][40]);
void genererPieces(Mario *mario,char grille[20][40]);

#endif






























