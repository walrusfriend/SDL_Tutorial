#include "../headers/physicsEngine.h"

/**
 * @brief Check for hit to a borders of the window
 * @param pers  Reference to the Character object
 * @return      True if hit else false
 */
bool PhysicsEngine::checkCollisionWithBorders(const Character& pers) {
    // Check X direction
    if (pers.size.x < 0 or pers.size.x + pers.size.w > SCREEN_WIDTH) 
        return true;

    // Check Y direction
    if (pers.size.y < 0 or pers.size.y + pers.size.h > SCREEN_HEIGHT)
        return true;

    return false;
}