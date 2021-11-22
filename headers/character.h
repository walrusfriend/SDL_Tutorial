#pragma once

#include "main.h"
#include <vector>

struct SpriteInfo {
    std::vector<SDL_Rect> walkSprite;
    int rotateLeft;
    int roteteRight;
    int rotateUp;
    int rotateDown;
    int numberOfStepInSprite;
    int useWalkClip;
    int clipsNumber;
};

class Character {
public:
    // Size settings
    SDL_Rect size;
    SDL_Rect prevSize;
    int stepSize;

    // Texture settings
    SDL_Texture* texture;
    int walkTextureSize;
    int attackTextureSize;

    // Info about character texture
    SpriteInfo spriteInfo;

    // Physics parameters
    float velocity;
    float velocityX;
    float velocityY;
    bool isMove;

public:
    Character();
    ~Character();

    void move(double dt);
    void scale(int zoom);
    void checkCollisionWithBorders();

private:
    const uint8_t* keyboardState;
};