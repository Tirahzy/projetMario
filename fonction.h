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

typedef struct {
    int x;
    int y;
    int w;
    int h;
    int est_touche;
}Plante;

typedef struct {
    int x;
    int y;
    int w;
    int h;
    int est_touche;
}Brique;

typedef struct {
    int x;
    int y;
    int visible;
}Piece;

void initialiserMario(Mario *mario);
void deplacement(Mario *mario, char direction);
void sauter(Mario *mario);
void collisionPlante(Mario *mario, Plante *plante);
void collision_brique(Mario *mario, Brique *brique);
void collisionAvecBrique(Mario *mario, Brique *brique, Piece *piece);
void gererPiece(Mario *mario, Brique *brique, Piece *piece);
































#endif //FONCTION_H
