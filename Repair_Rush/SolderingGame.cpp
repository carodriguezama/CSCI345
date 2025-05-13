#include "SolderingGame.h"
#include <cstdlib>
#include <iostream>
#include <map>
#include <vector>

SolderingGame::SolderingGame(SDL_Renderer* ren, MediaManager* media, int &jobCounter)
    : renderer(ren), mm(media), selectedIndex(-1), offsetX(0), offsetY(0), successfulJobs(jobCounter) {

    srand(static_cast<unsigned int>(time(nullptr)));  // for delay randomness

    // Load background texture
    backgroundTex = mm->loadTexture("mbImg/motherboard.bmp");
    backgroundRect = {0, 0, 800, 600};

    // Load the parts textures
    SDL_Texture* ironTex = mm->loadTexture("Images/Tools/Soldering_Iron.bmp");

    // Apply red tint to the soldering iron texture
    SDL_SetTextureColorMod(ironTex, 255, 0, 0);

    partTextures.push_back(ironTex);
    partRects.push_back({100, 100, 64, 64});  // initial position and size
    partNames[0] = "Iron";
    partLocked["Iron"] = false;
}

SolderingGame::~SolderingGame() {
    // Cleanup loaded textures
    for (auto tex : partTextures) {
        SDL_DestroyTexture(tex);
    }
    SDL_DestroyTexture(backgroundTex);
    if (font) TTF_CloseFont(font);
}

void SolderingGame::run() {
    bool running = true;
    SDL_Event event;

    // Random delay before starting the game
    int delaySeconds = 1 + rand() % 4;
    std::cout << "Delaying game start by " << delaySeconds << " seconds...\n";
    SDL_Delay(delaySeconds * 1000);

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) return;

            if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
                int mx = event.button.x, my = event.button.y;
                for (size_t i = 0; i < partRects.size(); ++i) {
                    SDL_Rect r = partRects[i];
                    if (!partLocked[partNames[i]] &&
                        mx >= r.x && mx <= r.x + r.w &&
                        my >= r.y && my <= r.y + r.h) {
                        selectedIndex = i;
                        offsetX = mx - r.x;
                        offsetY = my - r.y;
                        break;
                    }
                }
            }

            if (event.type == SDL_MOUSEMOTION && selectedIndex != -1) {
                int mx = event.motion.x, my = event.motion.y;
                partRects[selectedIndex].x = mx - offsetX;
                partRects[selectedIndex].y = my - offsetY;
            }

            if (event.type == SDL_MOUSEBUTTONUP && event.button.button == SDL_BUTTON_LEFT && selectedIndex != -1) {
                int x = partRects[selectedIndex].x;
                int y = partRects[selectedIndex].y;
                if (abs(x - 200) < 20 && abs(y - 150) < 20) {
                    partRects[selectedIndex].x = 200;
                    partRects[selectedIndex].y = 150;
                    std::string name = partNames[selectedIndex];
                    partLocked[name] = true;
                    partOrder.push_back(name);
                    std::cout << name << " placed at (200,150)\n";
                }
                selectedIndex = -1;
            }

            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
                running = false;
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Render background
        SDL_RenderCopy(renderer, backgroundTex, nullptr, &backgroundRect);

        // Render all parts
        for (size_t i = 0; i < partTextures.size(); ++i) {
            SDL_RenderCopy(renderer, partTextures[i], nullptr, &partRects[i]);
        }

        // Check if the game is finished
        if (partOrder.size() == 3) {
            gameFinished = true;
            std::vector<std::string> correctOrder = {"Iron", "Wire", "Chip"};
            if (partOrder == correctOrder) gameSuccess = true;
        }

        // Display completion message
        if (gameFinished) {
            SDL_Color white = {255, 255, 255};
            SDL_Surface* surf;
            SDL_Texture* tex;
            SDL_Rect dst = {250, 500, 0, 0};
            if (gameSuccess) {
                surf = TTF_RenderText_Solid(font, "Solder Complete! Press Enter.", white);
            } else {
                surf = TTF_RenderText_Solid(font, "Incorrect Order! Press Enter.", white);
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

        // Handle game finish and job tracking
        if (gameFinished) {
            while (SDL_PollEvent(&event)) {
                if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_RETURN) {
                    if (gameSuccess)
                        successfulJobs++;
                    std::cout << "Total successful jobs: " << successfulJobs << std::endl;
                    running = false;
                }
            }
        }
    }
}
