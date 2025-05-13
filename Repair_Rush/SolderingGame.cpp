#include "SolderingGame.h"
#include <cstdlib>
#include <iostream>

SolderingGame::SolderingGame(SDL_Renderer* ren, MediaManager* media, int &jobCounter)
    : renderer(ren), mm(media), selected(nullptr), offsetX(0), offsetY(0), successfulJobs(jobCounter) {
    
    background = new Sprite(mm, "mbImg/motherboard.bmp", 0, 0);
    background->rect.w = 800;
    background->rect.h = 600;

    // Only the iron tool
    Sprite* iron = new Sprite(mm, "Images/Tools/Soldering_Iron.bmp");
    iron->rect = {300, 250, 64, 64};
    parts.push_back(iron);
    partName[iron] = "Iron";
    locked[iron] = false;

    font = TTF_OpenFont("./Fonts/BungeeSpice-Regular.ttf", 24);
    if (!font) {
        std::cerr << "Failed to load font in SolderingGame: " << TTF_GetError() << std::endl;
    }
}

SolderingGame::~SolderingGame() {
    for (auto part : parts) delete part;
    delete background;
    if (font) TTF_CloseFont(font);
}

bool SolderingGame::run() {
    bool running = true;
    SDL_Event event;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) return false;

            // Dragging iron
            if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
                int mx = event.button.x, my = event.button.y;
                Sprite* iron = parts[0];
                if (!locked[iron]) {
                    SDL_Rect r = iron->rect;
                    if (mx >= r.x && mx <= r.x + r.w && my >= r.y && my <= r.y + r.h) {
                        selected = iron;
                        offsetX = mx - r.x;
                        offsetY = my - r.y;
                    }
                }
            }

            if (event.type == SDL_MOUSEMOTION && selected != nullptr) {
                int mx = event.motion.x, my = event.motion.y;
                selected->rect.x = mx - offsetX;
                selected->rect.y = my - offsetY;
            }

            if (event.type == SDL_MOUSEBUTTONUP && event.button.button == SDL_BUTTON_LEFT) {
                selected = nullptr;
            }

            // Press S to complete soldering
            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_s && !gameFinished) {
                gameSuccess = true;
                gameFinished = true;
                successfulJobs++;
                std::cout << "Soldering complete. Total successful jobs: " << successfulJobs << std::endl;
            }

            // Exit
            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
                return false;
            }
        }

        // Render
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        background->render(renderer);
        for (auto p : parts) p->render(renderer);

        // Message
        if (gameFinished) {
            SDL_Color white = {255, 255, 255};
            SDL_Surface* surf;
            SDL_Texture* tex;
            SDL_Rect dst = {250, 500, 0, 0};
            if (gameSuccess) {
                surf = TTF_RenderText_Solid(font, "Solder Complete! Press Enter.", white);
            } else {
                surf = TTF_RenderText_Solid(font, "Solder Failed! Press Enter.", white);
            }
            tex = SDL_CreateTextureFromSurface(renderer, surf);
            dst.w = surf->w;
            dst.h = surf->h;
            SDL_RenderCopy(renderer, tex, NULL, &dst);
            SDL_FreeSurface(surf);
            SDL_DestroyTexture(tex);
        }

        SDL_RenderPresent(renderer);
        SDL_Delay(16);

        // Wait for Enter after result
        if (gameFinished) {
            while (SDL_PollEvent(&event)) {
                if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_RETURN) {
                    return gameSuccess;
                }
            }
        }
    }

    return gameSuccess;
}