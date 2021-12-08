#pragma once

#include <vector>

#include "main.h"
#include "graphicsEngine.h"
#include "physicsEngine.h"
#include "character.h"

class Application 
{
public:
    Application();
    ~Application();

    bool init();
    void cleanup();
    void playerSetup(std::unique_ptr<Character>& player);
    void run();

private:
    std::unique_ptr<GraphicsEngine> gpxEngine;
    std::unique_ptr<Character> player;

    const uint8_t* keyboardState;

private:
    int definePlayerMovementDirection();
};