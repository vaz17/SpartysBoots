/**
 * @file InputPin.cpp
 * @author matthew vazquez
 */

#include "pch.h"
#include "InputPin.h"
#include "Item.h"
#include "Gates.h"
#include "OutputPin.h"

/// Diameter to draw the pin in pixels
const int PinSize = 10;

/// Color to use for drawing a zero connection wire
const wxColour ConnectionColorZero = *wxBLACK;

/// Color to use for drawing a one connection wire
const wxColour ConnectionColorOne = *wxRED;

/// Color to use for drawing an unknown state connection wire
const wxColour ConnectionColorUnknown= wxColour(128, 128, 128);


/**
 * Constructs pin
 * @param owner The item that owns this pin
 * @param location The location to set the pin
 */
InputPin::InputPin(Item *owner, wxPoint location) : mOwner(owner), mLocation(location)
{

}

/**
 * Draws Input pin
 * @param graphics Graphics context to draw on
 */
void InputPin::Draw(std::shared_ptr<wxGraphicsContext> graphics)
{
 wxColour connectionColor; // Declare connectionColor here

 // Set the connection color based on the state
 if (mState == States::Zero)
 {
  connectionColor = ConnectionColorZero;
 }
 else if (mState == States::One)
 {
  connectionColor = ConnectionColorOne;
 }
 else if (mState == States::Unknown)
 {
  connectionColor = ConnectionColorUnknown;
 }

 // Draw input and output connectors
 wxGraphicsPath path = graphics->CreatePath();
 auto loc = GetAbsoluteLocation();

 wxGraphicsPath path2 = graphics->CreatePath();
 path2.MoveToPoint(loc.x + DefaultLineLength + PinSize/2,loc.y);
 path2.AddLineToPoint(loc.x + PinSize/2,loc.y);
 graphics->SetPen(wxPen(connectionColor, LineWidth));
 graphics->DrawPath(path2);


 graphics->SetBrush(connectionColor);
 graphics->SetPen(*wxBLACK_PEN);
 path.AddCircle(loc.x, loc.y, PinSize / 2);
 path.CloseSubpath();
 graphics->DrawPath(path);
}

/**
 * Returns absolute location of pin
 * @return wxPoint Absolute location of pin
 */
wxPoint InputPin::GetAbsoluteLocation()
{
 if (mOwner != nullptr )
 {
  auto x = mOwner->GetX() + mLocation.x  - DefaultLineLength - PinSize/2;
  auto y = mOwner->GetY() + mLocation.y;
  return wxPoint(x, y);
 }
 //return wxPoint(mLocation.x  - DefaultLineLength - PinSize/2, mLocation.y);
 return mLocation;

}

/**
 * Tries to catch this pin if valid
 * @param pin Output pin that is to be connected
 * @param lineEnd Point at end of line
 * @return True if caught
 */
bool InputPin::Catch(OutputPin *pin, wxPoint lineEnd)
{
 auto loc = GetAbsoluteLocation();
 auto relative = lineEnd - loc;

 if( this->HitTest(lineEnd.x, lineEnd.y))
 {
  if(mLine != nullptr)
  {
   mLine->SetConnection(nullptr);
  }

  pin->SetConnection(this);
 }

 return false;
}

/**
 * Did we click on the rod end?
 * @param x X location clicked on
 * @param y Y location clicked on
 * @return Did we click on it?
 */
bool InputPin::HitTest(int x, int y)
{
 auto loc = GetAbsoluteLocation();

 if ((loc.x -x) * (loc.x - x) + (loc.y - y) * (loc.y - y) < PinSize * PinSize)
 {
  return true;
 }

 return false;

}

/**
 * Set's the line between two pins
 * @param line Line that needs to be set
 */
void InputPin::SetLine(OutputPin *line)
{
 if (mLine != nullptr)
 {
  mLine->Release(this);
  mLine = line;
 } else
 {
  mLine = line;
 }
}

