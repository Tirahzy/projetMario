#include "fonction.h"

//---------------------------------------------------------
// Fonction initalisation basique de SDL

SDL_Window *creerFenetre(char nom[])
{
    SDL_Window *fenetre = SDL_CreateWindow(nom, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                           LONGUEUR_FENETRE, LARGEUR_FENETRE, SDL_WINDOW_SHOWN);
    if (fenetre == NULL)
    {
        printf("Erreur SDL_CreateWindow\n");
        exit(1);
    }
    return fenetre;
}

SDL_Renderer *creerRenderer(SDL_Window *fenetre)
{
    SDL_Renderer *renderer = SDL_CreateRenderer(fenetre, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL)
    {
        printf("Erreur SDL_CreateRenderer\n");
        exit(1);
    }
    return renderer;
}

void dessinerCarre(SDL_Renderer *renderer, SDL_Rect carre)
{
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderFillRect(renderer, &carre);
}

//---------------------------------------------------------
// Chargement des textures

SDL_Texture *chargerTextureBMP(SDL_Renderer *renderer, char *chemin)
{
    SDL_Surface *surface = SDL_LoadBMP(chemin);
    if (!surface)
    {
        printf("Erreur SDL_LoadBMP\n");
        exit(1);
    }

    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    if (!texture)
    {
        printf("Erreur SDL_CreateTextureFromSurface\n");
        exit(1);
    }

    return texture;
}

TexturesJeu chargerTextures(SDL_Renderer *renderer)
{
    TexturesJeu textures;
    textures.bloc = chargerTextureBMP(renderer, "img/bloc.bmp");
    textures.blocSol = chargerTextureBMP(renderer, "img/sol.bmp");
    textures.blocStructure = chargerTextureBMP(renderer, "img/bloc-structure.bmp");
    textures.blocRecompense = chargerTextureBMP(renderer, "img/bloc-recompense.bmp");

    textures.tuyauHaut1 = chargerTextureBMP(renderer, "img/tuyaux-hg-1.bmp");
    textures.tuyauHaut2 = chargerTextureBMP(renderer, "img/tuyaux-hd-2.bmp");
    textures.tuyauBas1 = chargerTextureBMP(renderer, "img/tuyaux-bg-3.bmp");
    textures.tuyauBas2 = chargerTextureBMP(renderer, "img/tuyaux-bd-4.bmp");

    textures.goomba1 = chargerTextureBMP(renderer, "img/goomba-1.bmp");
    textures.goomba2 = chargerTextureBMP(renderer, "img/goomba-2.bmp");
    textures.goombaMort = chargerTextureBMP(renderer, "img/goomba-mort.bmp");
    textures.koopa1 = chargerTextureBMP(renderer, "img/tortue-1.bmp");
    textures.koopa2 = chargerTextureBMP(renderer, "img/tortue-2.bmp");
    textures.carapace1 = chargerTextureBMP(renderer, "img/carapace-1.bmp");
    textures.carapace2 = chargerTextureBMP(renderer, "img/carapace-2.bmp");

    textures.piece = chargerTextureBMP(renderer, "img/piece.bmp");
    textures.champignon = chargerTextureBMP(renderer, "img/champignon.bmp");
    textures.mario = chargerTextureBMP(renderer, "img/mario.bmp");
    textures.marioBig = chargerTextureBMP(renderer, "img/mario-big.bmp");
    textures.toad = chargerTextureBMP(renderer, "img/toad.bmp");

    textures.background = chargerTextureBMP(renderer, "img/fond.bmp");
    textures.vie = chargerTextureBMP(renderer, "img/vie.bmp");

    return textures;
}

void dessinerTexture(SDL_Renderer *renderer, SDL_Texture *texture, int x, int y, int w, int h, int cameraX)
{
    SDL_Rect dest = {x * BLOC_SIZE - cameraX, y * BLOC_SIZE, w * BLOC_SIZE, h * BLOC_SIZE};
    SDL_RenderCopy(renderer, texture, NULL, &dest);
}

void libererTextures(TexturesJeu textures)
{
    if (textures.bloc)
        SDL_DestroyTexture(textures.bloc);
    if (textures.blocSol)
        SDL_DestroyTexture(textures.blocSol);
    if (textures.blocStructure)
        SDL_DestroyTexture(textures.blocStructure);
    if (textures.blocRecompense)
        SDL_DestroyTexture(textures.blocRecompense);

    if (textures.tuyauHaut1)
        SDL_DestroyTexture(textures.tuyauHaut1);
    if (textures.tuyauHaut2)
        SDL_DestroyTexture(textures.tuyauHaut2);
    if (textures.tuyauBas1)
        SDL_DestroyTexture(textures.tuyauBas1);
    if (textures.tuyauBas2)
        SDL_DestroyTexture(textures.tuyauBas2);

    if (textures.goomba1)
        SDL_DestroyTexture(textures.goomba1);
    if (textures.goomba2)
        SDL_DestroyTexture(textures.goomba2);
    if (textures.goombaMort)
        SDL_DestroyTexture(textures.goombaMort);
    if (textures.koopa1)
        SDL_DestroyTexture(textures.koopa1);
    if (textures.koopa2)
        SDL_DestroyTexture(textures.koopa2);
    if (textures.carapace1)
        SDL_DestroyTexture(textures.carapace1);
    if (textures.carapace2)
        SDL_DestroyTexture(textures.carapace2);

    if (textures.piece)
        SDL_DestroyTexture(textures.piece);
    if (textures.champignon)
        SDL_DestroyTexture(textures.champignon);
    if (textures.mario)
        SDL_DestroyTexture(textures.mario);
    if (textures.marioBig)
        SDL_DestroyTexture(textures.marioBig);
    if (textures.toad)
        SDL_DestroyTexture(textures.toad);

    if (textures.background)
        SDL_DestroyTexture(textures.background);
    if (textures.vie)
        SDL_DestroyTexture(textures.vie);
}

