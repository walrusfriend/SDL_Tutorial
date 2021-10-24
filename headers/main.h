#pragma once
#include <iostream>
#include <string>
#include <iso646.h>
#include <memory>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

enum Directions {
    UP,
    DOWN,
    LEFT,
    RIGHT,
    UP_LEFT,
    UP_RIGHT,
    DOWN_LEFT,
    DOWN_RIGHT
};

// Set screen size
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

// Without this we have an error LNK2019: external unresolved symbol main
#undef main