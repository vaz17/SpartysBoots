/**
 * @file Game.cpp
 * @author Benjamin Eyke
 */

#include "pch.h"
#include "Game.h"
#include "Conveyor.h"
#include "Item.h"
#include "OrGate.h"
#include "Beam.h"
#include "Product.h"
#include "Sensor.h"
#include "Scoreboard.h"
#include "Sparty.h"
#include "LevelLoader.h"

using namespace std;

/// Frame duration in milliseconds
const int FrameDuration = 30;

/// Level notices duration in seconds
const double LevelNoticeDuration = 2.0;

/// Size of notices displayed on screen in virtual pixels
const int NoticeSize = 100;

/// Color to draw the level notices
const auto LevelNoticeColor = wxColour(24, 69, 59);

/// Color to draw a rectangle enclosing the level notice text
/// @return LevelNoticeBackground colour.
wxColour LevelNoticeBackground(255, 255, 255, 200);

/// Padding to add to the left and right side of the level
/// notice background rectangle in virtual pixels
const double LevelNoticePadding = 20;

/// Int to move objects that overlap
const int Overlap = 75;

/**
 * Game Constructor
 */
Game::Game()
{
}

/**
 * Draw the game.
 * @param graphics device context to draw on
 * @param width width of the drawing area
 * @param height height of the drawing area
 */
void Game::OnDraw(std::shared_ptr<wxGraphicsContext> graphics, int width, int height)
{
    // Determine the size of the playing area in pixels
    int pixelWidth = mPlayfieldWidth; // Still need to load this
    int pixelHeight = mPlayfieldHeight; // Still need to load this

    // Automatic Scaling
    auto scaleX = double(width) / double(pixelWidth);
    auto scaleY = double(height) / double(pixelHeight);
    mScale = std::min(scaleX, scaleY);

    mXOffset = (width - pixelWidth * mScale) / 2.0;
    mYOffset = 0;
    if (height > pixelHeight * mScale)
    {
        mYOffset = (double)((height - pixelHeight * mScale) / 2.0);
    }

    graphics->PushState();

    graphics->Translate(mXOffset, mYOffset);
    graphics->Scale(mScale, mScale);

    wxColour color(235,254,232);
    wxBrush boundaryBrush(color);
    graphics->SetBrush(boundaryBrush);
    graphics->DrawRectangle(0, 0, pixelWidth, pixelHeight);

    wxGraphicsPath clipPath = graphics->CreatePath();
    clipPath.AddRectangle(0,0,mPlayfieldWidth, mPlayfieldHeight);
    graphics->Clip(0,0, mPlayfieldWidth, mPlayfieldHeight);

    // Draw all game items
    for (const auto& item : mItems)
    {
        item->Draw(graphics);
    }

    graphics->PopState();

    if (mCurrentState == State::Ended)
    {
        wxRect rect(0, 0, width, height);
        wxString message = L"Level Complete!";
        DisplayMessage(graphics, mScale, rect, message);
    }
    else if (mCurrentState == State::Loading)
    {
        wxRect rect(0, 0, width, height);
        wxString message = L"Level " + to_string(mCurrentLevel) + " Begin";
        DisplayMessage(graphics, mScale, rect, message);
    }
}

/**
 * Resets the game Timer
 */
void Game::ResetTimer()
{
    mTimer.Reset();
}

/**
 * process level
 * @param node xml node
 */
void Game::XmlGame(wxXmlNode* node)
{
    wxString levelSize = node->GetAttribute("size", "0,0");
    int delimiterPos = levelSize.find(',');


    levelSize.substr(0, delimiterPos).ToInt(&mPlayfieldWidth);
    levelSize.substr(delimiterPos + 1, levelSize.length()).ToInt(&mPlayfieldHeight);
}

/**
 * Process item node
 * @param node XML node
 */
void Game::XmlItem(wxXmlNode *node)
{
    auto name = node->GetName().ToStdWstring();
    shared_ptr<Item> item;

    if (name == L"conveyor")
    {
        auto conveyor = make_shared<Conveyor>(this);
        mItems.push_back(conveyor);
        conveyor->XmlLoad(node);

    }
    else if (name == L"orgate")
    {
        item = make_shared<OrGate>(this);
        if (item != nullptr)
        {
            item->XmlLoad(node);
            Add(item);
        }
    }
    else if (name == L"beam")
    {
        item = make_shared<Beam>(this);
        if (item != nullptr)
        {
            item->XmlLoad(node);

            double x = item->GetX();
            double y = item->GetY();

            Add(item, x, y);
        }
    }
    else if (name == L"sensor")
    {
        item = make_shared<Sensor>(this);
        if (item != nullptr)
        {
            item->XmlLoad(node);

            double x = item->GetX();
            double y = item->GetY();

            Add(item, x, y);
        }
    }
    else if (name == L"scoreboard")
    {
        item = make_shared<Scoreboard>(this);
        if (item != nullptr)
        {
            item->XmlLoad(node);
            Add(item);
        }
    }
    else if (name == L"sparty")
    {
        item = make_shared<Sparty>(this);
        if (item != nullptr)
        {
            item->XmlLoad(node);
            double x = item->GetX();
            double y = item->GetY();
            Add(item, x, y);
        }
    }
}

