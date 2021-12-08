#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <iso646.h>
#include <memory>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include "settings.h"
#include "wtexture.h"

enum Directions {
    ERROR = -1,
    NONE,
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
void cerrErrorSDL(std::string&& str, std::string&& message);