/**
* @file GameView.h
 * @author matthew vazquez
 *
 * The GameView class creates window for the game.
 */

#ifndef PROJECT1_GAMEVIEW_H
#define PROJECT1_GAMEVIEW_H

#include <wx/wx.h>
#include <wx/timer.h>
#include "Game.h"
#include "LevelLoader.h"

/**
 * Class that implements our game view window.
 */
class GameView : public wxWindow {
private:
    /// An object that describes our game
    Game mGame;

    /// Class to load on level selects from menu
    LevelLoader mLevelLoader;

    /// Timer that allows for animation
    wxTimer mTimer;

    /// Stopwatch used to measure elapsed time
    wxStopWatch mStopWatch;

    /// The last stopwatch time
    long mTime = 0;

    /// Any item we are currently dragging
    std::shared_ptr<IDraggable> mGrabbedItem;

public:
    void Initialize(wxFrame* parent);
    void LoadStartLevel();
    void OnLevelSelect(wxCommandEvent& event);
    void OnTimer(wxTimerEvent& event);
    void OnPaint(wxPaintEvent& event);

    void OnLeftDown(wxMouseEvent& event);
    void OnLeftUp(wxMouseEvent& event);
    void OnMouseMove(wxMouseEvent& event);

    void OnAddOrGate(wxCommandEvent& event);
    void OnAddAndGate(wxCommandEvent& event);
    void OnAddNotGate(wxCommandEvent& event);
    void OnAddSRGate(wxCommandEvent& event);
    void OnAddDGate(wxCommandEvent& event);

    void OnControlPoints(wxCommandEvent& event);

    Game &GetGame();
};

#endif //PROJECT1_GAMEVIEW_H