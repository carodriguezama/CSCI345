#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <SDL_main.h>
#include <iostream>
#include <cstdlib>
#include <ctime>

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const int IRON_WIDTH = 50;
const int IRON_HEIGHT = 50;

enum class GameState {
    MAIN_SCENE,
    MINI_GAME
};

SDL_Texture* loadTexture(const std::string& file, SDL_Renderer* renderer) {
    SDL_Surface* surface = SDL_LoadBMP(file.c_str());
    if (!surface) {
        std::cerr << "Failed to load: " << file << " SDL_Error: " << SDL_GetError() << std::endl;
        return nullptr;
    }
    SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGB(surface->format, 0, 255, 0));  // Make green transparent
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    return texture;
}

bool isNear(SDL_Rect a, SDL_Rect b, int distance = 50) {
    int dx = (a.x + a.w / 2) - (b.x + b.w / 2);
    int dy = (a.y + a.h / 2) - (b.y + b.h / 2);
    return (dx * dx + dy * dy) <= (distance * distance);
}

int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_VIDEO);
    std::srand(static_cast<unsigned>(time(0)));

    SDL_Window* window = SDL_CreateWindow("Soldering Game",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    SDL_Texture* repairShopTex = loadTexture("Repair_Shop.bmp", renderer);
    SDL_Texture* motherboardTex = loadTexture("SolderingMotherBoard.bmp", renderer);
    SDL_Texture* ironTex = loadTexture("Soldering_Iron.bmp", renderer);
    SDL_Texture* signTex = loadTexture("Solder_Sign.bmp", renderer);

    if (!repairShopTex || !motherboardTex || !ironTex || !signTex) {
        std::cerr << "Failed to load textures\n";
        SDL_Quit();
        return 1;
    }

    SDL_Rect ironRect = { 375, 275, IRON_WIDTH, IRON_HEIGHT };
    SDL_Rect signRect = { 600, 400, 60, 60 };

    GameState gameState = GameState::MAIN_SCENE;
    SDL_Point solderPoint = {147, 136};
    const int solderProximity = 30;
    bool solderingActive = false;
    Uint32 solderStartTime = 0;
    Uint32 solderRequiredTime = 0;
    bool mouseHeld = false;
    bool quit = false;
    SDL_Event e;

    while (!quit) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT)
                quit = true;

            if (e.type == SDL_KEYDOWN) {
                switch (e.key.keysym.sym) {
                    case SDLK_w: ironRect.y -= 5; break;
                    case SDLK_s: ironRect.y += 5; break;
                    case SDLK_a: ironRect.x -= 5; break;
                    case SDLK_d: ironRect.x += 5; break;
                    case SDLK_e:
                        if (gameState == GameState::MAIN_SCENE && isNear(ironRect, signRect)) {
                            std::cout << "Mini-game starts!\n";
                            gameState = GameState::MINI_GAME;
                            ironRect = { 375, 275, IRON_WIDTH, IRON_HEIGHT };
                        }
                        break;
                }
            }

            if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT)
                mouseHeld = true;
            else if (e.type == SDL_MOUSEBUTTONUP && e.button.button == SDL_BUTTON_LEFT)
                mouseHeld = false;
        }

        SDL_RenderClear(renderer);

        if (gameState == GameState::MAIN_SCENE) {
            SDL_RenderCopy(renderer, repairShopTex, nullptr, nullptr);
            SDL_RenderCopy(renderer, signTex, nullptr, &signRect);
        } else if (gameState == GameState::MINI_GAME) {
            SDL_RenderCopy(renderer, motherboardTex, nullptr, nullptr);

            int dx = (ironRect.x + ironRect.w / 2) - solderPoint.x;
            int dy = (ironRect.y + ironRect.h / 2) - solderPoint.y;
            int distSq = dx * dx + dy * dy;

            if (distSq < solderProximity * solderProximity) {
                if (!solderingActive && mouseHeld) {
                    solderingActive = true;
                    solderStartTime = SDL_GetTicks();
                    solderRequiredTime = 1000 + rand() % 3000;
                    std::cout << "Soldering started! Hold for " << solderRequiredTime << " ms\n";
                }

                if (solderingActive) {
                    Uint32 elapsed = SDL_GetTicks() - solderStartTime;
                    if (!mouseHeld) {
                        solderingActive = false;
                        std::cout << "Released too early! Restart.\n";
                    } else {
                        float progress = (float)elapsed / solderRequiredTime;
                        if (progress > 1.0f) {
                            progress = 1.0f;
                            solderingActive = false;
                            std::cout << "Soldering complete!\n";
                            gameState = GameState::MAIN_SCENE;  // Exit mini-game after soldering
                            SDL_RenderClear(renderer); // Clear screen
                            SDL_RenderCopy(renderer, repairShopTex, nullptr, nullptr); // Show main screen
                            SDL_RenderPresent(renderer);
                            SDL_Delay(1000); // Delay before returning to main scene
                        }

                        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
                        SDL_SetRenderDrawColor(renderer, 255, 0, 0, static_cast<Uint8>(progress * 150));
                        SDL_Rect screenRect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
                        SDL_RenderFillRect(renderer, &screenRect);
                    }
                }
            } else {
                solderingActive = false;
            }
        }

        SDL_RenderCopy(renderer, ironTex, nullptr, &ironRect);
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(repairShopTex);
    SDL_DestroyTexture(motherboardTex);
    SDL_DestroyTexture(ironTex);
    SDL_DestroyTexture(signTex);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
