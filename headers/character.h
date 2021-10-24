#pragma once

#include "main.h"

class Character {

public:
    int x;
    int y;
    int width;
    int height;
    int stepSize;
    SDL_Texture* texture;

public:
    Character();
    ~Character();

    void move(int direction);
};