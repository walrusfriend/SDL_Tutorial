#include "../headers/character.h"
#include "../headers/physicsEngine.h"

Character::Character() {
    size.x = 0;
    size.y = 0;
    size.w = 0;
    size.h = 0;
    stepSize = 0;
    texture = nullptr;

    prevSize = size;

    spriteInfo.rotateLeft = 0;
    spriteInfo.roteteRight = 0;
    spriteInfo.rotateUp = 0;
    spriteInfo.rotateDown = 0;
    spriteInfo.numberOfStepInSprite = 0;
    spriteInfo.useWalkClip = 0;
    spriteInfo.clipsNumber = 0;

    velocityX = 0;
    velocityY = 0;
    isMove = false;
}

Character::~Character() {
    SDL_DestroyTexture(texture);
}

/**
 * @brief Move character in a certain direction
 * @param direction Direction to move
 * @return          None
 */
void Character::move(int direction, double dt) {
    // When we step on diagonal direction, an object move at a distance of hypotenuse
    // Therefore, need to define the value of X and Y components
    float rootOf2 = 1.41421356237f;             // The root of 2 required to calculate the diagonal distance
    int diagStepSize = stepSize / rootOf2;
    // We must convert from float to int
    int oldStep = stepSize;
    double velocity = 250.0;
    switch (direction) {
        case UP:
            velocityY = -velocity;
            size.y += velocityY * dt;
            checkCollisionWithBorders();
            break;
        case DOWN:
            velocityY = velocity;
            size.y += velocityY * dt;
            checkCollisionWithBorders();
            break;
        case LEFT:
            velocityX = -velocity;
            size.x += velocityX * dt;
            checkCollisionWithBorders();
            break;
        case RIGHT:
            velocityX = velocity;
            size.x += velocityX * dt;
            checkCollisionWithBorders();
            break;
        case UP_LEFT:
            // stepSize = diagStepSize;
            move(UP, dt);
            move(LEFT, dt);
            // stepSize = oldStep;
            break;
        case UP_RIGHT:
            // stepSize = diagStepSize;
            move(UP, dt);
            move(RIGHT, dt);
            // stepSize = oldStep;
            break;
        case DOWN_LEFT:
            // stepSize = diagStepSize;
            move(DOWN, dt);
            move(LEFT, dt);
            // stepSize = oldStep;
            break;
        case DOWN_RIGHT:
            // stepSize = diagStepSize;
            move(DOWN, dt);
            move(RIGHT, dt);
            // stepSize = oldStep;
            break;
        default:
            std::cerr << "ERROR: Character movement error!" << std::endl;
            break;
    }
}

/**
 * @brief Scale person size
 * @param zoom  Scale factor
 * @return None
 */
void Character::scale(int zoom) {
    size.w *= zoom;
    size.h *= zoom;
    stepSize *= zoom;

    prevSize = size;
}

/**
 * @brief Call a physics engine to check the collision with borders of the window
 * @return None
 */
void Character::checkCollisionWithBorders() {
    if (PhysicsEngine::checkCollisionWithBorders(*this))
        size = prevSize;
    else
        prevSize = size;
}