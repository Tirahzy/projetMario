#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include <unistd.h>

#include "fonction.h"

#define SOL 0  // Ligne du sol (hauteur fixe)

//initialisation de mario
void initialiserMario(Mario *mario) {
    mario->x = 0;
    mario->y = SOL;
    mario->sauter = 0;
    mario->contact = 1;
    mario->points = 0;
    mario->pieces = 0;
    mario->vie = 3;
}

//initialisation de la grille 20x40
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
    grille[18][5] = 'O';
    grille[18][15] = 'O';
    grille[18][25] = 'O';
    grille[19][10] = 'P';
    grille[19][20] = 'P';
    grille[19][30] = 'P';
}

//afficher la grille
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
    printf("Score: %d \n", mario->points);
    printf("Pieces: %d \n", mario->pieces);
    printf(" Vie : %d \n", mario->vie);
}

// deplacement de mario
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

// sauter
void sauter(Mario *mario) {
    if (mario->contact == 1) {
        mario->sauter = 2;
        mario->contact = 0;
    }
}

// mise a jour du saut
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

// fin du jeu
void gameOver(Mario *mario) {
    system("cls");
    printf("\n Game Over \n");
    printf(" Score final : %d \n", mario->points);
    printf(" Appuie sur une touche pour quitter \n");
    _getch();
    exit(0);
}

//verification et compteur des pieces/scores
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

//verification collision plante et compteur de vie
void verifierPlantes(Mario *mario, char grille[20][40]) {
    int y = 19 - mario->y;
    int x = mario->x;
    if (grille[y][x] == 'P') {
        mario->vie--;
        if (mario->vie <= 0) {
            gameOver(mario);
        }
    }
}

//l'écran se rafraichis
void clearScreen() {
    printf("\033[H\033[J"); // Déplace le curseur en haut à gauche et efface l'écran
}


int main() {
    Mario mario;
    char grille[20][40];
    char input;

    initialiserMario(&mario);
    initialiserGrille(grille);

    printf("Bienvenue dans Mario simplifie !\n");
    printf("Commandes :\n");
    printf("Z : Sauter\n");
    printf("D : Droite\n");
    printf("Q : Gauche\n");
    printf("Espace : Quitter\n");
    Sleep(2000);
    system("cls");

    while (1) {
        clearScreen(); // nettoyer ecran
        afficherGrille(grille, &mario);

        if (_kbhit()) {
            input = _getch();
            if (input == 32) {
                printf("Merci d'avoir joué ! \n");
                break;
            }
            deplacement(&mario, input);
        }

        miseAJourSaut(&mario);
        verifierCollectePieces(&mario, grille);
        verifierPlantes(&mario, grille);
        Sleep(50);
    }
    return 0;
}
