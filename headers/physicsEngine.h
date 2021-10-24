#pragma once

#include "main.h"
#include "character.h"

class PhysicsEngine {

public:
    static bool checkCollisionWithBorders(const Character& pers);

private:
    PhysicsEngine();
    PhysicsEngine(const PhysicsEngine&);
    ~PhysicsEngine();
};