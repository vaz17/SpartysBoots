/**
 * @file Scoreboard.cpp
 * @author Benjamin Eyke
 *
 * Implements the scoreboard for the game.
 * Is responsible for displaying game timer, game score, level score, and instructions.
 */

#include "pch.h"
#include "Scoreboard.h"
#include <sstream>
#include <iomanip>
#include <wx/dcbuffer.h>
#include "Game.h"

using namespace std;

/// Size of scoreboard in virtual pixels
const wxSize Scoreboard::ScoreboardSize(380, 175);

/// Score font size;
const int ScoreFontSize = 25;

/// Instruction font size;
const int InstructionFontSize = 15;

/// Timer threshold
const int Minute = 60;

/// Timer threshold 2
const int HalfMinute = 30;

/// X position for score
const int ScoreX = 10;

/// Y position for score
const int ScoreY = 50;

/// X position for time bonus
const int BonusX = 10;

/// Y position for time bonus
const int BonusY = 90;

/// Offset used for score of game.
const int GameScoreOffsetX = 200;

/// X coordinate offset for timer text.
const int TimerX = 10;

/// Y coordinate offset for timer text.
const int TimerY = 10;

/// X position of timer.
const int TimerOffsetX = 225;

/// Spacing between instructions
const int InstructionSpacing = 15;

/**
 * @return Green color
 */
const wxColour Default(24, 69, 59);

/**
 * @return Yellow color
 */
const wxColour MinuteWarning(201, 128, 4);

/**
 * @return Red color
 */
const wxColour HalfMinuteWarning(171, 25, 27);

/**
 * Item representing game scoreboard
 * @param game Game the scoreboard is connected to.
 */
Scoreboard::Scoreboard(Game* game) : Item(game)
{
    int good;
    int bad;
    int hours;
    int minutes;
    int seconds;
}

/**
 * Load attributes for item node.
 * Loads attributes for the scoreboard to use for drawing.
 * @param node Xml node we are loading item from.
 */
void Scoreboard::XmlLoad(wxXmlNode* node)
{

 node->GetAttribute(L"x", L"700").ToInt(&mX);
 node->GetAttribute(L"y", L"40").ToInt(&mY);

 //Load in the good attribute
 int good = 10; //Default 10
 node->GetAttribute(L"good", L"10").ToInt(&good);
 GetGame()->GetScore()->SetGoodScore(good);

 //Load in the bad attribute
 int bad = 0; //Default 0
 node->GetAttribute(L"bad", L"0").ToInt(&bad);
 GetGame()->GetScore()->SetBadScore(bad);

 //Load in the instruction text into a single string.
 auto child = node->GetChildren();
 for (; child; child = child->GetNext())
 {
     if (child->GetType() == wxXML_TEXT_NODE)
     {
         std::wstring instructionLine = child->GetContent().ToStdWstring();
         mGoalText += instructionLine;
     }
     else if (child->GetName() == L"br") //Line break
     {
         mGoalText += L"\n";
     }
 }

}

/**
 * Draws the scoreboard for the game.
 * @param graphics context to draw scoreboard on
 */
void Scoreboard::Draw(shared_ptr<wxGraphicsContext> graphics)
{
    //Draw Scoreboard box
    graphics->SetBrush(*wxWHITE_BRUSH); //Sets color of scoreboard
    graphics->SetPen(*wxBLACK_PEN); //Outlines scoreboard in black
    graphics->DrawRectangle(mX, mY, ScoreboardSize.GetWidth(), ScoreboardSize.GetHeight());

    //Get Score
    auto score = GetGame()->GetScore();
    int levelScore = score->GetLevelScore();
    int gameScore = score->GetGameScore();

    //Draw level score and game score
    auto scoreFont = graphics->CreateFont(ScoreFontSize, "Arial", wxFONTFLAG_BOLD, wxColour(24, 69, 59));
    graphics->SetFont(scoreFont);
    graphics->DrawText(L"Level: " + to_wstring(levelScore), mX + ScoreX, mY + ScoreY);
    graphics->DrawText(L"Game: " + to_wstring(gameScore), mX + GameScoreOffsetX, mY + ScoreY);

    //Draw Timer
    graphics->DrawText(L"Time Remaining:", mX + TimerX, mY + TimerY);
    double remainingTime = GetGame()->GetTimer().GetRemainingTime();
    wxColour timerColor = Default;
    if (remainingTime < Minute)
    {
        timerColor = MinuteWarning;
    }
    if (remainingTime < HalfMinute)
    {
        timerColor = HalfMinuteWarning;
    }
    auto timerFont = graphics->CreateFont(ScoreFontSize, "Arial", wxFONTFLAG_BOLD, timerColor);
    graphics->SetFont(timerFont);
    wstringstream timeStream;

    timeStream << setw(2) << setfill(L'0') << mHours << L':';
    timeStream << setw(2) << setfill(L'0') << mMinutes << L':';
    timeStream << setw(2) << setfill(L'0') << mSeconds;

    graphics->DrawText(timeStream.str(), mX + TimerOffsetX, mY + TimerY);

    // Draw time bonus
    auto bonusFont = graphics->CreateFont(ScoreFontSize, "Arial", wxFONTFLAG_BOLD, Default);
    graphics->DrawText(L"Time Bonus: " + to_string(GetGame()->GetTimeBonus()), mX + BonusX, mY + BonusY);


    //Initializes the spacing of instruction below the scores.
    int instruction_y = (mY + BonusY) + SpacingScoresToInstructions;

    //Draw the instruction text.
    auto instructionsFont = graphics->CreateFont(InstructionFontSize, "Arial", wxFONTFLAG_BOLD, *wxBLACK);
    graphics->SetFont(instructionsFont);

    wistringstream stream(mGoalText);
    wstring instructionLine;
    while (getline(stream, instructionLine))
    {
        graphics->DrawText(instructionLine, mX + ScoreX, instruction_y);
        instruction_y += SpacingInstructionLines;
    }
}

/**
 * Updates the timer on the scoreboard.
 * @param elapsed time elapsed by the game.
 */
void Scoreboard::Update(double elapsed)
{
    int timeToPrint = GetGame()->GetTimer().GetRemainingTime();

    //Format time to hour, minute, second
    mHours = timeToPrint / 3600;
    mMinutes = (timeToPrint % 3600) / 60;
    mSeconds = timeToPrint % 60;

}

/**
 * Included for virtual override
 * No current use.
 * @param x position clicked.
 * @param y position clicked.
 */
void Scoreboard::OnClick(double x, double y)
{
}
