#include "Game.h"
#include "Sprite.h"
#include "AnimatedSprite.h"
#include "SolderingGame.cpp" // includes the mini-game
#include <SDL.h>

class MyGame : public Game {
public:
    Sprite* screen1;
    Sprite* screen2;
    Sprite* screen3;
    Sprite* sign;               // Soldering sign
    int which = 1;
    bool solderingMiniGameDone = false; // Track completion

    MyGame() {
        screen1 = new Sprite(getMM(), "Images/Room.bmp", 0, 0);
        screen2 = new Sprite(getMM(), "Images/Repair_Shop.bmp", 0, 0);
        screen3 = new Sprite(getMM(), "Images/Outside.bmp", 0, 0);

        // Load the soldering sign
        sign = new Sprite(getMM(), "Images/UI/soldering_sign.png", 400, 240); // Adjust pos as needed
    }

    ~MyGame() {
        delete screen1;
        delete screen2;
        delete screen3;
        delete sign;
    }

    void loop() {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) setDone(true);
            if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                    case SDLK_ESCAPE: setDone(true); break;
                    case SDLK_1: which = 1; break;
                    case SDLK_2: which = 2; break;
                    case SDLK_3: which = 3; break;

                    case SDLK_i:
                        if (which == 2 && !solderingMiniGameDone) {
                            // Start the mini-game
                            SolderingGame game(getRen(), getMM());
                            game.play();
                            solderingMiniGameDone = true; // Mark it complete
                        }
                        break;
                }
            }
        }

        SDL_RenderClear(getRen());

        if (which == 1) {
            screen1->render(getRen());
        } else if (which == 2) {
            screen2->render(getRen());

            // Show the sign only if not done
            if (!solderingMiniGameDone) {
                sign->render(getRen());
            }
        } else if (which == 3) {
            screen3->render(getRen());
        }

        SDL_RenderPresent(getRen());
    }
};

int main(int argc, char* argv[]) {
    MyGame g;
    g.run();
    return 0;
}
