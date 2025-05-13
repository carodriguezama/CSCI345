#include "GameOver.h"
#include <iostream>

void drawGameOverText(SDL_Renderer *renderer, TTF_Font *font, const char *message, int x, int y) {
    SDL_Color red = {255, 0, 0};
    SDL_Surface *surface = TTF_RenderText_Solid(font, message, red);
    if (!surface) {
        std::cout << "Error rendering text: " << TTF_GetError() << std::endl;
        return;
    }

    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    if (!texture) {
        std::cout << "Error creating texture: " << SDL_GetError() << std::endl;
        return;
    }

    SDL_Rect dst = {x, y, 0, 0};
    SDL_QueryTexture(texture, NULL, NULL, &dst.w, &dst.h);
    SDL_RenderCopy(renderer, texture, NULL, &dst);
    SDL_DestroyTexture(texture);
}

void showGameOverScreen(SDL_Renderer *renderer, TTF_Font *font) {
    bool isRunning = true;
    SDL_Event event;

    while (isRunning) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT || 
               (event.type == SDL_KEYDOWN && 
               (event.key.keysym.sym == SDLK_RETURN || event.key.keysym.sym == SDLK_ESCAPE))) {
                isRunning = false;
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        drawGameOverText(renderer, font, "Day Over!", 100, 100);
        drawGameOverText(renderer, font, "All Jobs Complete!", 100, 150);
        drawGameOverText(renderer, font, "Press ESC", 100, 250);

        SDL_RenderPresent(renderer);
        SDL_Delay(16);  // ~60 FPS
    }
}