/**
 * Add item to game.
 * @param item New item to add
 */
void Game::Add(std::shared_ptr<Item> item)
{
    auto InitialX = mPlayfieldWidth / 2;
    auto InitialY = mPlayfieldHeight / 2;

    // Adjust the position if there are existing items
    AdjustPosition(item, InitialX, InitialY);

    mItems.push_back(item);
}

/**
 * Adjust the position of the item to avoid overlapping with existing items.
 * @param item Item to adjust
 * @param x The target x position
 * @param y The target y position
 */
void Game::AdjustPosition(std::shared_ptr<Item> item, int &x, int &y)
{
    // Check if the item overlaps with existing items
    for (const auto& existingItem : mItems)
    {
        if (existingItem->HitTest(x, y)) // Assuming HitTest checks for overlap
        {
            // Move the item down and to the right
            x += Overlap;
            y += Overlap;

            // Recur to check the new position
            AdjustPosition(item, x, y);
            break; // Break out to check the updated position with other items
        }
    }

    // Set the final location
    item->SetLocation(x, y);
}

/**
 * Add item to game.
 * @param item New item to add
 * @param customX Custom location to set x
 * @param customY Custom location to set y
 */
void Game::Add(std::shared_ptr<Item> item, int customX, int customY)
{
    item->SetLocation(customX, customY);
    mItems.push_back(item);
}

/**
 * Clear data in game.
 */
void Game::Clear()
{
    mItems.clear();
}

/**
 * Handles updates for animation
 * @param elapsed time since last update
 */
void Game::Update(double elapsed)
{


    for (auto item : mItems)
    {
        item->Update(elapsed);
    }

    mTimer.Update(elapsed);
    UpdateTimeBonus();

    // Game is ending, wait to display message after 3 seconds
    switch (mCurrentState)
    {
    case State::Ending:
        mEndDelay += elapsed;
        mEndDelay += elapsed;
        if (mEndDelay >= 3.0)
        {
            UpdateScore();
            mCurrentState = State::Ended;
            mEndDelay = 0;
        }
        break;

    case State::Ended:
        mEndDelay += elapsed;
        if (mEndDelay >= 2.0)
        {
            mCurrentState = State::LoadingNextlLevel;
            mEndDelay = 0;
            if (mCurrentLevel < 8)
            {
                mCurrentLevel++;
            }
            LoadNextLevel(mCurrentLevel);
        }
        break;
    // Game has ended, load next level after 2 seconds
    case State::Loading:
        mStartDelay += elapsed;
        if (mStartDelay >= 2.0)
        {
            mCurrentState = State::Loaded;
        }
        break;

    default:
        if (mTimer.GetRemainingTime() <= 0 && !mGameEnded)
        {
            mCurrentState = State::Ending;
            mEndDelay = 2.0;
        }
        break;
    }
}

/**
 * Test x and y location to see if an item was clicked on in the game.
 * @param x location in pixels
 * @param y location in pixels
 * @returns Pointer to item clicked on, or nullptr if none.
 */
std::shared_ptr<IDraggable> Game::HitTest(int x, int y)
{
    for (auto i = mItems.rbegin(); i != mItems.rend();  i++)
    {
        // Did we click on something contained in the drawable?
        auto draggable = (*i)->HitDraggable(x, y);
        if(draggable != nullptr)
        {
            return draggable;
        }

        if ((*i)->HitTest(x, y))
        {
            return *i;
        }
    }

    return  nullptr;
}

/**
 * Adds product onto conveyor
 * @param node The node that is to be added
 * @param conveyor The conveyor that will be added on
 */
void Game::AddProduct(wxXmlNode *node, std::shared_ptr<Conveyor> conveyor)
{
    auto product = make_shared<Product>(this);
    product->XmlLoad(node);
    double placement = 0;
    node->GetAttribute("placement", "0").ToDouble(&placement);
    double conveyorX = conveyor->GetX();
    double conveyorY = conveyor->GetY();
    product->SetLocation(conveyorX, conveyorY - placement);
    mItems.push_back(product);
}

/**
 * Handle a mouse click
 * @param x X location clicked on
 * @param y Y location clicked on
 */