//---------------------------------------------------------
// Gestions des touches et des collisions (base du jeu)

void gererTouches(int *continuer, SDL_Rect *carre, int *enSaut, float *vitesseSaut, Touches *touches)
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_QUIT:
            *continuer = 0;
            break;

        case SDL_KEYDOWN:
            switch (event.key.keysym.sym)
            {
            case SDLK_q:
                touches->gauche = 1;
                break;
            case SDLK_d:
                touches->droite = 1;
                break;
            case SDLK_SPACE:
                touches->saut = 1;
                break;
            case SDLK_ESCAPE:
                *continuer = 0;
                break;
            }
            break;

        case SDL_KEYUP:
            switch (event.key.keysym.sym)
            {
            case SDLK_q:
                touches->gauche = 0;
                break;
            case SDLK_d:
                touches->droite = 0;
                break;
            }
            break;
        }
    }
}

int detecterCollision(SDL_Rect joueur)
{
    int gauche = joueur.x / BLOC_SIZE;
    int droite = (joueur.x + joueur.w - 1) / BLOC_SIZE;
    int haut = joueur.y / BLOC_SIZE;
    int bas = (joueur.y + joueur.h - 1) / BLOC_SIZE;

    // Vérifier les limites de la carte
    if (gauche < 0)
        gauche = 0;
    if (droite >= MAP_LARGEUR)
        droite = MAP_LARGEUR - 1;
    if (haut < 0)
        haut = 0;
    if (bas >= MAP_HAUTEUR)
        bas = MAP_HAUTEUR - 1;

    // Vérifier les collisions avec les blocs
    for (int y = haut; y <= bas; y++)
    {
        for (int x = gauche; x <= droite; x++)
        {
            if (x < 0 || x >= MAP_LARGEUR || y < 0 || y >= MAP_HAUTEUR)
                continue;

            int bloc = map[y][x];

            // On ignore les pièces et TOAD
            if (bloc != 0 && bloc != PIECE && bloc != TOAD)
            {
                // Si c'est un bloc mystère, vérifier si Mario le touche par dessous
                if (bloc == BLOC_RECOMPENSE)
                {
                    int hautBloc = y * BLOC_SIZE;
                    int basJoueur = joueur.y + joueur.h;

                    // Collision par en dessous → on ne bloque pas
                    if (basJoueur <= hautBloc + 5)
                        continue;
                }

                return 1; // collision détectée
            }
        }
    }

    return 0; // pas de collision
}

//---------------------------------------------------------
// gestion des blocs mystères
int detecterCollisionBlocMystere(SDL_Rect joueur, float vitesseSaut)
{
    if (vitesseSaut >= 0)
        return 0; // On ne vérifie que si Mario saute vers le haut

    int xCentre = (joueur.x + joueur.w / 2) / BLOC_SIZE;
    int yDessus = (joueur.y - 1) / BLOC_SIZE;

    if (xCentre >= 0 && xCentre < MAP_LARGEUR && yDessus >= 0 && yDessus < MAP_HAUTEUR)
    {
        if (map[yDessus][xCentre] == BLOC_RECOMPENSE)
        {
            return 1;
        }
    }

    return 0;
}
Champignon champi = {{0, 0, BLOC_SIZE, BLOC_SIZE}, 0, 0.0f};

void ChampignonSiBlocMystereTouche(SDL_Rect joueur, SDL_Rect *champignon, float vitesseSaut)
{
    int xBloc = (joueur.x + joueur.w / 2) / BLOC_SIZE;
    int yBloc = (joueur.y - 1) / BLOC_SIZE;

    if (map[yBloc][xBloc] == BLOC_RECOMPENSE)
    {
        map[yBloc][xBloc] = BLOC_STRUCTURE;
        champignon->x = xBloc * BLOC_SIZE;
        champignon->y = (yBloc - 1) * BLOC_SIZE;
    }
}

//---------------------------------------------------------
// Initialisation et affichage des maps et des ennemis

// Initalisation des maps dans le fichier map.c

int map[MAP_HAUTEUR][MAP_LARGEUR];

