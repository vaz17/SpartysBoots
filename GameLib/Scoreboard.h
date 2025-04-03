/**
 * @file Scoreboard.h
 * @author matthew vazquez
 *
 *
 */
 
#ifndef SCOREBOARD_H
#define SCOREBOARD_H

#include "Item.h"

/**
 * Objects of this class represent the game Scoreboard
 * Displays level timer.
 * Displays the level score and game score.
 * Displays level instructions.
 */
class Scoreboard : public Item
{
private:
    /// X-coordinate of the scoreboard (taken from XML)
    int mX;

    /// Y-coordinate of the scoreboard (taken from XML)
    int mY;

    /// Amount to add for correct game actions
    int mGood;

    /// Amount to subtract for incorrect game actions
    int mBad;

    /// The score for current level
    int mLevelScore;

    /// Instruction text displayed in the scoreboard, from XML
    /// may contain line breaks.
    std::wstring mGoalText;

    /// Size of the scoreboard in virtual pixels
    static const wxSize ScoreboardSize;

    /// The spacing from the scores to the instructions presentation
    /// in virtual pixels
    const int SpacingScoresToInstructions = 40;

    /// The spacing between the instruction lines in virtual pixels.
    const int SpacingInstructionLines = 17;

    /// Hours remaining on the level
    int mHours = 0;

    /// Minutes remaining on the level
    int mMinutes = 0;

    /// Seconds remaining on the level
    int mSeconds = 0;

public:
    Scoreboard(Game* game);
    void XmlLoad(wxXmlNode* node) override;
    void Draw(std::shared_ptr<wxGraphicsContext> graphics) override;
    void OnClick(double x, double y) override;
    void Update(double elapsed) override;

    /**
     * Accept a visitor
     * @param visitor The visitor we accept
     */
    void Accept(ItemVisitor* visitor) override { visitor->VisitScoreboard(this); }

    /**
     * Returns x coordinate of scoreboard
     * @return x coord
     */
    int GetmX() const { return mX; }

    /**
     * Returns y coordinate of scoreboard
     * @return y coord
     */
    int GetmY() const { return mY; }
};



#endif //SCOREBOARD_H
