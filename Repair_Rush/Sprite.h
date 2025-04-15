#pragma once
#include "MediaManager.h"
#include "SDL.h"
#include "Tile.h"
#include <vector>

using namespace std;

/*
Removing acceleration and random velocity generation. Making default velocity 0
Renaming isActive to be isTouching to see if the cursor is over a tool
Made new function isActive to return value of new member bool active
Made functions for setting active, vy, and vx
*/

class Sprite : public Tile {
  float x, y;
  bool active;
  bool isWalkingSideways = false;
  bool isWalkingVertical = false;
  bool isAnimated = false;

public:
  bool isTouching(Sprite &tool) {
    // function to check if the user has selected the tool
    if (this == &tool)
      return false; // cursor can't select itself
    return ((tool.x < x && x < tool.x + tool.rect.w) &&
            (tool.y < y && y < tool.y + tool.rect.h));
  }

  bool isActive() { return active; }

  void setActive(bool b) { active = b; }

  void setx(float x0) {
    x += x0;
    isWalkingSideways = true;
  }

  void sety(float y0) {
    y += y0;
    isWalkingVertical = true;
  }

  Sprite(MediaManager *mm, string fname = "face.bmp", float x0 = 0,
         float y0 = 0, float vx0 = 0, float vy0 = 0)
      : Tile(mm, fname) {
    active = true;
    x = x0;
    y = y0;

    // Only enable animation for this specific sprite sheet
    if (fname.find("spriteAnimation") != string::npos) {
      isAnimated = true;

      frameWidth = 550;
      frameHeight = 850;
      totalFrames = 2;
      currentFrame = 0;
      frameDelay = 100;
      lastFrameTime = 0;

      // Fill frame rects (4 per row, 2 rows)
      for (int i = 0; i < totalFrames; ++i) {
        SDL_Rect r = {(i % 4) * frameWidth, (i / 4) * frameHeight, frameWidth,
                      frameHeight};
        frames.push_back(r);
      }

      rect.w = frameWidth / 2;
      rect.h = frameHeight / 2;
    } else {
      // Default to full size for static characters
      SDL_QueryTexture(t, NULL, NULL, &rect.w, &rect.h);
    }
  }

  virtual void loop(float dt) {
    rect.x = x;
    rect.y = y;

    if (isAnimated) {
      rect.w = frameWidth / 4;
      rect.h = frameHeight / 4;

      if (isWalkingSideways || isWalkingVertical) {
        updateAnimation();
        isWalkingSideways = false;
        isWalkingVertical = false;
      }
    }
  }

  void render(SDL_Renderer *ren) override {
    SDL_RendererFlip flip = facingLeft ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;

    if (isAnimated) {
      SDL_RenderCopyEx(ren, t, &frames[currentFrame], &rect, 0, NULL, flip);
    } else {
      SDL_RenderCopyEx(ren, t, NULL, &rect, 0, NULL, flip);
    }
  }

  bool facingLeft = false;
  bool moving = false;
  void setDirection(bool left) { facingLeft = left; }
  void walkingVertical(bool vert) { moving = vert; }

private:
  // Animation frame data
  vector<SDL_Rect> frames;
  int frameWidth, frameHeight;
  int currentFrame;
  int totalFrames;
  int frameDelay; // ms
  Uint32 lastFrameTime;

  void updateAnimation() {
    Uint32 now = SDL_GetTicks();
    if (now > lastFrameTime + frameDelay) {
      currentFrame = (currentFrame + 1) % totalFrames;
      lastFrameTime = now;
    }
  }
};