void Game::OnMouseDown(int x, int y)
{
    double oX = (x - mXOffset) / mScale;
    double oY = (y - mYOffset) / mScale;
    auto item = HitTest(oX, oY);
    if (item != nullptr)
    {
        item->OnClick(oX, oY);
    }
}

/**
 * Handle the left mouse button down event
 * @param x X coordinate of mouse
 * @param y Y coordinate of mouse
 */
void Game::OnLeftDown(int x, int y)
{
    double oX = (x - mXOffset) / mScale;
    double oY = (y - mYOffset) / mScale;
    mX = oX;
    mY = oY;
    OnMouseDown(x, y);
}

/**
 * Function that accepts a visitor
 * @param visitor A visitor item
 */
void Game::Accept(ItemVisitor* visitor)
{
    for (const auto& item : mItems)
    {
        item->Accept(visitor);
    }
}

void Game::TryToConnect(OutputPin* pin, wxPoint lineEnd)
{
    for (auto i = mItems.rbegin(); i != mItems.rend();  i++)
    {
        if((*i)->Connect(pin, lineEnd))
        {
            return;
        }
    }
}

/**
 * Sets state of game to end, used by beam.
 */
void Game::EndGame()
{
    if (!mGameEnded)
    {
        mEndDelay = 0;
        mCurrentState = State::Ending;
    }
}

void Game::SetState(State newState)
{
    mCurrentState = newState;
}

/**
 * Used by the gameview to set the state to loading when a level is selected from the menu.
 */
void Game::SetStateLoading()
{
    mCurrentState = State::Loading;
    mStartDelay = 0;
}

/**
 * Draws a formatted UI message for level loading and ending
 * @param gc Graphics context to draw on
 * @param scale Scale of the current window
 * @param rect Window width and height
 * @param message Message to display on screen
 */
void Game::DisplayMessage(std::shared_ptr<wxGraphicsContext> gc, double scale, wxRect rect, wxString message)
{
    auto levelMessageFont = gc->CreateFont(NoticeSize * scale, "Arial", wxFONTFLAG_BOLD, LevelNoticeColor);
    gc->SetFont(levelMessageFont);

    double messageWidth;
    double messageHeight;
    // Measure dimensions of text to center text.
    gc->GetTextExtent(message, &messageWidth, &messageHeight);

    // Calculate position of level start box.
    double rectangleX = (rect.GetWidth() - messageWidth) / 2 - (LevelNoticePadding * scale); //Centers horizontally
    double rectangleY = (rect.GetHeight() - messageHeight) / 2; //Centers vertically.
    double rectangleWidth = messageWidth + (2 * (LevelNoticePadding * scale)); //Adds padding to both sides.
    double rectangleHeight = messageHeight + (LevelNoticePadding * scale); //Add padding to top and bottom.

    // Draw level start box, centered on screen.
    wxBrush brush(LevelNoticeBackground);
    gc->SetBrush(brush);
    gc->SetPen(*wxTRANSPARENT_PEN);
    gc->DrawRectangle(rectangleX, rectangleY, rectangleWidth, rectangleHeight);

    // Draw level start text, centered on screen.
    gc->DrawText(message, (rect.GetWidth() - messageWidth) /2, rectangleY + (rectangleHeight - messageHeight) / 2);
}

void Game::LoadNextLevel(int level)
{
    mCurrentState = State::Loading;
    mStartDelay = 0;
    wxString levelName = "levels/level" + wxString::Format("%d",level) + ".xml";
    ResetTimer();
    mLevelLoader.LoadLevel(levelName, this);
}

void Game::UpdateScore()
{
    int levelScore = GetScore()->GetLevelScore();
    levelScore += mTimeBonus;
    GetScore()->SetGameScore(GetScore()->GetGameScore() + levelScore);
    GetScore()->ResetLevelScore();
}

/**
 * Updates the current time bonus for the game based on remaining time.
 */
void Game::UpdateTimeBonus()
{
    double timeLeft = mTimer.GetRemainingTime();

    int largeBonus = 30;
    int mediumBonus = 20;
    int smallBonus = 10;
    int smallDeduction = -5;
    int LargeDeducation = -10;

    if (timeLeft >= 90)
    {
        mTimeBonus = largeBonus;
    }
    else if (timeLeft < 90 && timeLeft >= 60)
    {
        mTimeBonus = mediumBonus;
    }
    else if (timeLeft < 60 && timeLeft >= 30)
    {
        mTimeBonus = smallBonus;
    }
    else if (timeLeft < 30 && timeLeft >= 1)
    {
        mTimeBonus = smallDeduction;
    }
    else if (timeLeft == 0)
    {
        mTimeBonus = LargeDeducation;
    }
}