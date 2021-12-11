#include "../headers/character.h"
#include "../headers/physicsEngine.h"
#include "../headers/graphicsEngine.h"

Character::Character() {
    // TODO Store a character setting to json file
    size = {0, 0, 0, 0};
    texture.reset(new WTexture());

    prevSize = size;

    moveDirection = Directions::NONE;
    spriteInfo.rotateLeft = 0;
    spriteInfo.rotateRight = 0;
    spriteInfo.rotateUp = 0;
    spriteInfo.rotateDown = 0;
    spriteInfo.currentRotation = 0;
    spriteInfo.numberOfStepInSprite = 0;
    spriteInfo.useWalkClip = 0;
    spriteInfo.clipsNumber = 0;
    spriteInfo.moveFramerate = 0;          // Number of ms between frames
    spriteInfo.flip = SDL_FLIP_NONE;

    velocity = 0;
    isMove = false;
    isAttack = false;

    keyboardState = SDL_GetKeyboardState(NULL); 
}

Character::~Character() {
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

    // TODO Fix priority of the W and A
    if (isMove and !isAttack) {
        if (keyboardState[SDL_SCANCODE_LSHIFT]) {
            velocity = 500;
        }
        else {
            velocity = 250;
        }

        if (moveDirection == Directions::UP_LEFT) {
            size.y += static_cast<int>(-velocity * dt / rootOf2);
            size.x += static_cast<int>(-velocity * dt / rootOf2);
            checkCollisionWithBorders();
            spriteInfo.currentRotation = spriteInfo.rotateLeft;
            spriteInfo.flip = SDL_FLIP_HORIZONTAL;
        }
        else if (moveDirection == Directions::UP_RIGHT) {
            size.y += static_cast<int>(-velocity * dt / rootOf2);
            size.x += static_cast<int>(velocity * dt / rootOf2);
            checkCollisionWithBorders();
            spriteInfo.currentRotation = spriteInfo.rotateRight;
            spriteInfo.flip = SDL_FLIP_NONE;
        }
        else if (moveDirection == Directions::DOWN_LEFT) {
            size.y += static_cast<int>(velocity * dt / rootOf2);
            size.x += static_cast<int>(-velocity * dt / rootOf2);
            checkCollisionWithBorders();
            spriteInfo.currentRotation = spriteInfo.rotateLeft;
            spriteInfo.flip = SDL_FLIP_HORIZONTAL;
        }
        else if (moveDirection == Directions::DOWN_RIGHT) {
            size.y += static_cast<int>(velocity * dt / rootOf2);
            size.x += static_cast<int>(velocity * dt / rootOf2);
            checkCollisionWithBorders();
            spriteInfo.currentRotation = spriteInfo.rotateRight;
            spriteInfo.flip = SDL_FLIP_NONE;
        }
        else if (moveDirection == Directions::UP) {
            size.y += static_cast<int>(-velocity * dt);
            checkCollisionWithBorders();
        }
        else if (moveDirection == Directions::DOWN) {
            size.y += static_cast<int>(velocity * dt);
            checkCollisionWithBorders();
        }
        else if (moveDirection == Directions::LEFT) {
            size.x += static_cast<int>(-velocity * dt);
            checkCollisionWithBorders();
            spriteInfo.currentRotation = spriteInfo.rotateLeft;
            spriteInfo.flip = SDL_FLIP_HORIZONTAL;
        }
        else if (moveDirection == Directions::RIGHT) {
            size.x += static_cast<int>(velocity * dt);
            checkCollisionWithBorders();
            spriteInfo.currentRotation = spriteInfo.rotateRight;
            spriteInfo.flip = SDL_FLIP_NONE;
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

    prevSize = size;

    scaleFactor = zoom;
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

/**
 * @brief Update character sprite on the screen
 * 
 */
void Character::update() {
    int currentFrame = 0;
    
    if (isMove) {
        currentFrame = SDL_GetTicks() / spriteInfo.moveFramerate % spriteInfo.numberOfStepInSprite;
    }
    
    // Take into account character movement direction
    currentFrame += spriteInfo.currentRotation;

    GraphicsEngine::renderTexture(*texture, size, &spriteInfo.walkSprite[currentFrame]);
    // GraphicsEngine::renderTexture(*texture, size, &spriteInfo.walkSprite[currentFrame], 
    //                               spriteInfo.flip, texture->getAngle());
}

/**
 * @brief Make an attack
 * 
 */
void Character::attack() {
    if (isAttack) {
        // Change movement texture size to attack texture size
        
        // Check collition with other objects

        // Draw attack animation

        // Cancel attack
        // int currentFrame = 0;
    
        // if (isMove) {
        //     currentFrame = SDL_GetTicks() / spriteInfo.moveFramerate % spriteInfo.numberOfStepInSprite;
        // }
        
        // // GraphicsEngine::renderTexture(*texture, size, &spriteInfo.walkSprite[currentFrame]);
        // GraphicsEngine::renderTexture(*texture, size, &spriteInfo.walkSprite[currentFrame], 
        //                           spriteInfo.flip, texture->getAngle());
    }
    else {
        // Return size
    }
}