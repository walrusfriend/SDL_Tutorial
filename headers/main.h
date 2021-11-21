#pragma once
#include <iostream>
#include <string>
#include <iso646.h>
#include <memory>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include "setting.h"

// If debug from vs code
// #define DEBUG_VS_CODE

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

// Without this we have an error LNK2019: external unresolved symbol main
#undef main

// Global functions
void cerrErrorSDL(std::string&& str);