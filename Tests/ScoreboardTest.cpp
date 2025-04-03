/**
 * @file ScoreboardTest.cpp
 * @author Benjamin Eyke
 */

#include <pch.h>
#include <gtest/gtest.h>
#include <Scoreboard.h>
#include <Game.h>
#include <regex>

using namespace std;

TEST(ScoreboardTest, Construct)
{
 Game myGame;

 Scoreboard scoreboard(&myGame);
}

TEST(ScoreboardTest, xml)
{
 Game game;
 Scoreboard scoreboard(&game);
 wxXmlNode node = wxXmlNode(wxXML_ELEMENT_NODE, "scoreboard");
 node.AddAttribute("x", "242");
 node.AddAttribute("y", "437");
 scoreboard.XmlLoad(&node);
 ASSERT_TRUE(scoreboard.GetmX() == 242);
 ASSERT_TRUE(scoreboard.GetmY() == 437);
}