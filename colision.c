#include "fonction.h"

 bool collisionPlante(Mario *mario, Plante *plante)
  {
      if (mario->x < plante->x && mario->x + mario->w > plante->x &&  //condition de collision entre deux rectangles
          mario->y < plante->y && mario->y + mario->h > plante->y) {
          plante->est_touche = 1;
          return true;
      } else {
          return false;
      }
  }

 bool collision_brique(Mario *mario, Brique *brique) {
      if (mario->x + mario->w > brique->x &&                        // meme logique
          mario->x < brique->x + brique->w &&
          mario->y + mario->h > brique->y &&
          mario->y < brique->y + brique->h) {
          brique->est_touche = 1;
          return true;
      }
      return false;
  } */

 void collisionAvecBrique(Mario *mario, Brique *brique, Piece *piece) {
    if (brique == NULL) {    // Verifie si la brique est valide
        return;
    }
    if (collision_brique(mario, brique)) {  // Verifie la collision et met à jour `est_touche`
        gererPiece(mario, brique, piece);
    }
} 


 void gererPiece(Mario *mario, Brique *brique, Piece *piece) {
    if (brique->est_touche) {
        if (!piece->visible) {
            piece->x = brique->x + brique->w / 2; // Position au centre de la brique
            piece->y = brique->y - 10;           // Au-dessus de la brique
            piece->visible = 1;                  // Piece devient visible
        }
        mario->points += 50;
        mario->pieces++;
    }
}