/**
 * @file DFlipFlopGateTest.cpp
 * @author Silas Hodges
 */


#include <pch.h>
#include "gtest/gtest.h"
#include <DFlipFlopGate.h>
#include <Game.h>
#include <InputPin.h>
#include <OutputPin.h>

class DFlipFlopGateTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        game = new Game();

        flipFlop = new DFlipFlopGate(game);

        flipFlop->SetLocation(600.0, 400.0);
    }

    void TearDown() override
    {
        delete flipFlop;
        delete game;
    }

    Game* game;
    DFlipFlopGate* flipFlop;
};

TEST_F(DFlipFlopGateTest, InitialState)
{
    // Verify initial states
    ASSERT_EQ(flipFlop->GetOutputA()->GetState(), States::Zero);
    ASSERT_EQ(flipFlop->GetOutputB()->GetState(), States::One);
}

TEST_F(DFlipFlopGateTest, ClockRisingEdge_InputAHigh)
{
    // Set InputA active
    flipFlop->GetInputA()->SetState(States::One);

    // Set InputB to zero
    flipFlop->GetInputB()->SetState(States::Zero);

    // Simulate a clock rising edge
    flipFlop->GetInputB()->SetState(States::One);
    flipFlop->Update(0.1);

    // Verify outputs
    ASSERT_EQ(flipFlop->GetOutputA()->GetState(), States::One);
    ASSERT_EQ(flipFlop->GetOutputB()->GetState(), States::Zero);
}

TEST_F(DFlipFlopGateTest, ClockRisingEdge_InputALow)
{
    // Set InputA zero
    flipFlop->GetInputA()->SetState(States::Zero);

    // Set InputB to zero
    flipFlop->GetInputB()->SetState(States::Zero);

    // Simulate a clock rising edge
    flipFlop->GetInputB()->SetState(States::One);
    flipFlop->Update(0.1);

    // Verify outputs
    ASSERT_EQ(flipFlop->GetOutputA()->GetState(), States::Zero);
    ASSERT_EQ(flipFlop->GetOutputB()->GetState(), States::One);
}

TEST_F(DFlipFlopGateTest, ClockFallingEdge_NoChange)
{
    // Set InputA to one
    flipFlop->GetInputA()->SetState(States::One);

    // Set InputB to one
    flipFlop->GetInputB()->SetState(States::One);

    // Simulate a clock falling edge
    flipFlop->GetInputB()->SetState(States::Zero);
    flipFlop->Update(0.1);

    // Verify outputs
    ASSERT_EQ(flipFlop->GetOutputA()->GetState(), States::Zero);
    ASSERT_EQ(flipFlop->GetOutputB()->GetState(), States::One);
}

TEST_F(DFlipFlopGateTest, Construct)
{
    Game myGame;

    DFlipFlopGate flipFlop(&myGame);
}

TEST_F(DFlipFlopGateTest, HitTest)
{
    Game myGame;
    DFlipFlopGate flipFlop(&myGame);

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