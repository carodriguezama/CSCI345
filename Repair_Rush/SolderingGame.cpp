#include "SolderingGame.h"

// Constructor
SolderingGame::SolderingGame(SDL_Renderer* renderer) : renderer_(renderer), isRunning(true) {
    // Initialize any game-specific resources (textures, fonts, etc.)
}

// Destructor
SolderingGame::~SolderingGame() {
    cleanUp();  // Make sure to clean up any resources when done
}

// Main game loop
void SolderingGame::run() {
    while (isRunning) {
        handleEvents();
        update();
        render();
    }
}

// Handle events (keyboard inputs, etc.)
void SolderingGame::handleEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            isRunning = false;  // Exit game loop
        }
        // Handle other key presses or events here
    }
}

// Update game logic (e.g., movement, game state)
void SolderingGame::update() {
    // Update game state, player movement, etc.
}

// Render the game
void SolderingGame::render() {
    SDL_RenderClear(renderer_);
    // Render game elements like sprites or background
    SDL_RenderPresent(renderer_);
}

// Clean up any allocated resources
void SolderingGame::cleanUp() {
    // Free any allocated memory, textures, etc.
}

// Procedural approach (if you're not using the class):
void runSolderingGame(SDL_Renderer* renderer) {
    bool isRunning = true;

    while (isRunning) {
        // Handle events, update game logic, and render
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                isRunning = false;
            }
        }
        // Update game state
        // Render game elements
        SDL_RenderClear(renderer);
        SDL_RenderPresent(renderer);
    }
}
