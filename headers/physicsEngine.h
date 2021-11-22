#pragma once

#include "main.h"
#include "character.h"

class PhysicsEngine {

public:
    PhysicsEngine();
    PhysicsEngine(const PhysicsEngine&);
    ~PhysicsEngine();
    
    static bool checkCollisionWithBorders(const Character& pers);
};