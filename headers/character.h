#pragma once

#include <vector>

#include "main.h"

struct SpriteInfo {
    std::vector<SDL_Rect> walkSprite;

    // Movement section
    int rotateLeft;
    int rotateRight;
    int rotateUp;
    int rotateDown;
    int currentRotation;
    int numberOfStepInSprite;
    int useWalkClip;
    int clipsNumber;
    int moveFramerate;          // Number of ms between frames
    SDL_RendererFlip flip;

    // Attack section
};

class Character {
public:
    // Current size and position
    SDL_Rect size;          
    // Previous size and position used to return to the old state after failing a collision check or something else
    SDL_Rect prevSize;      

    // Texture settings
    std::unique_ptr<WTexture> texture;
    int walkTextureSize;
    int attackTextureSize;

    // Info about character sprite
    SpriteInfo spriteInfo;

    // Physics parameters
    float velocity;
    bool isMove;
    bool isAttack;

    // Move direction
    int moveDirection;

public:
    Character();
    ~Character();

    void move(double dt);
    void scale(int zoom);
    void checkCollisionWithBorders();
    void update();
    void attack();

private:
    const uint8_t* keyboardState;
    int scaleFactor;
};