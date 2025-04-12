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

// fonction de la camera de Mario
void afficherMap(Mario *mario, char map[10][200]) {
    system("cls");

    int cameraX = mario->x - 40 / 2;
    if (cameraX < 0) cameraX = 0;
    if (cameraX > 200 - 40) cameraX = 200 - 40;

    for (int y = 0; y < 10; y++) {
        for (int x = cameraX; x < cameraX + 40; x++) {
            if (x == mario->x && y == 10 - 1 - mario->y) {
                printf("@");
            } else {
                printf("%c", map[y][x]);
            }
        }
        printf("\n");
    }
}

// affichage des données a chaque iteration
void afficherInfo(Mario *mario) {
    printf("Score: %d | Pièces: %d | Vie: %d\n", mario->points, mario->pieces, mario->vie);
}

// sauter
void sauter(Mario *mario) {
    if (mario->contact == 1) {
        mario->sauter = 2;
        mario->contact = 0;
    }
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
            if (mario->x < 200 - 1) {
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
    printf("\n\n");
    printf("*******************************\n");
    printf("*         GAME OVER          *\n");
    printf("*                            *\n");
    printf("*  Score final : %5d       *\n", mario->points);
    printf("*  Pieces collectees : %3d   *\n", mario->pieces);
    printf("*                            *\n");
    printf("* Appuie sur une touche pour *\n");
    printf("*         quitter            *\n");
    printf("*******************************\n");
    fflush(stdout);
    Sleep(2000);
    _getch();
    exit(0);
}

//verification et compteur des pieces/scores
void verifierCollectePieces(Mario *mario, char map[10][200]) {
    int y = 9 - mario->y;
    int x = mario->x;

    // Vérification des limites de la map
    if (y >= 0 && y < 10 && x >= 0 && x < 200) {
        if (map[y][x] == 'O') {
            mario->pieces++;
            mario->points += 100;
            map[y][x] = '.';
            if (mario->pieces % 100 == 0 && mario->pieces > 0) {
                mario->vie++;
            }
        }
    }
}

//verification collision plante et compteur de vie
void verifierPlantes(Mario *mario, char map[10][200]) {
    int y = 9 - mario->y;
    int x = mario->x;

    if (y >= 0 && y < 10 && x >= 0 && x < 200) {
        if (map[y][x] == 'P') {
            mario->vie--;
            // Modification : Deplacer Mario un peu en arrière pour eviter de rester sur la plante
            if (mario->x > 0) {
                mario->x -= 2;
            }
            if (mario->vie <= 0) {
                gameOver(mario);
            }
        }
    }
}

//l'écran se rafraichis
void clearScreen() {
    system("cls");
}

int main() {
    Mario mario;
    char input;
    char map[10][200] = {
        "........................................................................................................................................................................................................",
        "........................................................................................................................................................................................................",
        "........................................................................................................................................................................................................",
        "........................................................................................................................................................................................................",
        "........................................................................................................................................................................................................",
        "........................................................................................................................................................................................................",
        "........................................................OOO.............................................................................................................................................",
        "..............................................########..................................................................................................................................................",
        "........................OO..............####............................................................................................................................................................",
        "___________________P______________P______________P___________________P____________________P__________________P___________________P____________________P_________________________________________________"
    };

    initialiserMario(&mario);

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
        afficherMap(&mario, map);
        afficherInfo(&mario);

        if (_kbhit()) {
            input = _getch();
            if (input == 32) {
                printf("Merci d'avoir joué ! \n");
                break;
            }
            deplacement(&mario, input);
        }

        miseAJourSaut(&mario);
        verifierCollectePieces(&mario, map);
        verifierPlantes(&mario, map);

        Sleep(100);
    }
    return 0;
}
