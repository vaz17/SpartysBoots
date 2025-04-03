/**
 * @file InputPin.h
 * @author matthew vazquez
 *
 *
 */
 
#ifndef INPUTPIN_H
#define INPUTPIN_H

class Item;
class OutputPin;
enum class States;

/**
 * Class for Input pin
 */
class InputPin {
private:
 /// Item that owns pin
 Item* mOwner = nullptr;

 /// Location of pin
 wxPoint mLocation;

 /// State of pin
 States mState;

 /// Line we are connected to
 OutputPin* mLine = nullptr;

 /// Maximum offset of Bezier control points relative to line ends
 static constexpr double BezierMaxOffset = 200;

 /// Line with for drawing lines between pins
 static const int LineWidth = 3;

 /// Default length of line from the pin
 static const int DefaultLineLength = 20;

public:
 InputPin(Item *owner, wxPoint location);

 /**
  * Sets location of pin
  * @param x X location of pin
  * @param y Y location of pin
  */
 void SetLocation(int x, int y) {mLocation = wxPoint(x, y);}

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

 bool Catch(OutputPin *pin, wxPoint lineEnd);
 void SetLine(OutputPin* line);
 bool HitTest(int x, int y);

};



#endif //INPUTPIN_H
