/**
 * @file ConveyorTest.cpp
 * @author Karan Choudhary
 */

#include <pch.h>
#include "gtest/gtest.h"
#include <Conveyor.h>
#include <Game.h>


TEST(ConveyorTest, Construct) {
    Game myGame;
    Conveyor conveyor(&myGame);
}


TEST(ConveyorTest, StartAndStop) {
    Game game;
    Conveyor conveyor(&game);

    conveyor.Start();
    ASSERT_TRUE(conveyor.IsRunning());

    conveyor.Stop();
    ASSERT_FALSE(conveyor.IsRunning());
}

TEST(ConveyorTest, HitTest) {
    Game game;
    Conveyor conveyor(&game);
    conveyor.SetLocation(150, 300);
    ASSERT_TRUE(conveyor.HitTest(150, 300));
    ASSERT_FALSE(conveyor.HitTest(50, 80));
    ASSERT_FALSE(conveyor.HitTest(200, 200));
    ASSERT_FALSE(conveyor.HitTest(150, 0));
    ASSERT_TRUE(conveyor.HitTest(250, 400));
}

TEST(ConveyorTest, OnClick) {
    Game game;
    Conveyor conveyor(&game);
    conveyor.SetLocation(200, 400);
    conveyor.OnClick(250, 450);
    ASSERT_TRUE(conveyor.IsRunning());
    conveyor.OnClick(250, 500);
    ASSERT_FALSE(conveyor.IsRunning());
}