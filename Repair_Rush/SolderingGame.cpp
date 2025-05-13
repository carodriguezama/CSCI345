#include "SolderingGame.h"

SolderingGame::SolderingGame(SDL_Renderer* renderer) : renderer_(renderer), gameState_(GameState::MAIN_SCENE), solderingActive_(false), mouseHeld_(false), solderComp_(false) {
    // Initialize SDL textures
    repairShopTex_ = loadTexture("Repair_Shop.bmp");
    motherboardTex_ = loadTexture("SolderingMotherBoard.bmp");
    ironTex_ = loadTexture("Soldering_Iron.bmp");
    signTex_ = loadTexture("Solder_Sign.bmp");

    if (!repairShopTex_ || !motherboardTex_ || !ironTex_ || !signTex_) {
        std::cerr << "Failed to load textures\n";
        SDL_Quit();
        throw std::runtime_error("Texture load failure");
    }

    // Initialize rects
    ironRect_ = {375, 275, IRON_WIDTH, IRON_HEIGHT};
    signRect_ = {600, 400, 60, 60};
    solderPoint_ = {147, 136};
}

SolderingGame::~SolderingGame() {
    // Clean up SDL resources
    SDL_DestroyTexture(repairShopTex_);
    SDL_DestroyTexture(motherboardTex_);
    SDL_DestroyTexture(ironTex_);
    SDL_DestroyTexture(signTex_);
}

SDL_Texture* SolderingGame::loadTexture(const std::string& file) {
    SDL_Surface* surface = SDL_LoadBMP(file.c_str());
    if (!surface) {
        std::cerr << "Failed to load: " << file << " SDL_Error: " << SDL_GetError() << std::endl;
        return nullptr;
    }
    SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGB(surface->format, 0, 255, 0));  // Make green transparent
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer_, surface);
    SDL_FreeSurface(surface);
    return texture;
}

bool SolderingGame::isNear(SDL_Rect a, SDL_Rect b, int distance) {
    int dx = (a.x + a.w / 2) - (b.x + b.w / 2);
    int dy = (a.y + a.h / 2) - (b.y + b.h / 2);
    return (dx * dx + dy * dy) <= (distance * distance);
}

void SolderingGame::run() {
    SDL_Event e;
    bool quit = false;

    while (!quit) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT)
                quit = true;

            handleEvents(e);
        }

        update();
        render();
    }
}

void SolderingGame::handleEvents(SDL_Event& e) {
    if (e.type == SDL_KEYDOWN) {
        switch (e.key.keysym.sym) {
            case SDLK_w: ironRect_.y -= 5; break;
            case SDLK_s: ironRect_.y += 5; break;
            case SDLK_a: ironRect_.x -= 5; break;
            case SDLK_d: ironRect_.x += 5; break;
            case SDLK_e:
                if (gameState_ == GameState::MAIN_SCENE && isNear(ironRect_, signRect_) && !solderComp_) {
                    gameState_ = GameState::MINI_GAME;
                    ironRect_ = {375, 275, IRON_WIDTH, IRON_HEIGHT};
                }
                break;
        }
    }

    if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT)
        mouseHeld_ = true;
    else if (e.type == SDL_MOUSEBUTTONUP && e.button.button == SDL_BUTTON_LEFT)
        mouseHeld_ = false;
}

void SolderingGame::update() {
    if (gameState_ == GameState::MINI_GAME) {
        int dx = (ironRect_.x + ironRect_.w / 2) - solderPoint_.x;
        int dy = (ironRect_.y + ironRect_.h / 2) - solderPoint_.y;
        int distSq = dx * dx + dy * dy;

        if (distSq < 30 * 30) {
            if (!solderingActive_ && mouseHeld_) {
                solderingActive_ = true;
                solderStartTime_ = SDL_GetTicks();
                solderRequiredTime_ = 1000 + rand() % 3000;
            }

            if (solderingActive_) {
                Uint32 elapsed = SDL_GetTicks() - solderStartTime_;
                if (!mouseHeld_) {
                    solderingActive_ = false;
                } else {
                    float progress = (float)elapsed / solderRequiredTime_;
                    if (progress > 1.0f) {
                        progress = 1.0f;
                        solderingActive_ = false;
                        solderComp_ = true;
                        gameState_ = GameState::MAIN_SCENE;
                    }
                }
            }
        } else {
            solderingActive_ = false;
        }
    }
}

void SolderingGame::render() {
    SDL_RenderClear(renderer_);

    if (gameState_ == GameState::MAIN_SCENE) {
        SDL_RenderCopy(renderer_, repairShopTex_, nullptr, nullptr);
        if (!solderComp_) {
            SDL_RenderCopy(renderer_, signTex_, nullptr, &signRect_);
        }
    } else if (gameState_ == GameState::MINI_GAME) {
        SDL_RenderCopy(renderer_, motherboardTex_, nullptr, nullptr);
    }

    SDL_RenderCopy(renderer_, ironTex_, nullptr, &ironRect_);
    SDL_RenderPresent(renderer_);
}
