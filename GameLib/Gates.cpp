/**
 * @file Gates.cpp
 * @author matthew vazquez
 *
 * Declares implementation for Gate Object used to play the game.
 */

#include "pch.h"
#include "Gates.h"

/**
 * Constructor
 * @param game The game we are in
 */
Gates::Gates(Game *game) :
    Item(game)
{
}

/**
 * Test if we hit this object with a mouse.
 * @param x X position to test
 * @param y Y position to test
 * @return true if hit.
 */
bool Gates::HitTest(double x, double y) {
    // Check if the point is in the path
    auto wid = this->getWidth();
    auto hit = this->getHeight();
    double testX = x - GetX() + wid / 2;
    double testY = y - GetY() + hit / 2;

    // Test to see if x, y are in the image
    if (testX < 0 || testY < 0 || testX >= wid || testY >= hit)
    {
        // We are outside the image
        return false;
    }
    return true;
}
