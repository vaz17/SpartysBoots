/**
 * @file OrGateTest.cpp
 * @author matthew vazquez
 */

#include <pch.h>
#include "gtest/gtest.h"
#include <OrGate.h>
#include <Game.h>


TEST(OrGateTest, Construct) {
    Game myGame;

    OrGate orGate(&myGame);
}

TEST(OrGateTest, HitTest) {
    // Create a OrGate to test
    Game game;
    OrGate gate(&game);

    // Give it a location
    // Always make the numbers different, in case they are mixed up
    gate.SetLocation(100, 200);

    // Center of the gate should be a true
    ASSERT_TRUE(gate.HitTest(100, 200));

    // Left of the gate
    ASSERT_FALSE(gate.HitTest(10, 200));

    // Right of the gate
    ASSERT_FALSE(gate.HitTest(200, 200));

    // Above the gate
    ASSERT_FALSE(gate.HitTest(100, 0));

    // Below the gate
    ASSERT_FALSE(gate.HitTest(100, 300));

}