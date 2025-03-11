#include "fonction.h"
void deplacement(Mario *mario, char direction) {
    switch (direction) {
        case 'g':
            mario->x -= mario->vitesse;
        break;
        case 'd':
            mario->x += mario->vitesse;
        break;
        case 'h':
            mario->y -= mario->vitesse;
        break;
        case 'b':
            mario->y += mario->vitesse;
        break;
        default:
            break;
    }
}