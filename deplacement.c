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





