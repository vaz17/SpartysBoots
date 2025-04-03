/**
 * @file GameView.cpp
 * @author matthew vazquez
 *
 * The GameView class creates window for the game.
 */

#include "pch.h"
#include "GameView.h"
#include <wx/dcbuffer.h>
#include <wx/graphics.h>
#include "Game.h"
#include "ids.h"
#include "Item.h"
#include "OrGate.h"
#include "AndGate.h"
#include "NotGate.h"
#include "SrFlipFlopGate.h"
#include "DFlipFlopGate.h"

/// Frame duration in milliseconds
const int FrameDuration = 30;

/// one OneThousanth
const double OneThousanth = 0.001;

using namespace std;

/**
 * Initialize the GameView class.
 * @param parent The parent window
 */
void GameView::Initialize(wxFrame* parent)
{
    Create(parent, wxID_ANY,
       wxDefaultPosition, wxDefaultSize,
       wxFULL_REPAINT_ON_RESIZE);
    SetBackgroundStyle(wxBG_STYLE_PAINT);

    Bind(wxEVT_PAINT, &GameView::OnPaint, this);
    Bind(wxEVT_LEFT_DOWN, &GameView::OnLeftDown, this);
    Bind(wxEVT_LEFT_UP, &GameView::OnLeftUp, this);
    Bind(wxEVT_MOTION, &GameView::OnMouseMove, this);

    // Bind level menu events
    parent->Bind(wxEVT_COMMAND_MENU_SELECTED, &GameView::OnLevelSelect, this, IDM_LEVEL0);
    parent->Bind(wxEVT_COMMAND_MENU_SELECTED, &GameView::OnLevelSelect, this, IDM_LEVEL1);
    parent->Bind(wxEVT_COMMAND_MENU_SELECTED, &GameView::OnLevelSelect, this, IDM_LEVEL2);
    parent->Bind(wxEVT_COMMAND_MENU_SELECTED, &GameView::OnLevelSelect, this, IDM_LEVEL3);
    parent->Bind(wxEVT_COMMAND_MENU_SELECTED, &GameView::OnLevelSelect, this, IDM_LEVEL4);
    parent->Bind(wxEVT_COMMAND_MENU_SELECTED, &GameView::OnLevelSelect, this, IDM_LEVEL5);
    parent->Bind(wxEVT_COMMAND_MENU_SELECTED, &GameView::OnLevelSelect, this, IDM_LEVEL6);
    parent->Bind(wxEVT_COMMAND_MENU_SELECTED, &GameView::OnLevelSelect, this, IDM_LEVEL7);
    parent->Bind(wxEVT_COMMAND_MENU_SELECTED, &GameView::OnLevelSelect, this, IDM_LEVEL8);

    parent->Bind(wxEVT_COMMAND_MENU_SELECTED, &GameView::OnAddOrGate, this, IDM_ORGATE);
    parent->Bind(wxEVT_COMMAND_MENU_SELECTED, &GameView::OnAddAndGate, this, IDM_ANDGATE);
    parent->Bind(wxEVT_COMMAND_MENU_SELECTED, &GameView::OnAddNotGate, this, IDM_NOTGATE);
    parent->Bind(wxEVT_COMMAND_MENU_SELECTED, &GameView::OnAddSRGate, this, IDM_SRFLIPFLOP);
    parent->Bind(wxEVT_COMMAND_MENU_SELECTED, &GameView::OnAddDGate, this, IDM_DFLIPFLOP);

    parent->Bind(wxEVT_COMMAND_MENU_SELECTED, &GameView::OnControlPoints, this, IDM_CONTROLPOINTS);

    mTimer.SetOwner(this);
    mTimer.Start(FrameDuration);

    Bind(wxEVT_TIMER, &GameView::OnTimer, this);

    mStopWatch.Start();

    // Starts game with level 1 on load.
    LoadStartLevel();
}


/**
 * Paint event, draws the window.
 * @param event Paint event object
 */
void GameView::OnPaint(wxPaintEvent& event)
{
    // Create a double-buffered display context
    wxAutoBufferedPaintDC dc(this);

    // Clear the image to black
    wxBrush background(*wxBLACK);
    dc.SetBackground(background);
    dc.Clear();

    // Create a graphics context
    auto gc =
        std::shared_ptr<wxGraphicsContext>(wxGraphicsContext::Create(dc));

    // Compute the time that has elapsed since the last call to OnPaint
    auto newTime = mStopWatch.Time();
    auto elapsed = (double)(newTime - mTime) * OneThousanth;
    mTime = newTime;

    // Update the game with the elapsed time for animations
    mGame.Update(elapsed);

    // Tell the game class to draw
    wxRect rect = GetRect();
    mGame.OnDraw(gc, rect.GetWidth(), rect.GetHeight());
}

