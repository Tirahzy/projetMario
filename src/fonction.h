#ifndef FONCTION_H
#define FONCTION_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define LONGUEUR_FENETRE 960 // Taille de la fenetre en pixels
#define LARGEUR_FENETRE 540

#define MAP_LARGEUR 200 // Taille de la map en blocs
#define MAP_HAUTEUR 18

#define BLOC_SIZE 30 // Taille d'un bloc en pixels

// Variables pour la jouabilité du personnages (saut et vitesse)
#define GRAVITE 0.5f
#define FORCE_SAUT -11.0f
#define VITESSE_DEPLACEMENT 5

// Associations entre les caractères de la map et les blocs
#define BLOC 1
#define BLOC_SOL 2
#define BLOC_STRUCTURE 3
#define BLOC_RECOMPENSE 4
#define TUYAU_HAUT_GAUCHE 5
#define TUYAU_HAUT_DROITE 6
#define TUYAU_BAS_GAUCHE 7
#define TUYAU_BAS_DROITE 8
#define PIECE 9
#define TOAD 10
#define ENNEMI 11

// Variables ennemis
#define GOOMBA 0
#define KOOPA 1
#define CARAPACE 2
#define ENNEMI_CARAPACE_IMMOBILE 1
#define ENNEMI_CARAPACE_MOBILE 2
#define GAUCHE -1
#define DROITE 1

#define NOMBRE_NIVEAUX 9

#define MAX_ENNEMIS 20

// Fenetre afficher en jeu
#define ETAT_MENU 0
#define ETAT_JEU 1
#define ETAT_GAME_OVER 2
#define ETAT_NIVEAU_TERMINE 3
#define ETAT_SELECTION 4

#define NB_BOUTONS_MENU 3

#define FICHIER_SAUVEGARDE "sauvegarde.dat"

typedef struct
{
    SDL_Texture *bloc;
    SDL_Texture *blocSol;
    SDL_Texture *blocStructure;
    SDL_Texture *blocRecompense;

    SDL_Texture *tuyauHaut1;
    SDL_Texture *tuyauHaut2;
    SDL_Texture *tuyauBas1;
    SDL_Texture *tuyauBas2;

    SDL_Texture *goomba1;
    SDL_Texture *goomba2;
    SDL_Texture *goombaMort;
    SDL_Texture *koopa1;
    SDL_Texture *koopa2;
    SDL_Texture *carapace1;
    SDL_Texture *carapace2;

    SDL_Texture *piece;
    SDL_Texture *champignon;

    SDL_Texture *mario;
    SDL_Texture *marioBig;
    SDL_Texture *toad;

    SDL_Texture *background;

    SDL_Texture *vie;
} TexturesJeu;

typedef struct
{
    int gauche;
    int droite;
    int saut;
} Touches;

typedef struct
{
    SDL_Rect rect;
    int actif;
    int type;
    int direction;
} Ennemi;

typedef struct
{
    SDL_Rect rect;
    int duree;
    int actif;
} Effet;

typedef struct
{
    SDL_Rect rect;
    int vitesse;
    int actif;
    int direction;
    int mobile;
    int tempsLancement;
} Carapace;

typedef struct
{
    SDL_Rect corps;
    int actif;
    float vitesseY;
    float vitesseX;
    int direction;
} Champignon;

typedef struct
{
    SDL_Rect corps;
    int estGrand;
    int invincible;      // 1 = invincible, 0 = normal
    int tempsInvincible; // temps en ms où il est devenu invincible
} Mario;

typedef struct
{
    SDL_Rect rect;
    char *texte;
    int hover;
} Bouton;

typedef struct
{
    int score;
    int vies;
    int scoreTotal;
} ScoreJeu;

typedef struct
{
    int niveau;
} Sauvegarde;

