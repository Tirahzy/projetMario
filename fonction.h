#ifndef FONCTION_H
#define FONCTION_H
#include <stdlib.h>
#include <stdio.h>
#include <SDL2/SDL.h>

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



void collionPlante(Mario *mario, Plante *plante);
