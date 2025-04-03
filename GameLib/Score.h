/**
 * @file Score.h
 * @author Benjamin Eyke
 *
 * Contains description of game scores, each can be updated individually.
 */
 
#ifndef SCORE_H
#define SCORE_H

/**
 * Objects of this class represent the Scores for the game.
 *
 * Linked to Scoreboard and Game, so that each receives updates.
 */
class Score {
private:
    /// Score for current level
    int mLevelScore = 0; //Default 0
    /// Score for the entire game
    int mGameScore = 0; //Default 0
    /// Score to update for a correct kick.
    int mGoodScore = 0;
    /// Score to update for a bad kick.
    int mBadScore = 0;
public:
    /**
     * Return the level score of current level
     * @return int
     */
    int GetLevelScore() const {return mLevelScore;}
    /**
     * Return the game score of entire game
     * @return int
     */
    int GetGameScore() const {return mGameScore;}

    /**
     * Updates the level score for the game
     * @param levelScore score to be set
     */

    void SetLevelScore(int levelScore) {mLevelScore += levelScore;}
    /**
     * Updates the game score for the game
     * @param gameScore score to be set
     */
    void SetGameScore(int gameScore) {mGameScore = gameScore;}

    /**
     * Updates score for good game actions.
     */
    void SetLevelScoreGood() {mLevelScore += mGoodScore;}

    /**
     * Updates score for bad game actions.
     */
    void SetLevelScoreBad() {mLevelScore += mBadScore;}

    /**
     * Sets good score to update from the scoreboard.
     * @param goodScore score to set for good kicks.
     */
    void SetGoodScore(int goodScore) {mGoodScore = goodScore;}

    /**
     * Sets bad score to update from the scoreboard.
     * @param badScore score to set for bad kicks.
     */
    void SetBadScore(int badScore) {mBadScore = badScore;}

    /**
     * Resets the score of the level to zero.
     */
    void ResetLevelScore() {mLevelScore = 0;}

    /**
     * Resets to score of the game to zero.
     */
    void ResetGameScore() {mGameScore = 0;}

};

#endif //SCORE_H