void initialiserMap(int niveau)
{
    char (*sourceMap)[MAP_LARGEUR];

    switch (niveau)
    {
    case 1:
        sourceMap = niveau1;
        break;
    case 2:
        sourceMap = niveau2;
        break;
    case 3:
        sourceMap = niveau3;
        break;
    case 4:
        sourceMap = niveau4;
        break;
    case 5:
        sourceMap = niveau5;
        break;
    case 6:
        sourceMap = niveau6;
        break;
    case 7:
        sourceMap = niveau7;
        break;
    case 8:
        sourceMap = niveau8;
        break;
    case 9:
        sourceMap = niveau9;
        break;
    default:
        sourceMap = niveau1;
        break;
    }

    for (int y = 0; y < MAP_HAUTEUR; y++)
        for (int x = 0; x < MAP_LARGEUR; x++)
            map[y][x] = 0;

    for (int y = 0; y < MAP_HAUTEUR; y++)
    {
        for (int x = 0; x < MAP_LARGEUR; x++)
        {
            switch (sourceMap[y][x])
            {
            case '#':
                map[y][x] = BLOC;
                break;
            case '=':
                map[y][x] = BLOC_SOL;
                break;
            case '$':
                map[y][x] = PIECE;
                break;
            case '?':
                map[y][x] = BLOC_RECOMPENSE;
                break;
            case '1':
                map[y][x] = TUYAU_HAUT_GAUCHE;
                break;
            case '2':
                map[y][x] = TUYAU_HAUT_DROITE;
                break;
            case '3':
                map[y][x] = TUYAU_BAS_GAUCHE;
                break;
            case '4':
                map[y][x] = TUYAU_BAS_DROITE;
                break;
            case 'f':
                map[y][x] = TOAD;
                break;
            case 'u':
                map[y][x] = BLOC_STRUCTURE;
                break;
            default:
                map[y][x] = 0;
                break;
            }
        }
    }

    initialiserEnnemis(sourceMap);
}

Ennemi ennemis[MAX_ENNEMIS];

void initialiserEnnemis(char niveau[][MAP_LARGEUR])
{
    for (int i = 0; i < MAX_ENNEMIS; i++)
    {
        ennemis[i].actif = 0;
    }

    int nombreEnnemis = 0;

    for (int y = 0; y < MAP_HAUTEUR; y++)
    {
        for (int x = 0; x < MAP_LARGEUR; x++)
        {
            if (nombreEnnemis >= MAX_ENNEMIS)
                return;

            if (niveau[y][x] == 'g')
            {
                ennemis[nombreEnnemis].rect.x = x * BLOC_SIZE;
                ennemis[nombreEnnemis].rect.y = y * BLOC_SIZE;
                ennemis[nombreEnnemis].rect.w = BLOC_SIZE;
                ennemis[nombreEnnemis].rect.h = BLOC_SIZE;
                ennemis[nombreEnnemis].direction = (rand() % 2) ? DROITE : GAUCHE; // Direction aléatoire entre droite et gauche
                ennemis[nombreEnnemis].type = GOOMBA;
                ennemis[nombreEnnemis].actif = 1;
                nombreEnnemis++;
            }

            if (niveau[y][x] == 'k')
            {
                ennemis[nombreEnnemis].rect.x = x * BLOC_SIZE;
                ennemis[nombreEnnemis].rect.y = y * BLOC_SIZE;
                ennemis[nombreEnnemis].rect.w = BLOC_SIZE;
                ennemis[nombreEnnemis].rect.h = BLOC_SIZE;
                ennemis[nombreEnnemis].direction = (rand() % 2) ? DROITE : GAUCHE;
                ennemis[nombreEnnemis].type = KOOPA;
                ennemis[nombreEnnemis].actif = 1;
                nombreEnnemis++;
            }
        }
    }
}

void dessinerMap(SDL_Renderer *renderer, int cameraX, TexturesJeu textures)
{
    for (int y = 0; y < MAP_HAUTEUR; y++)
    {
        for (int x = 0; x < MAP_LARGEUR; x++)
        {
            int bloc = map[y][x];
            if (bloc == 0)
                continue;

            SDL_Texture *texture = NULL;

            switch (bloc)
            {
            case BLOC:
                texture = textures.bloc;
                break;
            case BLOC_SOL:
                texture = textures.blocSol;
                break;
            case BLOC_STRUCTURE:
                texture = textures.blocStructure;
                break;
            case BLOC_RECOMPENSE:
                texture = textures.blocRecompense;
                break;
            case TUYAU_BAS_DROITE:
                texture = textures.tuyauBas2;
                break;
            case TUYAU_BAS_GAUCHE:
                texture = textures.tuyauBas1;
                break;
            case TUYAU_HAUT_DROITE:
                texture = textures.tuyauHaut2;
                break;
            case TUYAU_HAUT_GAUCHE:
                texture = textures.tuyauHaut1;
                break;
            case PIECE:
                texture = textures.piece;
                break;
            case TOAD:
                texture = textures.toad;
                break;
            default:
                texture = NULL;
                break;
            }

            if (texture)
                dessinerTexture(renderer, texture, x, y, 1, 1, cameraX);
        }
    }
}

void dessinerEnnemis(SDL_Renderer *renderer, int cameraX, TexturesJeu textures)
{
    for (int i = 0; i < MAX_ENNEMIS; i++)
    {
        if (ennemis[i].actif)
        {
            SDL_Rect actuel = {
                ennemis[i].rect.x - cameraX,
                ennemis[i].rect.y,
                ennemis[i].rect.w,
                ennemis[i].rect.h};

            SDL_Texture *texture = NULL;

            if (ennemis[i].type == GOOMBA)
                texture = textures.goomba1;
            else if (ennemis[i].type == KOOPA)
                texture = textures.koopa1;

            if (texture)
                SDL_RenderCopy(renderer, texture, NULL, &actuel);
        }
    }
}

//---------------------------------------------------------
// Deplacement ennemis

