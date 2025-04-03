/**
 * @file Timer.cpp
 * @author Benjamin Eyke
 */

#include "pch.h"
#include "Timer.h"

/**
 * Updates the remaining time on the timer.
 * @param elapsed time elapsed, from game.
 */
void Timer::Update(double elapsed)
{
 if (mRemainingTime > 0)
 {
  mElapsedTime += elapsed;
 }
 while (mElapsedTime >= 1.0) {
  mRemainingTime -= 1.0;
  mElapsedTime -= 1.0;
 }
 if (mRemainingTime <= 0)
 {
  mRemainingTime = 0;
 }
}