/**
 * Handle the left mouse button down event
 * @param event Mouse event
 */
void GameView::OnLeftDown(wxMouseEvent& event)
{
    mGame.OnLeftDown(event.GetX(), event.GetY());
    mGrabbedItem = mGame.HitTest(mGame.GetX(), mGame.GetY());
}

/**
 * Handle the left mouse button up event
 * @param event Mouse event
 */
void GameView::OnLeftUp(wxMouseEvent& event)
{
    OnMouseMove(event);
}

/**
 * Handle the mouse move event
 * @param event Mouse event
 */
void GameView::OnMouseMove(wxMouseEvent &event)
{
    if (mGrabbedItem != nullptr)
    {
        if (event.LeftIsDown() && mGrabbedItem->IsGrabbable())
        {
            mGame.OnLeftDown(event.GetX(), event.GetY());
            mGrabbedItem->SetLocation(mGame.GetX(), mGame.GetY());
        }
        else
        {
            mGrabbedItem->Release();
            mGrabbedItem = nullptr;
        }
        Refresh();
    }
}

/**
 * Loads level 1 on game start.
 */
void GameView::LoadStartLevel()
{
    int level = 1;
    wxString levelName = "levels/level" + wxString::Format("%d",level) + ".xml";
    mGame.ResetTimer();
    mLevelLoader.LoadLevel(levelName, &mGame);
    GetGame().SetCurrentLevel(level);
}
/**
 * Handle the level selection
 * @param event Menu event
 */
void GameView::OnLevelSelect(wxCommandEvent& event)
{
    int level = event.GetId() - IDM_LEVEL0;
    wxString levelName = "levels/level" + wxString::Format("%d", level) + ".xml";
    mGame.ResetTimer();
    mLevelLoader.LoadLevel(levelName, &mGame);
    GetGame().SetCurrentLevel(level);
    GetGame().SetStateLoading();
    GetGame().GetScore()->ResetLevelScore();
    GetGame().GetScore()->ResetGameScore();
    Refresh();
}

/**
 * Handle toggling of control points
 * @param event Menu event
 */
void GameView::OnControlPoints(wxCommandEvent& event)
{
    // Note: ToggleControlPoints functionality commented out as it's not implemented in Game class
    // mGame.ToggleControlPoints();
}

/**
 * Handle timer events
 * @param event Timer event
 */
void GameView::OnTimer(wxTimerEvent& event)
{
    Refresh();
}

/**
 * Get the Game object
 * @return Reference to the Game object
 */
Game &GameView::GetGame()
{
    return mGame;
}

/**
 * Menu hander for Add Gate>Or Gate
 * @param event Mouse event
 */
void GameView::OnAddOrGate(wxCommandEvent& event)
{
    auto gate = make_shared<OrGate>(&mGame);
    mGame.Add(gate);
    Refresh();
}

/**
 * Menu hander for Add Gate>And Gate
 * @param event Mouse event
 */
void GameView::OnAddAndGate(wxCommandEvent& event)
{
    auto gate = make_shared<AndGate>(&mGame);
    mGame.Add(gate);
    Refresh();
}

/**
 * Menu hander for Add Gate>Not Gate
 * @param event Mouse event
 */
void GameView::OnAddNotGate(wxCommandEvent& event)
{
    auto gate = make_shared<NotGate>(&mGame);
    mGame.Add(gate);
    Refresh();
}

/**
* Menu hander for Add Gate>DFlipFlop Gate
* @param event Mouse event
*/
void GameView::OnAddDGate(wxCommandEvent& event)
{
    auto gate = make_shared<DFlipFlopGate>(&mGame);
    mGame.Add(gate);
    Refresh();
}

/**
* Menu hander for Add Gate>SR Flip Flop Gate
* @param event Mouse event
*/
void GameView::OnAddSRGate(wxCommandEvent& event)
{
    auto gate = make_shared<SrFlipFlopGate>(&mGame);
    mGame.Add(gate);
    Refresh();
}