void mettreAJourEnnemis()
{
    for (int i = 0; i < MAX_ENNEMIS; i++)
    {
        if (ennemis[i].actif)
        {
            // Déplacement horizontal
            SDL_Rect testRect = ennemis[i].rect;
            testRect.x += ennemis[i].direction;

            // Vérifier collision avec les murs
            if (detecterCollision(testRect))
            {
                ennemis[i].direction = -ennemis[i].direction;
            }

            // Vérifier collision avec d'autres ennemis
            else if (detecterCollisionEntreEnnemis(testRect, i))
            {
                ennemis[i].direction = -ennemis[i].direction;
            }
            else
            {
                // Vérifier si l'ennemi a du sol sous lui
                SDL_Rect testSol = testRect;
                testSol.y += 5;
                testSol.w -= 10;
                testSol.x += 5;

                if (!detecterCollision(testSol))
                {
                    // Pas de sol, changer de direction
                    ennemis[i].direction = -ennemis[i].direction;
                }
                else
                {
                    ennemis[i].rect.x += ennemis[i].direction;
                }
            }

            // Appliquer la gravité aux ennemis s'ils ne sont pas sur le sol
            SDL_Rect testChute = ennemis[i].rect;
            testChute.y += 1;

            if (!detecterCollision(testChute))
            {
                // Pour appliquer la gravité
                ennemis[i].rect.y += 5;

                testChute = ennemis[i].rect;
                testChute.y += 1;

                if (detecterCollision(testChute))
                {

                    ennemis[i].rect.y = ((ennemis[i].rect.y + ennemis[i].rect.h) / BLOC_SIZE) * BLOC_SIZE - ennemis[i].rect.h;
                }
            }

            if (ennemis[i].rect.y > MAP_HAUTEUR * BLOC_SIZE)
            {
                ennemis[i].actif = 0;
            }
        }
    }
}

int detecterCollisionEntreEnnemis(SDL_Rect ennemi, int indexEnnemi)
{
    for (int i = 0; i < MAX_ENNEMIS; i++)
    {
        if (i != indexEnnemi && ennemis[i].actif)
        {
            if (ennemi.x < ennemis[i].rect.x + ennemis[i].rect.w &&
                ennemi.x + ennemi.w > ennemis[i].rect.x &&
                ennemi.y < ennemis[i].rect.y + ennemis[i].rect.h &&
                ennemi.y + ennemi.h > ennemis[i].rect.y)
            {
                return 1;
            }
        }
    }
    return 0;
}

//---------------------------------------------------------
// Collisions / Interaction basique avec les ennemis

int detecterCollisionEnnemi(SDL_Rect joueur)
{
    for (int i = 0; i < MAX_ENNEMIS; i++)
    {
        if (ennemis[i].actif)
        {
            if (joueur.x < ennemis[i].rect.x + ennemis[i].rect.w &&
                joueur.x + joueur.w > ennemis[i].rect.x &&
                joueur.y < ennemis[i].rect.y + ennemis[i].rect.h &&
                joueur.y + joueur.h > ennemis[i].rect.y)
            {
                int basJoueur = joueur.y + joueur.h;
                int hautEnnemi = ennemis[i].rect.y;

                if (basJoueur >= hautEnnemi - 5 && basJoueur <= hautEnnemi + 10)
                {
                    continue;
                }

                return 1;
            }
        }
    }
    return 0;
}

int sauterSurEnnemi(SDL_Rect joueur, float vitesseSaut, ScoreJeu *scoreData)
{
    if (vitesseSaut > 0)
    {
        for (int i = 0; i < MAX_ENNEMIS; i++)
        {
            if (ennemis[i].actif)
            {
                int basJoueur = joueur.y + joueur.h;
                int hautEnnemi = ennemis[i].rect.y;

                if (basJoueur >= hautEnnemi - 5 && basJoueur <= hautEnnemi + 10)
                {
                    if (joueur.x < ennemis[i].rect.x + ennemis[i].rect.w &&
                        joueur.x + joueur.w > ennemis[i].rect.x)
                    {
                        ajouterEffetEcrasement(ennemis[i].rect.x, ennemis[i].rect.y);

                        if (ennemis[i].type == KOOPA)
                        {
                            for (int j = 0; j < MAX_ENNEMIS; j++)
                            {
                                if (!carapaces[j].actif)
                                {
                                    carapaces[j].rect = ennemis[i].rect;
                                    carapaces[j].vitesse = 0;
                                    carapaces[j].actif = 1;
                                    break;
                                }
                            }
                        }

                        // Il gagne 400 points
                        // et un bonus de 100 points par ennemi tué dans les 5 secondes
                        scoreData->score += 400;

                        // Afficher l'effet de points (optionnel si vous avez cette fonction)

                        ennemis[i].actif = 0;
                        return 1;
                    }
                }
            }
        }
    }
    return 0;
}

//---------------------------------------------------------
// Gestion des affichages apres collision avec les ennemis

Effet effets[MAX_ENNEMIS];

void initialiserEffets()
{
    for (int i = 0; i < MAX_ENNEMIS; i++)
    {
        effets[i].actif = 0;
    }
}

void ajouterEffetEcrasement(int x, int y)
{
    for (int i = 0; i < MAX_ENNEMIS; i++)
    {
        if (!effets[i].actif)
        {
            effets[i].rect.x = x;
            effets[i].rect.y = y;
            effets[i].rect.w = BLOC_SIZE;
            effets[i].rect.h = BLOC_SIZE;
            effets[i].duree = 30;
            effets[i].actif = 1;
            break;
        }
    }
}

