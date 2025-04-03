/**
 * @file AndGateTest.cpp
 * @author Silas Hodges
 *
 * Unit tests for the AndGate class
 */

#include <pch.h>
#include "gtest/gtest.h"
#include <AndGate.h>
#include <Game.h>

/// Test that AndGate class can be constructed
TEST(AndGateTest, Construct)
{
    Game game;
	AndGate andGate(&game);
}

/// Test HitTest function of AndGate class
TEST(AndGateTest, HitTest)
{
	Game game;
	AndGate gate(&game);

	// Set location
	gate.SetLocation(100, 200);

	// Gate dimensions
	double width = gate.getWidth();
	double height = gate.getHeight();

	// Center of gate should be hit
	ASSERT_TRUE(gate.HitTest(100, 200));

	// Point inside gate boundary
	ASSERT_TRUE(gate.HitTest(100 + width / 4, 200)); //< Right inside
	ASSERT_TRUE(gate.HitTest(100 - width / 4, 200)); //< Left inside
	ASSERT_TRUE(gate.HitTest(100, 200 + height / 4)); //< Below inside
	ASSERT_TRUE(gate.HitTest(100, 200 - height / 4)); //< Above inside

	// Points outside the gate boundary
	ASSERT_FALSE(gate.HitTest(100 + width / 2 + 1, 200)); //< Right outside
	ASSERT_FALSE(gate.HitTest(100 - width / 2 - 1, 200)); //< Left outside
	ASSERT_FALSE(gate.HitTest(100, 200 + height / 2 + 1)); //< Below outside
	ASSERT_FALSE(gate.HitTest(100, 200 - height / 2 - 1)); //< Above outside
}
