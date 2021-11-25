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

    velocity = 250;
    velocityX = 0;
    velocityY = 0;
    isMove = false;

    keyboardState = SDL_GetKeyboardState(NULL); 
}

Character::~Character() {
    SDL_DestroyTexture(texture);
}

/**
 * @brief Move character in a certain direction
 * @param direction Direction to move
 * @return          None
 */
void Character::move(double dt) {
    // When we step on diagonal direction, an object move at a distance of hypotenuse
    // Therefore, need to define the value of X and Y components
    float rootOf2 = 1.41421356237f;             // The root of 2 required to calculate the diagonal distance
    int diagStepSize = stepSize / rootOf2;
    int oldStep = stepSize;

    // TODO Fix priority of the W and A
    if (isMove) {
        if (keyboardState[SDL_SCANCODE_W] and keyboardState[SDL_SCANCODE_A]) {
            size.y += -velocity * dt / rootOf2;
            size.x += -velocity * dt / rootOf2;
            checkCollisionWithBorders();
        }
        else if (keyboardState[SDL_SCANCODE_W] and keyboardState[SDL_SCANCODE_D]) {
            size.y += -velocity * dt / rootOf2;
            size.x += velocity * dt / rootOf2;
            checkCollisionWithBorders();
        }
        else if (keyboardState[SDL_SCANCODE_S] and keyboardState[SDL_SCANCODE_A]) {
            size.y += velocity * dt / rootOf2;
            size.x += -velocity * dt / rootOf2;
            checkCollisionWithBorders();
        }
        else if (keyboardState[SDL_SCANCODE_S] and keyboardState[SDL_SCANCODE_D]) {
            size.y += velocity * dt / rootOf2;
            size.x += velocity * dt / rootOf2;
            checkCollisionWithBorders();
        }
        else if (keyboardState[SDL_SCANCODE_W]) {
            size.y += -velocity * dt;
            checkCollisionWithBorders();
        }
        else if (keyboardState[SDL_SCANCODE_S]) {
            size.y += velocity * dt;
            checkCollisionWithBorders();
        }
        else if (keyboardState[SDL_SCANCODE_A]) {
            size.x += -velocity * dt;
            checkCollisionWithBorders();
        }
        else if (keyboardState[SDL_SCANCODE_D]) {
            size.x += velocity * dt;
            checkCollisionWithBorders();
        }
        else
            isMove = false;
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