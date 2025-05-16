
#include "fonction.h"

// Commit Elliott

int main(int argc, char *argv[])
{
    srand(time(NULL));

    if (SDL_Init(SDL_INIT_VIDEO) != 0 || TTF_Init() != 0)
    {
        fprintf(stderr, "Erreur SDL ou TTF_Init : %s\n", SDL_GetError());
        return 1;
    }

    TTF_Font *police = TTF_OpenFont("fonts/arial.ttf", 24);
    if (!police)
    {
        fprintf(stderr, "Erreur chargement police : %s\n", TTF_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Window *fenetre = creerFenetre("Super Mario ISEN");
    SDL_Renderer *renderer = creerRenderer(fenetre);
    TexturesJeu textures = chargerTextures(renderer);

    int startX = 100;
    int startY = (MAP_HAUTEUR - 3) * BLOC_SIZE - BLOC_SIZE;
    Mario mario = {{startX, startY, BLOC_SIZE, BLOC_SIZE}, 0};
    Champignon champi = {{0, 0, BLOC_SIZE, BLOC_SIZE}, 0, 0.0f};

    Touches touches = {0, 0, 0};
    int enSaut = 0;
    float vitesseSaut = 0.0f;
    int cameraX = 0;
    int Score = 0;
    int scoreFinal = 0;
    int current_level = 1;
    if (chargerPartie(&current_level))
        printf("Charge depuis sauvegarde: niveau=%d\n", current_level);
    else
        printf("Aucune sauvegarde, niveau 1.\n");

    // Détermine et charge le bon background
    SDL_DestroyTexture(textures.background);
    if (current_level <= 3)
        textures.background = chargerTextureBMP(renderer, "img/fond.bmp");
    else if (current_level <= 6)
        textures.background = chargerTextureBMP(renderer, "img/fond2.bmp");
    else
        textures.background = chargerTextureBMP(renderer, "img/fond3.bmp");

    int continuer = 1;
    int etatJeu = ETAT_MENU;
    ScoreJeu scoreJeu = {0, 3};

    initialiserMap(current_level);
    initialiserCarapaces();
    initialiserEffets();

    // Création des boutons
    char *labelsMenu[NB_BOUTONS_MENU] = {
        "Jouer",
        "Niveau",
        "Quitter"};
    Bouton boutonsMenu[NB_BOUTONS_MENU];
    initialiserBoutons(boutonsMenu, NB_BOUTONS_MENU, labelsMenu);

    Bouton boutonsNiveauTermine[2] = {
        {{380, 200, 200, 50}, strdup("Niveau Suivant"), 0},
        {{380, 270, 200, 50}, strdup("Menu Principal"), 0}};

    Bouton boutonsGameOver[2] = {
        {{380, 200, 200, 50}, strdup("Rejouer"), 0},
        {{380, 270, 200, 50}, strdup("Menu Principal"), 0}};

    while (continuer)
    {
        switch (etatJeu)
        {
        case ETAT_MENU:
        {
            int choix = gererEvenementsMenu(&continuer, boutonsMenu, NB_BOUTONS_MENU);
            if (choix >= 0)
            {
                etatJeu = choix;

                // Réinitialiser seulement si on a cliqué sur "Jouer"
                if (choix == ETAT_JEU)
                {
                    mario.invincible = 0;
                    mario.tempsInvincible = 0;

                    if (!chargerPartie(&current_level))
                    {
                        current_level = 1; // aucune sauvegarde trouvée
                    }

                    mario.corps.x = startX;
                    mario.corps.y = startY;
                    mario.estGrand = 0;
                    mario.corps.h = BLOC_SIZE;
                    champi.actif = 0;
                    champi.vitesseY = 0;
                    champi.corps.x = 0;
                    champi.corps.y = 0;
                    scoreJeu.vies = 3;

                    initialiserMap(current_level);
                    initialiserCarapaces();
                    initialiserEffets();

                    SDL_DestroyTexture(textures.background);
                    if (current_level <= 3)
                        textures.background = chargerTextureBMP(renderer, "img/fond.bmp");
                    else if (current_level <= 6)
                        textures.background = chargerTextureBMP(renderer, "img/fond2.bmp");
                    else
                        textures.background = chargerTextureBMP(renderer, "img/fond3.bmp");
                }
            }

            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderClear(renderer);
            dessinerBoutons(renderer, boutonsMenu, NB_BOUTONS_MENU, police);
            SDL_RenderPresent(renderer);
            SDL_Delay(16);
            break;
        }

        case ETAT_SELECTION:
        {
            static const char *labelsNiv[NOMBRE_NIVEAUX] = {
                "Niveau 1", "Niveau 2", "Niveau 3",
                "Niveau 4", "Niveau 5", "Niveau 6",
                "Niveau 7", "Niveau 8", "Niveau 9"};

            Bouton boutonsSelect[NOMBRE_NIVEAUX];
            int largeur = 180, hauteur = 50, espacement = 20;
            int debutX = (LONGUEUR_FENETRE - (3 * largeur + 2 * espacement)) / 2;
            int debutY = 100;

            for (int i = 0; i < NOMBRE_NIVEAUX; i++)
            {
                boutonsSelect[i].rect.x = debutX + (i % 3) * (largeur + espacement);
                boutonsSelect[i].rect.y = debutY + (i / 3) * (hauteur + 50);
                boutonsSelect[i].rect.w = largeur;
                boutonsSelect[i].rect.h = hauteur;
                boutonsSelect[i].texte = labelsNiv[i];
                boutonsSelect[i].hover = 0;
            }

            int choixNiv = -1;
            SDL_Event ev;
            while (choixNiv < 0 && continuer)
            {
                while (SDL_PollEvent(&ev))
                {
                    if (ev.type == SDL_QUIT)
                        continuer = 0;
                    else if (ev.type == SDL_MOUSEMOTION)
                    {
                        int mx = ev.motion.x, my = ev.motion.y;
                        for (int i = 0; i < NOMBRE_NIVEAUX; i++)
                            boutonsSelect[i].hover = pointDansRect(mx, my, boutonsSelect[i].rect);
                    }
                    else if (ev.type == SDL_MOUSEBUTTONDOWN && ev.button.button == SDL_BUTTON_LEFT)
                    {
                        int mx = ev.button.x, my = ev.button.y;
                        for (int i = 0; i < NOMBRE_NIVEAUX; i++)
                        {
                            if (pointDansRect(mx, my, boutonsSelect[i].rect))
                            {
                                choixNiv = i + 1;
                                break;
                            }
                        }
                    }
                }

                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                SDL_RenderClear(renderer);

                // Texte Monde
                SDL_Color couleur = {255, 255, 255};
                for (int ligne = 0; ligne < 3; ligne++)
                {
                    char texteMonde[32];
                    sprintf(texteMonde, "Monde %d :", ligne + 1);
                    SDL_Surface *surface = TTF_RenderText_Solid(police, texteMonde, couleur);
                    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
                    SDL_Rect pos = {debutX - 150, debutY + ligne * (hauteur + 50) + 10, surface->w, surface->h};
                    SDL_RenderCopy(renderer, texture, NULL, &pos);
                    SDL_FreeSurface(surface);
                    SDL_DestroyTexture(texture);
                }

                dessinerBoutons(renderer, boutonsSelect, NOMBRE_NIVEAUX, police);
                SDL_RenderPresent(renderer);
                SDL_Delay(16);
            }

            if (choixNiv > 0)
            {
                current_level = choixNiv;
                mario.corps.x = startX;
                mario.corps.y = startY;
                scoreJeu.score = 0;
                SDL_DestroyTexture(textures.background);
                if (current_level <= 3)
                    textures.background = chargerTextureBMP(renderer, "img/fond.bmp");
                else if (current_level <= 6)
                    textures.background = chargerTextureBMP(renderer, "img/fond2.bmp");
                else
                    textures.background = chargerTextureBMP(renderer, "img/fond3.bmp");
                initialiserMap(current_level);
                initialiserCarapaces();
                initialiserEffets();
                etatJeu = ETAT_JEU;
            }
            break;
        }

        case ETAT_JEU:
        {
            gererTouches(&continuer, &mario.corps, &enSaut, &vitesseSaut, &touches);

            SDL_Rect test = mario.corps;
            if (touches.gauche)
            {
                test.x -= VITESSE_DEPLACEMENT;
                if (!detecterCollision(test))
                    mario.corps.x = test.x;
            }
            if (touches.droite)
            {
                test.x += VITESSE_DEPLACEMENT;
                if (!detecterCollision(test))
                    mario.corps.x = test.x;
            }

            if (enSaut)
            {
                mario.corps.y += vitesseSaut;
                vitesseSaut += GRAVITE;

                test = mario.corps;
                test.y += 2;
                if (vitesseSaut > 0 && detecterCollision(test))
                {
                    mario.corps.y = (mario.corps.y + mario.corps.h) / BLOC_SIZE * BLOC_SIZE - mario.corps.h;
                    vitesseSaut = 0;
                    enSaut = 0;
                }

                test = mario.corps;
                test.y -= 1;
                if (vitesseSaut < 0 && detecterCollisionBlocMystere(mario.corps, vitesseSaut))
                {
                    int xBloc = (mario.corps.x + mario.corps.w / 2) / BLOC_SIZE;
                    int yBloc = (mario.corps.y - 1) / BLOC_SIZE;

                    if (map[yBloc][xBloc] == BLOC_RECOMPENSE)
                    {
                        map[yBloc][xBloc] = BLOC_STRUCTURE;
                        if (!mario.estGrand)
                        {
                            champi.corps.x = xBloc * BLOC_SIZE;
                            champi.corps.y = (yBloc - 1) * BLOC_SIZE - 5; // anti-glitch

                            champi.vitesseY = -2;
                            champi.vitesseX = 3.5f; // ou 2.0f pour + rapide
                            champi.direction = 1;   // droite par défaut
                            champi.actif = 1;
                        }
                        else
                        {
                            map[yBloc - 1][xBloc] = PIECE;
                        }
                    }

                    int yDessousBloc = (mario.corps.y - 1) / BLOC_SIZE;
                    mario.corps.y = (yDessousBloc + 1) * BLOC_SIZE;
                    vitesseSaut = 0;
                }
                else if (vitesseSaut < 0 && detecterCollision(test))
                {
                    int yDessousBloc = (mario.corps.y - 1) / BLOC_SIZE;
                    mario.corps.y = (yDessousBloc + 1) * BLOC_SIZE;
                    vitesseSaut = 0;
                }
            }
            else
            {
                test = mario.corps;
                test.y += 8;
                if (!detecterCollision(test))
                {
                    enSaut = 1;
                    vitesseSaut = 0;
                }
                else if (touches.saut)
                {
                    enSaut = 1;
                    vitesseSaut = FORCE_SAUT;
                    touches.saut = 0;
                }
            }

            if (enSaut && sauterSurEnnemi(mario.corps, vitesseSaut, &scoreJeu))
                vitesseSaut = FORCE_SAUT / 1.5f;

            if (!mario.invincible && interagirAvecCarapaces(&mario.corps, &vitesseSaut))
            {
                if (mario.estGrand)
                {
                    // Simule le corps réduit
                    SDL_Rect futurCorps = mario.corps;
                    futurCorps.h = BLOC_SIZE;
                    futurCorps.y += (mario.corps.h - BLOC_SIZE);

                    // Check si y'a de la place
                    if (!detecterCollision(futurCorps))
                    {
                        mario.corps = futurCorps;
                    }
                    else
                    {
                        // Pas de place → on pousse encore plus (juste en dessous)
                        mario.corps.y += BLOC_SIZE;
                        mario.corps.h = BLOC_SIZE;
                    }

                    mario.estGrand = 0;
                    mario.invincible = 1;
                    mario.tempsInvincible = SDL_GetTicks();
                }
                else
                {
                    scoreJeu.vies--;
                    if (scoreJeu.vies <= 0)
                    {
                        scoreFinal = scoreJeu.score;
                        etatJeu = ETAT_GAME_OVER;
                        break;
                    }
                    else
                    {
                        // Réinitialisation du niveau sans perdre le score
                        mario.corps.x = startX;
                        mario.corps.y = startY;
                        enSaut = 0;
                        vitesseSaut = 0;
                        touches.gauche = touches.droite = touches.saut = 0;
                        initialiserMap(current_level);
                        initialiserCarapaces();
                        initialiserEffets();
                        cameraX = 0;
                        break;
                    }
                }
            }

            carapacesTuantEnnemis();
            mettreAJourCarapaces();
            mettreAJourEnnemis();
            mettreAJourEffets();
            // Gérer l'invincibilité
            if (mario.invincible)
            {
                if (SDL_GetTicks() - mario.tempsInvincible >= 1000)
                {
                    mario.invincible = 0;
                }
            }

            if (!mario.invincible && (detecterCollisionEnnemi(mario.corps) || mario.corps.y > MAP_HAUTEUR * BLOC_SIZE))
            {
                if (mario.estGrand)
                {
                    // Préparer futur corps de Mario petit
                    SDL_Rect futurCorps = mario.corps;
                    futurCorps.h = BLOC_SIZE;
                    futurCorps.y += (mario.corps.h - BLOC_SIZE); // décale vers le bas

                    if (!detecterCollision(futurCorps))
                    {
                        mario.corps = futurCorps;
                    }
                    else
                    {
                        // y'a un bloc en dessous, on pousse encore plus pour pas se bloquer
                        mario.corps.y += BLOC_SIZE;
                        mario.corps.h = BLOC_SIZE;
                    }

                    mario.estGrand = 0;
                    mario.invincible = 1;
                    mario.tempsInvincible = SDL_GetTicks();
                }
                else
                {
                    scoreJeu.vies--;
                    if (scoreJeu.vies <= 0)
                    {
                        scoreFinal = scoreJeu.score;
                        etatJeu = ETAT_GAME_OVER;
                        break;
                    }
                    else
                    {
                        // Réinitialisation du niveau sans perdre le score
                        mario.corps.x = startX;
                        mario.corps.y = startY;
                        enSaut = 0;
                        vitesseSaut = 0;
                        touches.gauche = touches.droite = touches.saut = 0;
                        initialiserMap(current_level); // Recharger le niveau actuel
                        initialiserCarapaces();
                        initialiserEffets();
                        cameraX = 0;
                        break;
                    }
                }
            }

            int gauche = mario.corps.x / BLOC_SIZE;
            int droite = (mario.corps.x + mario.corps.w - 1) / BLOC_SIZE;
            int haut = mario.corps.y / BLOC_SIZE;
            int bas = (mario.corps.y + mario.corps.h - 1) / BLOC_SIZE;

            for (int y = haut; y <= bas; y++)
                for (int x = gauche; x <= droite; x++)
                    if (map[y][x] == PIECE)
                    {
                        map[y][x] = 0;
                        scoreJeu.score += 500;
                    }

            if (finDeNiveau(mario.corps))
            {
                scoreJeu.score += 2000;
                etatJeu = ETAT_NIVEAU_TERMINE;
                break;
            }

            if (champi.actif)
            {
                // GRAVITÉ
                champi.vitesseY += GRAVITE;
                SDL_Rect testY = champi.corps;
                testY.y += (int)champi.vitesseY;

                if (!detecterCollision(testY))
                {
                    champi.corps.y = testY.y;
                }
                else
                {
                    champi.vitesseY = 0;
                    // Ajuste pile sur le bloc, sans le repousser violemment car sinon rebond
                    champi.corps.y = (champi.corps.y + BLOC_SIZE) / BLOC_SIZE * BLOC_SIZE - champi.corps.h;
                }

                // DÉPLACEMENT HORIZONTAL
                SDL_Rect testX = champi.corps;
                testX.x += champi.vitesseX * champi.direction;

                if (!detecterCollision(testX))
                {
                    champi.corps.x = testX.x;
                }
                else
                {
                    champi.direction *= -1;
                }
                if (SDL_HasIntersection(&champi.corps, &mario.corps))
                {
                    champi.actif = 0;
                    if (!mario.estGrand)
                    {
                        mario.estGrand = 1;
                        mario.corps.h = BLOC_SIZE * 1.75;
                        mario.corps.y -= BLOC_SIZE;
                    }
                }
            }

            // sdl_clamp sert à limiter la valeur de cameraX entre 0 et MAP_LARGEUR * BLOC_SIZE - LONGUEUR_FENETRE
            mario.corps.x = SDL_clamp(mario.corps.x, 0, MAP_LARGEUR * BLOC_SIZE - mario.corps.w);
            cameraX = mario.corps.x + mario.corps.w / 2 - LONGUEUR_FENETRE / 2;
            cameraX = SDL_clamp(cameraX, 0, MAP_LARGEUR * BLOC_SIZE - LONGUEUR_FENETRE);

            SDL_SetRenderDrawColor(renderer, 129, 212, 255, 255);
            SDL_RenderClear(renderer);
            dessinerFondParallaxe(renderer, textures.background, cameraX);
            dessinerMap(renderer, cameraX, textures);
            dessinerEnnemis(renderer, cameraX, textures);
            dessinerCarapaces(renderer, cameraX, textures);
            dessinerEffets(renderer, cameraX, textures);

            SDL_Rect dst = {mario.corps.x - cameraX, mario.corps.y, mario.corps.w, mario.corps.h};
            SDL_RenderCopy(renderer, mario.estGrand ? textures.marioBig : textures.mario, NULL, &dst);

            if (champi.actif)
            {
                SDL_Rect dstCh = {champi.corps.x - cameraX, champi.corps.y, champi.corps.w, champi.corps.h};
                SDL_RenderCopy(renderer, textures.champignon, NULL, &dstCh);
            }

            afficherScore(renderer, &scoreJeu, police);
            afficherVies(renderer, &scoreJeu, textures);
            SDL_RenderPresent(renderer);
            SDL_Delay(16);
            break;
        }

        case ETAT_NIVEAU_TERMINE:
        {
            touches.gauche = touches.droite = touches.saut = 0;
            int choix = gererEvenementsNiveauTermine(&continuer, boutonsNiveauTermine, 2);

            if (current_level == NOMBRE_NIVEAUX)
            {
                afficherEcranFin(renderer, police);
                choix = ETAT_MENU;
                textures.background = chargerTextureBMP(renderer, "img/fond.bmp");
                current_level = 1;
            }
            else if (current_level == 3)
            {
                afficherMonde2(renderer, police);
                // mettre un delai de 3 secondes
                SDL_Delay(2000);
                textures.background = chargerTextureBMP(renderer, "img/fond2.bmp");
                choix = ETAT_JEU;
            }
            else if (current_level == 6)
            {
                afficherMonde3(renderer, police);
                // mettre un delai de 3 secondes
                SDL_Delay(2000);
                textures.background = chargerTextureBMP(renderer, "img/fond3.bmp");
                choix = ETAT_JEU;
            }

            if (choix == ETAT_JEU)
            {
                current_level++;
                mario.corps
                    .x = startX;
                mario.corps
                    .y = startY;
                scoreJeu.vies = 3;
                sauvegarderPartie(current_level);
                initialiserMap(current_level);
                initialiserCarapaces();
                initialiserEffets();
                etatJeu = ETAT_JEU;
            }
            else if (choix == ETAT_MENU)
            {
                etatJeu = ETAT_MENU;
            }

            SDL_SetRenderDrawColor(renderer, 0, 0, 100, 255);
            SDL_RenderClear(renderer);
            dessinerBoutons(renderer, boutonsNiveauTermine, 2, police);
            SDL_RenderPresent(renderer);
            SDL_Delay(16);
            break;
        }

        case ETAT_GAME_OVER:
        {
            mario.invincible = 0;
            mario.tempsInvincible = 0;
            champi.actif = 0;
            champi.vitesseY = 0;
            champi.corps.x = 0;
            champi.corps.y = 0;

            touches.gauche = touches.droite = touches.saut = 0;

            SDL_Event event;
            int choix = -1;

            while (SDL_PollEvent(&event))
            {
                if (event.type == SDL_QUIT)
                    continuer = 0;
                else if (event.type == SDL_MOUSEMOTION || event.type == SDL_MOUSEBUTTONDOWN)
                {
                    int x = event.motion.x;
                    int y = event.motion.y;
                    for (int i = 0; i < 2; i++)
                    {
                        boutonsGameOver[i].hover = pointDansRect(x, y, boutonsGameOver[i].rect);
                        if (event.type == SDL_MOUSEBUTTONDOWN && boutonsGameOver[i].hover)
                            choix = i;
                    }
                }
            }

            if (choix == 0)
            {
                current_level = 1;
                textures.background = chargerTextureBMP(renderer, "img/fond.bmp");
                mario.corps
                    .x = startX;
                mario.corps
                    .y = startY;
                scoreJeu.score = 0;
                scoreJeu.vies = 3;
                enSaut = 0;
                vitesseSaut = 0;
                touches.gauche = touches.droite = touches.saut = 0;
                initialiserMap(current_level);
                initialiserCarapaces();
                initialiserEffets();
                for (int i = 0; i < MAX_ENNEMIS; i++)
                {
                    carapaces[i].mobile = 0;
                    carapaces[i].direction = 0;
                    carapaces[i].tempsLancement = 0;
                }
                etatJeu = ETAT_JEU;
            }
            else if (choix == 1)
            {
                etatJeu = ETAT_MENU;
            }

            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderClear(renderer);
            dessinerBoutons(renderer, boutonsGameOver, 2, police);
            SDL_RenderPresent(renderer);
            SDL_Delay(16);
            break;
        }
        }
    }

    // Sauvegarde avant de quitter
    if (etatJeu == ETAT_JEU || etatJeu == ETAT_NIVEAU_TERMINE)
        sauvegarderPartie(current_level);

    free(boutonsNiveauTermine[0].texte);
    free(boutonsNiveauTermine[1].texte);
    free(boutonsGameOver[0].texte);
    free(boutonsGameOver[1].texte);
    libererTextures(textures);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(fenetre);
    TTF_CloseFont(police);
    TTF_Quit();
    SDL_Quit();
    return 0;
}
