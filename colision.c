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
     if (tileX < 0  && tileX >= 200 && tileY < 0 && tileY >= 10) return 1;
     return map[tileY][tileX] == '#'  map[tileY][tileX] == '=';
}

int collisionPlante(Mario *mario, Plante *plante)
  {
      if (mario->x < plante->x && mario->x + mario->w > plante->x &&  //condition de collision entre deux rectangles
          mario->y < plante->y && mario->y + mario->h > plante->y) {
          plante->est_touche = 1;
          return 0;
      } else {
          return 1;
      }
  }




void verifierCollectePieces(Mario *mario, char grille[20][40]) {
    int y = 19 - mario->y;
    int x = mario->x;
    if (grille[y][x] == 'O') {
        mario->pieces++;
        mario->points += 100;
        grille[y][x] = '.';
        if (mario->pieces % 100 == 0 && mario->pieces > 0) {
            mario->vie++;
        }

    }
}



