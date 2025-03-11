#include "fonction.h"
void deplacement(Mario *mario, char direction) {
    switch (direction) {
        case 'q':
            mario->x -= mario->vitesse;
        break;
        case 'd':
            mario->x += mario->vitesse;
        break;
        case 'z':
            sauter(mario);
            break;
        break;
        case 's':
            if(mario->sauter) {
                mario->y += mario->vitesse/2;    // descends mario
                if (mario->y >= 0) {    //evite de descendre en dessous du sol
                    mario->sauter = 0;  // fin du saut
                }
            }
        break;
        default:
            break;
    }
}
void sauter(Mario *mario) {
    if (!mario->sauter) {
        mario->sauter = 1;
        mario->y -= mario->vitesse;
    }

}

bool collisionPlante(Mario *mario, Plante *plante)
{
    if (mario->x == plante->x && mario->y == plante->y)
    {
        plante->est_touche = 1;
        return true;
    } else {
        return false;
    }
}

bool collision_brique(Mario *mario, Brique *brique) {
    if (mario->x + mario->w > brique->x && mario->x < brique->x + brique->w && mario->y + mario->h > brique->y && mario->y < brique->y + brique->h) {
        brique->est_touche = 1;
        return true;
    } else {
        return false;
    }
}
