// Tableau de caractères pour les niveaux (fichier map.c)
extern char niveau1[18][200];
extern char niveau2[18][200];
extern char niveau3[18][200];
extern char niveau4[18][200];
extern char niveau5[18][200];
extern char niveau6[18][200];
extern char niveau7[18][200];
extern char niveau8[18][200];
extern char niveau9[18][200];

extern int map[MAP_HAUTEUR][MAP_LARGEUR];

extern Ennemi ennemis[MAX_ENNEMIS];
extern Effet effets[MAX_ENNEMIS];
extern Carapace carapaces[MAX_ENNEMIS];

SDL_Window *creerFenetre(char nom[]);
SDL_Renderer *creerRenderer(SDL_Window *fenetre);
void dessinerCarre(SDL_Renderer *renderer, SDL_Rect carre);

SDL_Texture *chargerTextureBMP(SDL_Renderer *renderer, char *chemin);
TexturesJeu chargerTextures(SDL_Renderer *renderer);
void dessinerTexture(SDL_Renderer *renderer, SDL_Texture *texture, int x, int y, int w, int h, int cameraX);
void libererTextures(TexturesJeu textures);

void gererTouches(int *continuer, SDL_Rect *carre, int *enSaut, float *vitesseSaut, Touches *touches);
int detecterCollision(SDL_Rect joueur);

void initialiserMap(int niveau);
void initialiserEnnemis(char niveau[][MAP_LARGEUR]);
void dessinerMap(SDL_Renderer *renderer, int cameraX, TexturesJeu textures);
void dessinerEnnemis(SDL_Renderer *renderer, int cameraX, TexturesJeu textures);

void mettreAJourEnnemis();
int detecterCollisionEntreEnnemis(SDL_Rect ennemi, int indexEnnemi);

int detecterCollisionEnnemi(SDL_Rect joueur);
int sauterSurEnnemi(SDL_Rect joueur, float vitesseSaut, ScoreJeu *scoreData);

void initialiserEffets();
void ajouterEffetEcrasement(int x, int y);
void mettreAJourEffets();
void dessinerEffets(SDL_Renderer *renderer, int cameraX, TexturesJeu textures);

void initialiserCarapaces();
void mettreAJourCarapaces();
int interagirAvecCarapaces(SDL_Rect *joueur, float *vitesseSaut);
void carapacesTuantEnnemis();
void dessinerCarapaces(SDL_Renderer *renderer, int cameraX, TexturesJeu textures);

void initialiserBoutons(Bouton boutons[], int nombreBoutons, const char *labels[]);
void dessinerBoutons(SDL_Renderer *renderer, Bouton boutons[], int nombreBoutons, TTF_Font *police);
int pointDansRect(int x, int y, SDL_Rect rect);

int gererEvenementsMenu(int *continuer, Bouton boutons[], int nombreBoutons);

int finDeNiveau(SDL_Rect joueur);
int gererEvenementsNiveauTermine(int *continuer, Bouton boutons[], int nombreBoutons);

void afficherMonde2(SDL_Renderer *renderer, TTF_Font *police);
void afficherMonde3(SDL_Renderer *renderer, TTF_Font *police);
void afficherEcranFin(SDL_Renderer *renderer, TTF_Font *police);

int gererGameOver(int *continuer, Bouton boutons[], int nombreBoutons);

void dessinerFondParallaxe(SDL_Renderer *renderer, SDL_Texture *texture, int cameraX);

int detecterCollisionBlocMystere(SDL_Rect joueur, float vitesseSaut);
void ChampignonSiBlocMystereTouche(SDL_Rect joueur, SDL_Rect *champignon, float vitesseSaut);

void afficherScore(SDL_Renderer *renderer, ScoreJeu *scoreJeu, TTF_Font *police);
void afficherVies(SDL_Renderer *renderer, ScoreJeu *scoreJeu, TexturesJeu textures);

void sauvegarderPartie(int niveau);
int chargerPartie(int *niveau);

#endif