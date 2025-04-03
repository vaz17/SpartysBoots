/**
* @file Game.h
 * @author Benjamin Eyke
 */

#ifndef GAME_H
#define GAME_H

#include <memory>
#include <vector>
#include <wx/xml/xml.h>
#include <wx/graphics.h>

#include "OutputPin.h"
#include "Product.h"
#include "Score.h"
#include "Timer.h"
#include "LevelLoader.h"

class Item;

/**
 *  Class representing the game environment.
 *
 * The Game class is responsible for managing the items
 */
class Game
{
private:
    /// Vector of all items in the game. Cannot be duplicated.
    std::vector<std::shared_ptr<Item>> mItems;

    /// Play Field Width
    int mPlayfieldWidth = 1200;
    /// Play Field Height
    int mPlayfieldHeight = 800;

    /// Scale based on Width and Height
    double mScale;
    /// Virtual offset of X
    double mXOffset;
    /// Virtual offset of Y
    double mYOffset;

    /// X coordinate of game.
    double mX;
    /// Y coordinate of game.
    double mY;

    /// Timer displayed on scoreboard, used to calculate score.
    Timer mTimer;

    /// Shared pointer for the score, so that multiple classes can access
    std::shared_ptr<Score> mScore = std::make_shared<Score>();

    /// Keeps track of number of products on conveyor
    int mNumProducts = 0;

    /// Helps load levels from xml files.
    LevelLoader mLevelLoader;

    /// Maintains the state of game for game sequencing
    enum class State {Loading, Loaded, LoadingNextlLevel, Ending, Ended};

    /// Maintains current state of game. Initial state of the game is loading.
    State mCurrentState = State::Loading;

    /// Delay for ending the level
    double mEndDelay = 0;

    /// Delay for starting the level
    double mStartDelay = 0;

    /// The current level of the game
    int mCurrentLevel = 1;

    /// Maximum number of levels in the game
    int mMaxLevel = 8;

    /// Current time bonus of game
    int mTimeBonus = 30;

    /// Flag to help prevent double endings
    bool mGameEnded = false;

public:
    Game();

    void OnDraw(std::shared_ptr<wxGraphicsContext> graphics, int width, int height);
    void Add(std::shared_ptr<Item> item);
    void Add(std::shared_ptr<Item> item, int customX, int customY);
    void Update(double elapsed);
    void Clear();
    std::shared_ptr<IDraggable> HitTest(int x, int y);
    void XmlGame(wxXmlNode *node);
    void XmlItem(wxXmlNode *node);
    void OnMouseDown(int x, int y);
    void AddProduct(wxXmlNode *node, std::shared_ptr<Conveyor> conveyor);
    void AdjustPosition(std::shared_ptr<Item> item, int &x, int &y);

    /**
     * Gets the pointer to the game score.
     * @return shared pointer to game score.
     */
    std::shared_ptr<Score> GetScore() {return mScore;}

    /**
     * Gets the game timer.
     * @return Timer on the scoreboard
     */
    Timer GetTimer() {return mTimer;}

    /**
     * Resets the timer for each level.
     */
    void ResetTimer();

    /**
     * Get the width of the game area
     * @return Game area width in pixels
     */
    int GetWidth() const { return mPlayfieldWidth; }

    void OnLeftDown(int x, int y);

    /**
     * Get the height of the game area
     * @return Game area height in pixels
     */
    int GetHeight() const { return mPlayfieldHeight; }

    /**
    * Get the x coordinate of the game area
    * @return x coordinate.
    */
    double GetX() const { return mX; }

    /**
    * Get the y coordinate of the game area.
    * @return y coordinate.
    */
    double GetY() const { return mY; }

    void Accept(ItemVisitor* visitor);

    /**
     * Get the scale of game window.
     * @return scale of game
     */
    double GetScale() const { return mScale; }

    /**
     * Sets the number of products in the game.
     * @param number new number of products
     */
    void SetNumProducts(int number) { mNumProducts = number; }

    /**
     * Returns the number of products in the game
     * @return number of products in game
     */
    int GetNumProducts() const { return mNumProducts; }

    /**
     * Attempts to connect an output pin to a line.
     * @param pin pint to connect.
     * @param lineEnd line to connect.
     */
    void TryToConnect(OutputPin* pin, wxPoint lineEnd);

    /**
     * Sets state of game to ending.
     */
    void EndGame();

    /**
     * Sets the state of game.
     * @param newState state to set.
     */
    void SetState(State newState);

    /**
     * Sets the state of game to loading.
     */
    void SetStateLoading();

    void DisplayMessage(std::shared_ptr<wxGraphicsContext> gc, double scale, wxRect rect, wxString message);

    /**
     * Uses LevelLoader class to load levels from Xml to game.
     * @param level level to be loaded.
     */
    void LoadNextLevel(int level);

    /**
     * Updates the score of the game when level finishes.
     */
    void UpdateScore();
    void UpdateTimeBonus();

    /**
     * Gets the current state of the game.
     * @return State of the game.
     */
    State GetState() { return mCurrentState; }

    /**
     * Sets the current level of the game.
     * @param level level to be set.
     */
    void SetCurrentLevel(int level) { mCurrentLevel = level; }

    /**
     * Get the current level of the game.
     * @return current level of the game.
     */
    int GetCurrentLevel() { return mCurrentLevel; }

    /**
     * Get the current time bonus of the game.
     * @return current time bonus.
     */
    int GetTimeBonus() { return mTimeBonus; }

};


#endif // GAME_H