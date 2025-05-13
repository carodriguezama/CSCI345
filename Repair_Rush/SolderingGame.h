#ifndef SOLDERING_GAME_H
#define SOLDERING_GAME_H

#include <SDL2/SDL.h>  // Include SDL2 for rendering and events
#include <iostream>

// Declare the SolderingGame class or functions if you are using an object-oriented approach.
// Example with a class:
class SolderingGame {
public:
    SolderingGame(SDL_Renderer* renderer);  // Constructor to initialize the mini-game
    ~SolderingGame();  // Destructor to clean up resources

    void run();  // Function to start and run the mini-game

private:
    SDL_Renderer* renderer_;  // SDL_Renderer to handle rendering
    bool isRunning;  // Flag to track if the mini-game is still running

    void handleEvents();  // Function to handle events like user input
    void update();  // Function to update the game logic
    void render();  // Function to render the game visuals
    void cleanUp();  // Function to clean up resources
};

// Or, if you prefer a procedural approach, you could define functions:
void runSolderingGame(SDL_Renderer* renderer);  // Function to run the soldering mini-game

#endif // SOLDERING_GAME_H
