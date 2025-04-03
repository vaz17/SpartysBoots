/**
 * @file SrFlipFlopGateTest.cpp
 * @author Silas Hodges
 */

#include <pch.h>
#include "gtest/gtest.h"
#include <SrFlipFlopGate.h>
#include <Game.h>
#include <InputPin.h>
#include <OutputPin.h>

class SrFlipFlopGateTest : public ::testing::Test
{
protected:
	void SetUp() override
	{
		game = new Game();

		flipFlop = new SrFlipFlopGate(game);

		flipFlop->SetLocation(600, 400);
	}

	void TearDown() override
	{
		delete flipFlop;
		delete game;
	}

	Game* game;
	SrFlipFlopGate* flipFlop;
};

TEST_F(SrFlipFlopGateTest, InitialState)
{
	ASSERT_EQ(flipFlop->GetOutputA()->GetState(), States::Zero);
	ASSERT_EQ(flipFlop->GetOutputB()->GetState(), States::One);
}

TEST_F(SrFlipFlopGateTest, SetSHigh_RLow)
{
	// Set S to one
	flipFlop->GetInputA()->SetState(States::One);

	// Set R to zero
	flipFlop->GetInputB()->SetState(States::Zero);

	// Update gate
	flipFlop->Update(0.1);

	// Verify outputs
	ASSERT_EQ(flipFlop->GetOutputA()->GetState(), States::One);
	ASSERT_EQ(flipFlop->GetOutputB()->GetState(), States::Zero);
}

TEST_F(SrFlipFlopGateTest, SetSLow_RHigh)
{
	// Set S to zero
	flipFlop->GetInputA()->SetState(States::Zero);

	// Set R to one
	flipFlop->GetInputB()->SetState(States::One);

	// Update gate
	flipFlop->Update(0.1);

	// Verify outputs
	ASSERT_EQ(flipFlop->GetOutputA()->GetState(), States::Zero);
	ASSERT_EQ(flipFlop->GetOutputB()->GetState(), States::One);
}

TEST_F(SrFlipFlopGateTest, SetSHigh_RHigh_InvalidState)
{
	// Set S to one
	flipFlop->GetInputA()->SetState(States::One);

	// Set R to one
	flipFlop->GetInputB()->SetState(States::One);

	// Update gate
	flipFlop->Update(0.1);

	// Verify outputs
	ASSERT_EQ(flipFlop->GetOutputA()->GetState(), States::Unknown);
	ASSERT_EQ(flipFlop->GetOutputB()->GetState(), States::Unknown);
}

TEST_F(SrFlipFlopGateTest, SetSLow_RLow_NoChange)
{
	// Set S to zero
	flipFlop->GetInputA()->SetState(States::Zero);

	// Set R to zero
	flipFlop->GetInputB()->SetState(States::Zero);

	// Update gate
	flipFlop->Update(0.1);

	// Verify outputs
	ASSERT_EQ(flipFlop->GetOutputA()->GetState(), States::Zero);
	ASSERT_EQ(flipFlop->GetOutputB()->GetState(), States::One);
}

TEST_F(SrFlipFlopGateTest, Construct)
{
	Game myGame;

	SrFlipFlopGate flipFlop(&myGame);
}

TEST_F(SrFlipFlopGateTest, HitTest)
{
	Game myGame;
	SrFlipFlopGate flipFlop(&myGame);

	flipFlop.SetLocation(100, 200);

	// Center of the gate should be a true
	ASSERT_TRUE(flipFlop.HitTest(100, 200));

	// Left of the gate
	ASSERT_FALSE(flipFlop.HitTest(10, 200));

	// Right of the gate
	ASSERT_FALSE(flipFlop.HitTest(200, 200));

	// Above the gate
	ASSERT_FALSE(flipFlop.HitTest(100, 0));

	// Below the gate
	ASSERT_FALSE(flipFlop.HitTest(100, 300));
}