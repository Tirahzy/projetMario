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

/* typedef struct {
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
}Piece; */

void initialiserMario(Mario *mario);
void afficherGrille(char grille[20][20], Mario *mario);
void initialiserGrille(char grille[20][20]);
void deplacement(Mario *mario, char direction);
void miseAJourSaut(Mario *mario);
void sauter(Mario *mario);

#endif






























