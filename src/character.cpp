#include "../headers/character.h"
#include "../headers/physicsEngine.h"

Character::Character() {
    x = 0;
    y = 0;
    width = 0;
    height = 0;
    stepSize = 0;
    texture = nullptr;
}

Character::~Character() {
}

/**
 * Move character in a certain direction
 * @param direction Direction to move
 * @return          None
 */
void Character::move(int direction) {
    // When we step on diagonal direction, an object move at a distance of hypotenuse
    // Therefore, need to define the value of X and Y components
    float rootOf2 = 1.41421356237f;             // The root of 2 required to calculate the diagonal distance
    int diagStepSize = stepSize / rootOf2;      // We must convert from float to int
    int oldStep = stepSize;
    switch (direction) {
        case UP:
            y -= stepSize;
            if (PhysicsEngine::checkCollisionWithBorders(*this))
                y += stepSize;
            break;
        case DOWN:
            y += stepSize;
            if (PhysicsEngine::checkCollisionWithBorders(*this))
                y -= stepSize;
            break;
        case LEFT:
            x -= stepSize;
            if (PhysicsEngine::checkCollisionWithBorders(*this))
                x += stepSize;
            break;
        case RIGHT:
            x += stepSize;
            if (PhysicsEngine::checkCollisionWithBorders(*this))
                x -= stepSize;
            break;
        case UP_LEFT:
            stepSize = diagStepSize;
            move(UP);
            move(LEFT);
            stepSize = oldStep;
            break;
        case UP_RIGHT:
            stepSize = diagStepSize;
            move(UP);
            move(RIGHT);
            stepSize = oldStep;
            break;
        case DOWN_LEFT:
            stepSize = diagStepSize;
            move(DOWN);
            move(LEFT);
            stepSize = oldStep;
            break;
        case DOWN_RIGHT:
            stepSize = diagStepSize;
            move(DOWN);
            move(RIGHT);
            stepSize = oldStep;
            break;
        default:
            std::cerr << "ERROR: Character movement error!" << std::endl;
            break;
    }
}