void mettreAJourEffets()
{
    for (int i = 0; i < MAX_ENNEMIS; i++)
    {
        if (effets[i].actif)
        {
            effets[i].duree--;
            if (effets[i].duree <= 0)
            {
                effets[i].actif = 0;
            }
        }
    }
}

void dessinerEffets(SDL_Renderer *renderer, int cameraX, TexturesJeu textures)
{
    for (int i = 0; i < MAX_ENNEMIS; i++)
    {
        if (effets[i].actif)
        {
            SDL_Rect dest = {
                effets[i].rect.x - cameraX,
                effets[i].rect.y,
                effets[i].rect.w,
                effets[i].rect.h};

            SDL_Texture *textureEffet = textures.goombaMort;
            for (int j = 0; j < MAX_ENNEMIS; j++)
            {
                if (ennemis[j].rect.x == effets[i].rect.x && ennemis[j].rect.y == effets[i].rect.y)
                {
                    if (ennemis[j].type == KOOPA)
                    {
                        textureEffet = textures.carapace1;
                    }
                    else if (ennemis[j].type == GOOMBA)
                    {
                        textureEffet = textures.goombaMort;
                    }
                    break;
                }
            }

            SDL_RenderCopy(renderer, textureEffet, NULL, &dest);

            effets[i].duree--;
            if (effets[i].duree <= 0)
            {
                effets[i].actif = 0;
            }
        }
    }
}

//---------------------------------------------------------

Carapace carapaces[MAX_ENNEMIS];

void initialiserCarapaces()
{
    for (int i = 0; i < MAX_ENNEMIS; i++)
    {
        carapaces[i].actif = 0;
        carapaces[i].mobile = 0;
        carapaces[i].vitesse = 0;
        carapaces[i].direction = 0;
        carapaces[i].tempsLancement = 0;
        carapaces[i].rect.x = 0;
        carapaces[i].rect.y = 0;
        carapaces[i].rect.w = BLOC_SIZE;
        carapaces[i].rect.h = BLOC_SIZE;
    }
}

void mettreAJourCarapaces()
{
    for (int i = 0; i < MAX_ENNEMIS; i++)
    {
        if (!carapaces[i].actif || !carapaces[i].mobile)
            continue;

        carapaces[i].rect.x += carapaces[i].vitesse * carapaces[i].direction;

        if (detecterCollision(carapaces[i].rect))
        {
            carapaces[i].direction *= -1;
            carapaces[i].rect.x += carapaces[i].vitesse * carapaces[i].direction;
        }

        SDL_Rect testSol = carapaces[i].rect;
        testSol.y += 5;
        if (!detecterCollision(testSol))
        {
            carapaces[i].rect.y += 5;
        }

        if (carapaces[i].rect.x < 0 || carapaces[i].rect.x + carapaces[i].rect.w > MAP_LARGEUR * BLOC_SIZE)
        {
            carapaces[i].direction *= -1;
            carapaces[i].rect.x += carapaces[i].vitesse * carapaces[i].direction;
        }
    }
}

int interagirAvecCarapaces(SDL_Rect *joueur, float *vitesseSaut)
{
    int maintenant = (int)SDL_GetTicks(); // SDL_GetTicks EXPLICATION !!!!

    for (int i = 0; i < MAX_ENNEMIS; i++)
    {
        if (!carapaces[i].actif)
            continue;

        SDL_Rect *c = &carapaces[i].rect;

        int basJoueur = joueur->y + joueur->h;
        int hautCarapace = c->y;
        int toucheDessus = (basJoueur >= hautCarapace - 5 && basJoueur <= hautCarapace + 10);

        if (toucheDessus &&
            joueur->x + joueur->w > c->x && joueur->x < c->x + c->w &&
            *vitesseSaut > 0)
        {
            if (carapaces[i].mobile)
            {
                carapaces[i].mobile = 0;
                carapaces[i].vitesse = 0;
            }

            *vitesseSaut = FORCE_SAUT / 1.5f;
            return 0;
        }

        int collisionLaterale =
            joueur->x + joueur->w > c->x && joueur->x < c->x + c->w &&
            joueur->y + joueur->h > c->y && joueur->y < c->y + c->h;

        if (collisionLaterale)
        {
            if (!carapaces[i].mobile)
            {
                // On l’active (Mario l’a tapée en marchant dessus)
                carapaces[i].mobile = 1;
                carapaces[i].vitesse = 6;
                carapaces[i].direction = (joueur->x < c->x) ? DROITE : GAUCHE;
                carapaces[i].tempsLancement = maintenant;
            }
            else
            {
                // Ne tuer Mario que si la carapace est mobile depuis plus de 200 ms
                if (carapaces[i].mobile && maintenant - carapaces[i].tempsLancement >= 200)
                {
                    return 1;
                }
            }
        }
    }

    return 0;
}

void carapacesTuantEnnemis()
{
    int maintenant = (int)SDL_GetTicks();

    for (int i = 0; i < MAX_ENNEMIS; i++)
    {
        if (!carapaces[i].actif || !carapaces[i].mobile)
            continue;
        if (maintenant - carapaces[i].tempsLancement < 300)
            continue;

        SDL_Rect c = carapaces[i].rect;

        for (int j = 0; j < MAX_ENNEMIS; j++)
        {
            if (!ennemis[j].actif)
                continue;

            SDL_Rect e = ennemis[j].rect;

            if (SDL_HasIntersection(&c, &e)) // SDL_HasIntersection EXPLICATION !!!!
            {
                ennemis[j].actif = 0;
            }
        }
    }
}

