#include "../headers/physicsEngine.h"

/**
 * Check for hit to a borders of the window
 * @param pers  Reference to the Character object
 * @return      True if hit else false
 */
bool PhysicsEngine::checkCollisionWithBorders(const Character& pers) {
    // Check X direction
    if (pers.x < 0 or pers.x + pers.width > SCREEN_WIDTH)
        return true;

    // Check Y direction
    if (pers.y < 0 or pers.y + pers.height > SCREEN_HEIGHT)
        return true;

    return false;
}