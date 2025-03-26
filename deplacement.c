#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include <unistd.h>

#include "fonction.h"

#define SOL 0  // Ligne du sol (hauteur fixe)

void initialiserMario(Mario *mario) {
    mario->x = 0;
    mario->y = SOL;
    mario->sauter = 0;
    mario->contact = 1;
}

void initialiserGrille(char grille[20][40]) {
    for (int y = 0; y < 20; y++) {
        for (int x = 0; x < 40; x++) {
            if (y == 19) {
                grille[y][x] = '_';
            } else {
                grille[y][x] = '.';
            }
        }
    }
}

void afficherGrille(char grille[20][40], Mario *mario) {
    system("cls"); // Nettoyer l'écran
    for (int y = 0; y < 20; y++) {
        for (int x = 0; x < 40; x++) {
            if (x == mario->x && y == 19 - mario->y) {
                printf("@");
            } else {
                printf("%c", grille[y][x]);
            }
        }
        printf("\n");
    }
}

void deplacement(Mario *mario, char direction) {
    switch (direction) {
        case 'q':
            if (mario->x > 0) {
                mario->x -= 1;
            }
            break;
        case 'd':
            if (mario->x < 39) {
                mario->x += 1;
            }
            break;
        case 'z':
            sauter(mario);
            break;
        default:
            break;
    }
}

void sauter(Mario *mario) {
    if (mario->contact == 1) {
        mario->sauter = 2;
        mario->contact = 0;
    }
}

void miseAJourSaut(Mario *mario) {
    if (mario->sauter > 0) {
        mario->y += 1;
        mario->sauter--;
    } else if (mario->y > SOL) {
        mario->y -= 1;
    } else {
        mario->contact = 1;
    }
}

void clearScreen() {
    printf("\033[H\033[J"); // Déplace le curseur en haut à gauche et efface l'écran
}


int main() {
    Mario mario;
    char grille[20][40];
    char input;

    initialiserMario(&mario);
    initialiserGrille(grille);

    printf("Bienvenue dans Mario simplifié !\n");
    printf("Commandes :\n");
    printf("Z : Sauter\n");
    printf("Q : Gauche\n");
    printf("D : Droite\n");
    printf("Espace : Quitter\n");
    Sleep(2000);
    system("cls");

    while (1) {
        clearScreen(); // nettoyer ecran
        afficherGrille(grille, &mario);

        if (_kbhit()) {
            input = _getch();
            if (input == 32) {
                printf("Merci d'avoir joué !\n");
                break;
            }
            deplacement(&mario, input);
        }

        miseAJourSaut(&mario);
        Sleep(50);
    }
    return 0;
}



/* bool collisionPlante(Mario *mario, Plante *plante)
  {
      if (mario->x < plante->x && mario->x + mario->w > plante->x &&  //condition de collision entre deux rectangles
          mario->y < plante->y && mario->y + mario->h > plante->y) {
          plante->est_touche = 1;
          return true;
      } else {
          return false;
      }
  } /*

/* bool collision_brique(Mario *mario, Brique *brique) {
      if (mario->x + mario->w > brique->x &&                        // meme logique
          mario->x < brique->x + brique->w &&
          mario->y + mario->h > brique->y &&
          mario->y < brique->y + brique->h) {
          brique->est_touche = 1;
          return true;
      }
      return false;
  } */

/* void collisionAvecBrique(Mario *mario, Brique *brique, Piece *piece) {
    if (brique == NULL) {    // Verifie si la brique est valide
        return;
    }
    if (collision_brique(mario, brique)) {  // Verifie la collision et met à jour `est_touche`
        gererPiece(mario, brique, piece);
    }
} */


/* void gererPiece(Mario *mario, Brique *brique, Piece *piece) {
    if (brique->est_touche) {
        if (!piece->visible) {
            piece->x = brique->x + brique->w / 2; // Position au centre de la brique
            piece->y = brique->y - 10;           // Au-dessus de la brique
            piece->visible = 1;                  // Piece devient visible
        }
        mario->points += 50;
        mario->pieces++;
    }
} */




