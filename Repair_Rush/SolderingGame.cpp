#include "SolderingGame.h"
#include <cstdlib>
#include <iostream>

SolderingGame::SolderingGame(SDL_Renderer* ren, MediaManager* media, int &jobCounter)
    : renderer(ren), mm(media), selected(nullptr), offsetX(0), offsetY(0), successfulJobs(jobCounter) {
    
    background = new Sprite(mm, "mbImg/motherboard.bmp", 0, 0);
    background->rect.w = 800;
    background->rect.h = 600;

    parts.push_back(new Sprite(mm, "Images/Tools/Soldering_Iron.bmp"));

    partName[parts[0]] = "Iron";


    for (auto part : parts)
        locked[part] = false;

}

SolderingGame::~SolderingGame() {
    for (auto part : parts) delete part;
    delete background;
    if (font) TTF_CloseFont(font);
}

void SolderingGame::run() {
    bool running = true;
    SDL_Event event;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) return;

            if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
                int mx = event.button.x, my = event.button.y;
                for (auto part : parts) {
                    if (locked[part]) continue;
                    SDL_Rect r = part->rect;
                    if (mx >= r.x && mx <= r.x + r.w && my >= r.y && my <= r.y + r.h) {
                        selected = part;
                        offsetX = mx - r.x;
                        offsetY = my - r.y;
                        break;
                    }
                }
            }

            if (event.type == SDL_MOUSEMOTION && selected != nullptr) {
                int mx = event.motion.x, my = event.motion.y;
                selected->rect.x = mx - offsetX;
                selected->rect.y = my - offsetY;
            }

            if (event.type == SDL_MOUSEBUTTONUP && event.button.button == SDL_BUTTON_LEFT) {
                if (selected && !locked[selected]) {
                    int x = selected->rect.x;
                    int y = selected->rect.y;

                    if (abs(x - 200) < 20 && abs(y - 150) < 20) {
                        selected->rect.x = 200;
                        selected->rect.y = 150;
                        locked[selected] = true;
                        placementOrder.push_back(partName[selected]);
                        cout << partName[selected] << " placed at (200,150)\n";
                    }
                }
                selected = nullptr;
            }

            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
                running = false;
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        background->render(renderer);
        for (auto p : parts) p->render(renderer);

        if (placementOrder.size() == 3) {
            gameFinished = true;
            vector<string> correctOrder = {"Iron", "Wire", "Chip"};
            if (placementOrder == correctOrder) gameSuccess = true;
        }

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

        if (gameFinished) {
            while (SDL_PollEvent(&event)) {
                if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_RETURN) {
                    if (gameSuccess)
                        successfulJobs++;
                    cout << "Total successful jobs: " << successfulJobs << endl;
                    running = false;
                }
            }
        }
    }
}
