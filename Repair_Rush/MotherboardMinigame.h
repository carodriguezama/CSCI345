#pragma once
#include "Sprite.h"
#include "MediaManager.h"
#include "SDL.h"
#include "SDL_ttf.h"
#include <vector>

using namespace std;

class MotherboardMinigame {
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
    MotherboardMinigame(SDL_Renderer* ren, MediaManager* media, int &jobCounter);
    void run();
    ~MotherboardMinigame();
};