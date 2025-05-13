#pragma once
#include "Sprite.h"
#include "MediaManager.h"
#include "SDL.h"
#include "SDL_ttf.h"
#include <vector>
#include <map>

using namespace std;

class SolderingGame {
    SDL_Renderer* renderer;
    MediaManager* mm;
    vector<Sprite*> parts;
    Sprite* background;
    Sprite* selected;
    TTF_Font* font;
    int offsetX, offsetY;
    map<Sprite*, bool> locked;
    map<Sprite*, string> partName;
    vector<string> placementOrder;
    bool gameFinished = false;
    bool gameSuccess = false;
    int& successfulJobs;

public:
    SolderingGame(SDL_Renderer* ren, MediaManager* media, int &jobCounter);
    bool run(); // needed to change for errors
    ~SolderingGame();
};
