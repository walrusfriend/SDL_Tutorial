#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <iso646.h>
#include <memory>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include "setting.h"
#include "wtexture.h"

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

extern bool vscodeDebug;

// Send error to the cerr output stream
void cerrErrorSDL(std::string&& str);           