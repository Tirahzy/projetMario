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
void afficherMap(Mario *mario, char map[10][200]);
void deplacement(Mario *mario, char direction);
void miseAJourSaut(Mario *mario);
void sauter(Mario *mario);
void verifierCollectePieces(Mario *mario, char map[10][200]);
void verifierPlantes(Mario *mario, char map[10][200]);
void afficherInfo(Mario *mario);


#endif






