void dessinerCarapaces(SDL_Renderer *renderer, int cameraX, TexturesJeu textures)
{
    for (int i = 0; i < MAX_ENNEMIS; i++)
    {
        if (carapaces[i].actif)
        {
            SDL_Rect dst = {
                carapaces[i].rect.x - cameraX,
                carapaces[i].rect.y,
                carapaces[i].rect.w,
                carapaces[i].rect.h};
            SDL_RenderCopy(renderer, textures.carapace1, NULL, &dst);
        }
    }
}

//---------------------------------------------------------
// Affichage du score / vies

void afficherScore(SDL_Renderer *renderer, ScoreJeu *scoreJeu, TTF_Font *police)
{
    char texte[64];
    sprintf(texte, "Score : %d", scoreJeu->score);

    SDL_Color couleur = {255, 255, 255};
    SDL_Surface *surfaceTexte = TTF_RenderText_Solid(police, texte, couleur);
    if (!surfaceTexte)
    {
        printf("Erreur lors du rendu du texte: %s\n", TTF_GetError());
        return;
    }

    SDL_Texture *textureTexte = SDL_CreateTextureFromSurface(renderer, surfaceTexte);
    if (!textureTexte)
    {
        printf("Erreur lors de la création de la texture du texte: %s\n", SDL_GetError());
        SDL_FreeSurface(surfaceTexte);
        return;
    }

    SDL_Rect dest = {10, 10, surfaceTexte->w, surfaceTexte->h};
    SDL_RenderCopy(renderer, textureTexte, NULL, &dest);

    SDL_FreeSurface(surfaceTexte);
    SDL_DestroyTexture(textureTexte);
}

// FONCTION VIES
void afficherVies(SDL_Renderer *renderer, ScoreJeu *scoreJeu, TexturesJeu textures)
{
    int taille = 30;
    int marge = 10;
    SDL_Rect rect;
    rect.y = 10;
    rect.w = taille;
    rect.h = taille;

    // Aligné à droite
    rect.x = LONGUEUR_FENETRE - scoreJeu->vies * (taille + marge);

    for (int i = 0; i < scoreJeu->vies; i++)
    {
        SDL_RenderCopy(renderer, textures.vie, NULL, &rect);
        rect.x += taille + marge;
    }
}

//---------------------------------------------------------
// Boutons pour Menu

void initialiserBoutons(Bouton boutons[], int nombreBoutons, const char *labels[])
{
    int largeur = 200;
    int hauteur = 50;
    int espacement = 20;
    int debutY = (LARGEUR_FENETRE - (nombreBoutons * hauteur + (nombreBoutons - 1) * espacement)) / 2;

    for (int i = 0; i < nombreBoutons; i++)
    {
        boutons[i].rect.x = (LONGUEUR_FENETRE - largeur) / 2;
        boutons[i].rect.y = debutY + i * (hauteur + espacement);
        boutons[i].rect.w = largeur;
        boutons[i].rect.h = hauteur;
        boutons[i].texte = labels[i]; // on pointe sur la chaÃ®ne constante
        boutons[i].hover = 0;
    }
}

void dessinerBoutons(SDL_Renderer *renderer, Bouton boutons[], int nombreBoutons, TTF_Font *police)
{
    for (int i = 0; i < nombreBoutons; i++)
    {
        if (boutons[i].hover)
        {
            SDL_SetRenderDrawColor(renderer, 100, 150, 255, 255);
        }
        else
        {
            SDL_SetRenderDrawColor(renderer, 70, 70, 200, 255);
        }

        SDL_RenderFillRect(renderer, &boutons[i].rect);

        SDL_Color couleurTexte = {255, 255, 255};
        SDL_Surface *surfaceTexte = TTF_RenderText_Solid(police, boutons[i].texte, couleurTexte);
        if (!surfaceTexte)
        {
            printf("Erreur TTF_RenderText_Solid\n");
            continue;
        }

        SDL_Texture *textureTexte = SDL_CreateTextureFromSurface(renderer, surfaceTexte);
        if (!textureTexte)
        {
            printf("Erreur SDL_CreateTextureFromSurface\n");
            SDL_FreeSurface(surfaceTexte);
            continue;
        }

        SDL_Rect destTexte = {
            boutons[i].rect.x + (boutons[i].rect.w - surfaceTexte->w) / 2,
            boutons[i].rect.y + (boutons[i].rect.h - surfaceTexte->h) / 2,
            surfaceTexte->w,
            surfaceTexte->h};

        SDL_RenderCopy(renderer, textureTexte, NULL, &destTexte);

        SDL_FreeSurface(surfaceTexte);
        SDL_DestroyTexture(textureTexte);
    }
}

int pointDansRect(int x, int y, SDL_Rect rect)
{
    return (x >= rect.x && x <= rect.x + rect.w && y >= rect.y && y <= rect.y + rect.h);
}

//---------------------------------------------------------
// Menu

