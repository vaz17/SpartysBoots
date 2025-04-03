/**
 * @file Timer.h
 * @author Benjamin Eyke
 *
 * Contains description of game Timer.
 */
 
#ifndef TIMER_H
#define TIMER_H

/**
 * Objects of this type represent the Scoreboard Timer
 * Linked to the scoreboard and game, which updates the timer.
 * Updates every second.
 */
class Timer {
private:
    /// The start time for the level, add setter function if adjusting time.
    double mStartTime = 120;
    /// The time left for the level, used for scoreboard display
    double mRemainingTime;
    /// Tracks the time passed by elapsed so that updates are made every second.
    /// Needed because the elapsed from game is not exactly a second.
    double mElapsedTime;

public:
    /**
     * Default constructor for timer.
     */
    Timer() : mRemainingTime(mStartTime) {}

    /**
     * Constructor of timer with start time.
     * @param timerStart initializes timer with a start time
     */
    Timer(double timerStart) : mRemainingTime(timerStart) {}

    void Update(double elapsed);

    /**
     * Returns the remaining time in the level
     * @return time remaining in the level
     */
    double GetRemainingTime() const { return mRemainingTime; }

    /**
     * Resets the timer to the start time for the level. Does so on level load.
     */
    void Reset() {mRemainingTime = mStartTime;}

};



#endif //TIMER_H
