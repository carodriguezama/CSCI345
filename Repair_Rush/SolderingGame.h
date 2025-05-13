#ifndef SOLDERING_GAME_H
#define SOLDERING_GAME_H

#include "Animation.h"
#include "ControlScreen.h"
#include "Game.h"
#include "SDL.h"
#include "ServiceTable.h"
#include "Sound.h"
#include "Sprite.h"
#include "MotherboardMinigame.h"
#include "Text.h"
#include "SolderingGame.h"
#include "Tile.h"
#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <vector>

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const int IRON_WIDTH = 50;
const int IRON_HEIGHT = 50;

enum class GameState {
    MAIN_SCENE,
    MINI_GAME
};

// SolderingGame class declaration
class SolderingGame {
public:
    SolderingGame(SDL_Renderer* renderer);
    ~SolderingGame();

    void run();  // Main game loop
    void handleEvents(SDL_Event& e);  // Handle user input
    void update();  // Update game state and logic
    void render();  // Render the current game state

private:
    SDL_Renderer* renderer_;  // SDL_Renderer for drawing
    SDL_Texture* repairShopTex_;  // Texture for the repair shop
    SDL_Texture* motherboardTex_;  // Texture for the motherboard
    SDL_Texture* ironTex_;  // Texture for the soldering iron
    SDL_Texture* signTex_;  // Texture for the sign

    SDL_Rect ironRect_;  // Position of the soldering iron
    SDL_Rect signRect_;  // Position of the sign to trigger the mini-game

    GameState gameState_;  // Current game state (MAIN_SCENE or MINI_GAME)
    SDL_Point solderPoint_;  // The point to solder to
    bool solderingActive_;  // Flag to track if soldering is active
    Uint32 solderStartTime_;  // Start time of soldering
    Uint32 solderRequiredTime_;  // Required time to complete soldering
    bool mouseHeld_;  // Flag to track mouse hold state
    bool solderComp_;  // Flag to track if soldering is completed

    // Helper functions
    SDL_Texture* loadTexture(const std::string& file);  // Load a texture from a file
    bool isNear(SDL_Rect a, SDL_Rect b, int distance = 50);  // Check if two rectangles are near
};

#endif // SOLDERING_GAME_H
