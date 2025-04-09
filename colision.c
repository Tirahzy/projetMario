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

int collision_brique(Mario *mario, Brique *brique) {
      if (mario->x + mario->w > brique->x &&                        // meme logique
          mario->x < brique->x + brique->w &&
          mario->y + mario->h > brique->y &&
          mario->y < brique->y + brique->h) {
          brique->est_touche = 1;
          return 0;
      }
      return 1;
  }




 void genererPiece(Piece *piece, int x, int y) {
        piece->x = x;
        piece->y = y;
        piece->w = 32; // Largeur de la pièce
        piece->h = 32; // Hauteur de la pièce
        piece->visible = 1; // La pièce est visible au départ
        piece->est_touche = 0; // La pièce n'est pas touchée au départ
    }

    void collisionPiece(Mario *mario, Piece *piece) {
        if (piece->visible && mario->x < piece->x + piece->w && mario->x + mario->w > piece->x &&
            mario->y < piece->y + piece->h && mario->y + mario->h > piece->y) {
            piece->visible = 0; // Piece collectée
            mario->points += 50;
            mario->pieces++;
        }
    }




