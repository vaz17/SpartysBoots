/**
 * @file OutputPin.h
 * @author matthew vazquez
 *
 *
 */
 
#ifndef OUTPUTPIN_H
#define OUTPUTPIN_H
#include "IDraggable.h"

class Item;
class InputPin;
enum class States;

/**
 * Class for Output pin
 */
class OutputPin : public IDraggable {
private:

 /// Item that owns pin
 Item* mOwner = nullptr;
 /// Location of pin
 wxPoint mLocation;
 /// State of pin
 States mState;

 /// Location of the line end when dragging
 wxPoint mLineEnd;

 /// Are we dragging the line?
 bool mDragging = false;

 /// Have we connected to anything?
 std::vector<InputPin*> mConnected;

 /// Maximum offset of Bezier control points relative to line ends
 static constexpr double BezierMaxOffset = 200;

 /// Line with for drawing lines between pins
 static const int LineWidth = 3;

 /// Default length of line from the pin
 int DefaultLineLength = 20;

public:
 OutputPin(Item *owner, wxPoint location);
 void SetLocation(double x, double y) override;
 void Draw(std::shared_ptr<wxGraphicsContext> graphics);
 wxPoint GetAbsoluteLocation();

 /**
  * Sets the state of the pin
  * @param state The state to be set
  */
 void SetState(States state) {mState = state;};

 /**
  * Gets the state of the pin
  * @return state The state of the pin
  */
 States GetState() {return mState;}

 bool HitTest(int x, int y);

 /**
  * Sees if item was clicked
  * @param x X location of click
  * @param y Y location of click
  */
 void OnClick(double x, double y ) override {};

 /**
  * Checks if item is grabbable
  * @return True since grabbable
  */
 bool IsGrabbable() override { return true; };

 void Release() override;

 void SetConnection(InputPin* caught);

 void Release(InputPin* caught);

 /**
  * Updates the state of the pin when it is connected.
  */
 void Update();

 /**
  * Changes default line length
  * @param length The new default line length
  */
 void ChangeDefaultLineLength(int length) { DefaultLineLength = length; };

 /**
  * Necessary override function, performs no purpose.
  * @param kick speed to kick with.
  */
 void Kick(double kick) override {};
};



#endif //OUTPUTPIN_H
