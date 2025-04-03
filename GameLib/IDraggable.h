/**
 * @file IDraggable.h
 * @author matthew vazquez
 *
 *
 */
 
#ifndef IDRAGGABLE_H
#define IDRAGGABLE_H

/**
 * Interface for something in the game that is draggable
 */
class IDraggable
{
public:
 /**
  * Set the location of the draggable thing
  * @param x X location in pixels
  * @param y Y location in pixels
  */
 virtual void SetLocation(double x, double y) = 0;

 /**
 * Check if item was clicked at location
 * @param x X location relative to the game
 * @param y Y location relative to the game
 */
 virtual void OnClick(double x, double y) = 0;

 /**
  * Determines if item is draggable.
  * @return True if draggable, false otherwise
  */
 virtual bool IsGrabbable() = 0;

 /**
 * Release after moving
 */
 virtual void Release() = 0;

 /**
  * Signals draggable objects to be kicked.
  * @param kick speed to kick with.
  */
 virtual void Kick(double kick) = 0;
};

#endif //IDRAGGABLE_H

