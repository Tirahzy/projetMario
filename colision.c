#include "fonction.h"

#define LARGEUR_FENETRE 640
#define HAUTEUR_FENETRE 320
#define TAILLE_TILE 32
#define GRAVITE 1
#define VITESSE_X 4
#define IMPULSION_SAUT 14
int estCollision(int x, int y) {
     int tileX = x / TAILLE_TILE;
     int tileY = y / TAILLE_TILE;
     if (tileX < 0  tileX >= 200  tileY < 0  tileY >= 10) return 1;
     return map[tileY][tileX] == '#'  map[tileY][tileX] == '=';
}

int planteCollision( Plante *est_touchee,Mario *mario) {
  est_touchee = malloc(sizeof(Plante));
     if (mario->x + mario->w < est_touchee->x) return 0;
     if (mario->x > est_touchee->x + est_touchee->w) return 0;
     if (mario->y + mario->h < est_touchee->y) return 0;
     if (mario->y > est_touchee->y + est_touchee->h) return 0;
     return 1;

}