// - c
#include "ControlScreen.h"
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

// A basic function to render text to the screen at a given position
void drawText(SDL_Renderer* renderer, TTF_Font* font, const char* message, int x, int y) {
    SDL_Color white = {255, 255, 255};
    SDL_Surface* surface = TTF_RenderText_Solid(font, message, white);
    if (surface == nullptr) {
        std::cout << "Error rendering text surface: " << TTF_GetError() << std::endl;
        return;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (texture == nullptr) {
        std::cout << "Error creating text texture: " << SDL_GetError() << std::endl;
        SDL_FreeSurface(surface);
        return;
    }

    SDL_Rect dst;
    dst.x = x;
    dst.y = y;
    dst.w = surface->w;
    dst.h = surface->h;

    SDL_FreeSurface(surface);

    SDL_RenderCopy(renderer, texture, NULL, &dst);
    SDL_DestroyTexture(texture);
}

void showControlsScreen(SDL_Renderer* renderer, TTF_Font* font) {
    bool isRunning = true;
    SDL_Event event;

    while (isRunning) {
        // handle input
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                isRunning = false;
            }
            if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_ESCAPE || event.key.keysym.sym == SDLK_RETURN) {
                    isRunning = false;
                }
            }
        }

        // background color
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // draw controls
        drawText(renderer, font, "Controls", 100, 50);
        drawText(renderer, font, "Move: WASD or Arrow Keys", 100, 100);
        drawText(renderer, font, "Action: Spacebar or Enter", 100, 140);
        drawText(renderer, font, "Go to Repair Shop: R", 100, 180);
        drawText(renderer, font, "Go to Tool Cabinet: C", 100, 220);
        drawText(renderer, font, "Go to Benchtop: B", 100, 260);
        drawText(renderer, font, "Drop Item: Shift", 100, 300);
        drawText(renderer, font, "Press ESC or Enter to go back", 100, 360);

        SDL_RenderPresent(renderer);
        SDL_Delay(16); // ~60fps
    }
}
// - c end