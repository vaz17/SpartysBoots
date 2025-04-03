/**
 * @file LevelLoaderTest.cpp
 * @author Shawn_Porto
 */

#include <pch.h>
#include "gtest/gtest.h"
#include <LevelLoader.h>
#include <Game.h>
#include <regex>
#include <string>
#include <fstream>
#include <streambuf>
#include <wx/filename.h>

using namespace std;

class LevelLoaderTest : public ::testing::Test
{
private:

public:
    LevelLoaderTest();
};

TEST(LevelLoaderTest, Construct)
{
    Game myGame;

    LevelLoader lvlLoader;
}

TEST(LevelLoaderTest, HitTest0)
{
    Game game;
    LevelLoader lvlLoader;

    ASSERT_EQ(game.HitTest(155, 430), nullptr);

    lvlLoader.LoadLevel("levels/level0.xml", &game);

    //Looks for the sensor
    ASSERT_FALSE(game.HitTest(155, 430) == nullptr);
    //Looks for the Conveyor
    ASSERT_FALSE(game.HitTest(205, 430) == nullptr);
    //Looks in a place where there is supposed to be nothing
    ASSERT_TRUE(game.HitTest(600, 600) == nullptr);
}

TEST(LevelLoaderTest, HitTest1)
{
    Game game;
    LevelLoader lvlLoader;

    ASSERT_EQ(game.HitTest(150, 400), nullptr);

    lvlLoader.LoadLevel("levels/level1.xml", &game);

    //Looks for the Conveyor
    ASSERT_FALSE(game.HitTest(150, 400) == nullptr);
    //Looks for the Conveyor
    ASSERT_FALSE(game.HitTest(160, 130) == nullptr);
    //Looks in a place where there is supposed to be nothing
    ASSERT_TRUE(game.HitTest(600, 600) == nullptr);
}