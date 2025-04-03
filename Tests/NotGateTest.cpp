/**
 * @file NotGateTest.cpp
 * @author Silas Hodges
 */

#include <pch.h>
#include "gtest/gtest.h"
#include <NotGate.h>
#include <Game.h>
#include <InputPin.h>
#include <OutputPin.h>

class NotGateTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
	    game = new Game();

    	notGate = new NotGate(game);

    	notGate->SetLocation(600, 400);
    }

	void TearDown() override
    {
	    delete notGate;
    	delete game;
    }

	Game* game;
	NotGate* notGate;
};

TEST_F(NotGateTest, InitialState)
{
	ASSERT_EQ(notGate->GetOutput()->GetState(), States::Unknown);
}

TEST_F(NotGateTest, InputHigh_OutputLow)
{
	// Set input to one
	notGate->GetInput()->SetState(States::One);

	// Update gate
	notGate->Update(0.1);

	// Verify output
	ASSERT_EQ(notGate->GetOutput()->GetState(), States::Zero);
}

TEST_F(NotGateTest, InputLow_OutputHigh)
{
	// Set input to zero
	notGate->GetInput()->SetState(States::Zero);

	// Update gate
	notGate->Update(0.1);

	// Verify output
	ASSERT_EQ(notGate->GetOutput()->GetState(), States::One);
}

TEST_F(NotGateTest, ToggleInput)
{
	// Set input to one
	notGate->GetInput()->SetState(States::One);
	notGate->Update(0.1);
	ASSERT_EQ(notGate->GetOutput()->GetState(), States::Zero);

	// Set input to zero
	notGate->GetInput()->SetState(States::Zero);
	notGate->Update(0.1);
	ASSERT_EQ(notGate->GetOutput()->GetState(), States::One);

	// Set input ot one again
	notGate->GetInput()->SetState(States::One);
	notGate->Update(0.1);
	ASSERT_EQ(notGate->GetOutput()->GetState(), States::Zero);
}

TEST_F(NotGateTest, Construct)
{
	ASSERT_NE(notGate, nullptr);
}

TEST_F(NotGateTest, HitTest)
{
	// Set location
	notGate->SetLocation(600.0, 400.0);

	// Gate dimensions
	double width = notGate->getWidth();
	double height = notGate->getHeight();

	// Center gate should hit
	ASSERT_TRUE(notGate->HitTest(600, 400));

	// Left of gate
	ASSERT_FALSE(notGate->HitTest(510, 400));

	// Right of gate
	ASSERT_FALSE(notGate->HitTest(700, 400));

	// Above gate
	ASSERT_FALSE(notGate->HitTest(600, 300));

	// Below gate
	ASSERT_FALSE(notGate->HitTest(600, 500));
}