int gererEvenementsMenu(int *continuer, Bouton boutons[], int nombreBoutons)
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_QUIT:
            *continuer = 0;
            break;

        case SDL_MOUSEMOTION:
        {
            int mx = event.motion.x, my = event.motion.y;
            for (int i = 0; i < nombreBoutons; i++)
                boutons[i].hover = pointDansRect(mx, my, boutons[i].rect);
        }
        break;

        case SDL_MOUSEBUTTONDOWN:
            if (event.button.button == SDL_BUTTON_LEFT)
            {
                int mx = event.button.x, my = event.button.y;
                for (int i = 0; i < nombreBoutons; i++)
                {
                    if (pointDansRect(mx, my, boutons[i].rect))
                    {
                        if (i == 0)
                            return ETAT_JEU;
                        else if (i == 1)
                            return ETAT_SELECTION;
                        else if (i == 2)
                            *continuer = 0;
                    }
                }
            }
            break;

        case SDL_KEYDOWN:
            if (event.key.keysym.sym == SDLK_ESCAPE)
                *continuer = 0;
            break;
        }
    }
    return -1;
}

//---------------------------------------------------------
// Gestion transition entre les niveaux

int finDeNiveau(SDL_Rect joueur)
{
    int joueurGaucheBloc = (joueur.x / BLOC_SIZE > 0) ? (joueur.x / BLOC_SIZE - 1) : 0;
    int joueurDroiteBloc = ((joueur.x + joueur.w - 1) / BLOC_SIZE + 1 < MAP_LARGEUR) ? ((joueur.x + joueur.w - 1) / BLOC_SIZE + 1) : (MAP_LARGEUR - 1);
    int joueurHautBloc = (joueur.y / BLOC_SIZE > 0) ? (joueur.y / BLOC_SIZE - 1) : 0;
    int joueurBasBloc = ((joueur.y + joueur.h - 1) / BLOC_SIZE + 1 < MAP_HAUTEUR) ? ((joueur.y + joueur.h - 1) / BLOC_SIZE + 1) : (MAP_HAUTEUR - 1);

    for (int y = joueurHautBloc; y <= joueurBasBloc; y++)
    {
        for (int x = joueurGaucheBloc; x <= joueurDroiteBloc; x++)
        {

            if (map[y][x] == TOAD)
            {
                // Calculer les coordonnées réelles du bloc de fin
                SDL_Rect blocFin = {
                    x * BLOC_SIZE,
                    y * BLOC_SIZE,
                    BLOC_SIZE,
                    BLOC_SIZE};

                if (joueur.x < blocFin.x + blocFin.w &&
                    joueur.x + joueur.w > blocFin.x &&
                    joueur.y < blocFin.y + blocFin.h &&
                    joueur.y + joueur.h > blocFin.y)
                {
                    return 1;
                }
            }
        }
    }

    return 0;
}
int gererEvenementsNiveauTermine(int *continuer, Bouton boutons[], int nombreBoutons)
{
    SDL_Event event;

    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_QUIT:
            *continuer = 0;
            break;

        case SDL_MOUSEMOTION:
        {
            int mouseX = event.motion.x;
            int mouseY = event.motion.y;

            for (int i = 0; i < nombreBoutons; i++)
            {
                if (mouseX >= boutons[i].rect.x && mouseX <= boutons[i].rect.x + boutons[i].rect.w &&
                    mouseY >= boutons[i].rect.y && mouseY <= boutons[i].rect.y + boutons[i].rect.h)
                {
                    boutons[i].hover = 1;
                }
                else
                {
                    boutons[i].hover = 0;
                }
            }
        }
        break;

        case SDL_MOUSEBUTTONDOWN:
            if (event.button.button == SDL_BUTTON_LEFT)
            {
                int mouseX = event.button.x;
                int mouseY = event.button.y;

                for (int i = 0; i < nombreBoutons; i++)
                {
                    if (mouseX >= boutons[i].rect.x && mouseX <= boutons[i].rect.x + boutons[i].rect.w &&
                        mouseY >= boutons[i].rect.y && mouseY <= boutons[i].rect.y + boutons[i].rect.h)
                    {

                        if (i == 0)
                            return ETAT_JEU;
                        else if (i == 1)
                            return ETAT_MENU;
                    }
                }
            }
            break;

        case SDL_KEYDOWN:
            if (event.key.keysym.sym == SDLK_ESCAPE)
            {
                return ETAT_MENU;
            }
            break;
        }
    }

    return -1;
}

int gererGameOver(int *continuer, Bouton boutons[], int nombreBoutons)
{
    SDL_Event event;

    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_QUIT:
            *continuer = 0;
            break;

        case SDL_MOUSEMOTION:
        {
            int mouseX = event.motion.x;
            int mouseY = event.motion.y;

            for (int i = 0; i < nombreBoutons; i++)
            {
                if (mouseX >= boutons[i].rect.x && mouseX <= boutons[i].rect.x + boutons[i].rect.w &&
                    mouseY >= boutons[i].rect.y && mouseY <= boutons[i].rect.y + boutons[i].rect.h)
                {
                    boutons[i].hover = 1;
                }
                else
                {
                    boutons[i].hover = 0;
                }
            }
        }
        break;

        case SDL_MOUSEBUTTONDOWN:
            if (event.button.button == SDL_BUTTON_LEFT)
            {
                int mouseX = event.button.x;
                int mouseY = event.button.y;

                for (int i = 0; i < nombreBoutons; i++)
                {
                    if (mouseX >= boutons[i].rect.x && mouseX <= boutons[i].rect.x + boutons[i].rect.w &&
                        mouseY >= boutons[i].rect.y && mouseY <= boutons[i].rect.y + boutons[i].rect.h)
                    {

                        if (i == 0)
                            return ETAT_JEU;
                        else if (i == 1)
                            return ETAT_MENU;
                    }
                }
            }
            break;

        case SDL_KEYDOWN:
            if (event.key.keysym.sym == SDLK_ESCAPE)
            {
                return ETAT_MENU;
            }
            break;
        }
    }

    return -1;
}


