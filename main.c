#define SDL_MAIN_HANDLED
#include "fonction.h"

int main(int argc, char* argv[]) {
    SDL_Window* window = NULL;
    SDL_Surface* screenSurface = NULL;
    SDL_Surface* icon = NULL;
    SDL_Surface* menu = NULL;
    SDL_Rect positionMenu;

    // Initialisation de SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL n'a pas pu s'initialiser! Erreur: %s\n", SDL_GetError());
        return 1;
    }

    // Création de la fenêtre
    window = SDL_CreateWindow(
        "Mario",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        800, 600,
        SDL_WINDOW_SHOWN
    );

    if (window == NULL) {
        printf("La fenêtre n'a pas pu être créée! Erreur: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    // Chargement de l'icône
    icon = SDL_LoadBMP("../image/icon.bmp");
    if (icon) {
        SDL_SetWindowIcon(window, icon);
        SDL_FreeSurface(icon);
    }

    // Chargement de l'image du menu
    menu = SDL_LoadBMP("../projetMario/image/menu.bmp");
    if (!menu) {
        printf("Erreur chargement menu.bmp : %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // Récupération de la surface de la fenêtre
    screenSurface = SDL_GetWindowSurface(window);

    // Position initiale du menu
    positionMenu.x = 0;
    positionMenu.y = 0;

    // Boucle principale
    SDL_Event event;
    int running = 1;

    while (running) {
        // Gestion des événements
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    running = 0;
                    break;
                case SDL_KEYDOWN:
                    switch (event.key.keysym.sym) {
                        case SDLK_ESCAPE:
                            running = 0;
                            break;
                        case SDLK_UP:
                            positionMenu.y -= 10;
                            break;
                        case SDLK_DOWN:
                            positionMenu.y += 10;
                            break;
                        case SDLK_RETURN:
                            running = 0;
                            break;
                    }
                    break;
            }
        }

        // Effacer l'écran en blanc
        SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 255, 255, 255));

        // Affichage du menu
        SDL_BlitSurface(menu, NULL, screenSurface, &positionMenu);

        // Mettre à jour l'écran
        SDL_UpdateWindowSurface(window);
    }

    // Nettoyage
    SDL_FreeSurface(menu);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