//---------------------------------------------------------
// Affichage des transitions entre les mondes

void afficherMonde2(SDL_Renderer *renderer, TTF_Font *police)
{
    SDL_Color couleur = {255, 255, 255};
    SDL_Surface *surfaceTexte = TTF_RenderText_Solid(police, "Monde 2", couleur);
    if (!surfaceTexte)
    {
        printf("Erreur TTF_RenderText_Solid\n");
        return;
    }

    SDL_Texture *textureTexte = SDL_CreateTextureFromSurface(renderer, surfaceTexte);
    SDL_FreeSurface(surfaceTexte);
    if (!textureTexte)
    {
        printf("Erreur SDL_CreateTextureFromSurface\n");
        return;
    }

    SDL_Rect rectTexte = {
        (LONGUEUR_FENETRE - surfaceTexte->w) / 2,
        (LARGEUR_FENETRE - surfaceTexte->h) / 2,
        surfaceTexte->w,
        surfaceTexte->h};

    int continuer = 1;
    SDL_Event event;

    while (continuer)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT || event.type == SDL_KEYDOWN)
            {
                continuer = 0;
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, textureTexte, NULL, &rectTexte);
        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    SDL_DestroyTexture(textureTexte);
}
void afficherMonde3(SDL_Renderer *renderer, TTF_Font *police)
{
    SDL_Color couleur = {255, 255, 255};
    SDL_Surface *surfaceTexte = TTF_RenderText_Solid(police, "Monde 3", couleur);
    if (!surfaceTexte)
    {
        printf("Erreur TTF_RenderText_Solid\n");
        return;
    }

    SDL_Texture *textureTexte = SDL_CreateTextureFromSurface(renderer, surfaceTexte);
    SDL_FreeSurface(surfaceTexte);
    if (!textureTexte)
    {
        printf("Erreur SDL_CreateTextureFromSurface\n");
        return;
    }

    SDL_Rect rectTexte = {
        (LONGUEUR_FENETRE - surfaceTexte->w) / 2,
        (LARGEUR_FENETRE - surfaceTexte->h) / 2,
        surfaceTexte->w,
        surfaceTexte->h};

    int continuer = 1;
    SDL_Event event;

    while (continuer)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT || event.type == SDL_KEYDOWN)
            {
                continuer = 0;
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, textureTexte, NULL, &rectTexte);
        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    SDL_DestroyTexture(textureTexte);
}

void afficherEcranFin(SDL_Renderer *renderer, TTF_Font *police)
{
    SDL_Color couleur = {255, 255, 255};
    SDL_Surface *surfaceTexte = TTF_RenderText_Solid(police, "Bien joue ! Jeu termine !", couleur);
    if (!surfaceTexte)
    {
        printf("Erreur TTF_RenderText_Solid\n");
        return;
    }

    SDL_Texture *textureTexte = SDL_CreateTextureFromSurface(renderer, surfaceTexte);
    SDL_FreeSurface(surfaceTexte);
    if (!textureTexte)
    {
        printf("Erreur SDL_CreateTextureFromSurface\n");
        return;
    }

    SDL_Rect rectTexte = {
        (LONGUEUR_FENETRE - surfaceTexte->w) / 2,
        (LARGEUR_FENETRE - surfaceTexte->h) / 2,
        surfaceTexte->w,
        surfaceTexte->h};

    int continuer = 1;
    SDL_Event event;

    while (continuer)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT || event.type == SDL_KEYDOWN)
            {
                continuer = 0;
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, textureTexte, NULL, &rectTexte);
        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    SDL_DestroyTexture(textureTexte);
}


//---------------------------------------------------------
// Affichage background

void dessinerFondParallaxe(SDL_Renderer *renderer, SDL_Texture *texture, int cameraX)
{
    int vitesseParallaxe = 3;
    int offsetX = cameraX / vitesseParallaxe;

    SDL_Rect dst = {-offsetX, 0, LONGUEUR_FENETRE, LARGEUR_FENETRE};
    while (dst.x < LONGUEUR_FENETRE)
    {
        SDL_RenderCopy(renderer, texture, NULL, &dst);
        dst.x += LONGUEUR_FENETRE;
    }
}

//---------------------------------------------------------
// Fonction de sauvegarde de la partie
void sauvegarderPartie(int niveau)
{
    FILE *fichier = fopen(FICHIER_SAUVEGARDE, "wb");
    if (!fichier)
        return;

    Sauvegarde sauvegarde = {niveau};
    fwrite(&sauvegarde, sizeof(Sauvegarde), 1, fichier);
    fclose(fichier);
}

int chargerPartie(int *niveau)
{
    FILE *fichier = fopen(FICHIER_SAUVEGARDE, "rb");
    if (!fichier)
        return 0;

    Sauvegarde sauvegarde;
    size_t lu = fread(&sauvegarde, sizeof(Sauvegarde), 1, fichier);
    fclose(fichier);

    if (lu != 1)
        return 0;

    *niveau = sauvegarde.niveau;
    return 1;
}
