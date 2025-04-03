/**
 * @file OutputPin.cpp
 * @author matthew vazquez
 */

#include "pch.h"
#include "OutputPin.h"

#include "Game.h"
#include "Item.h"
#include "Gates.h"
#include "InputPin.h"

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
OutputPin::OutputPin(Item *owner, wxPoint location) : mOwner(owner), mLocation(location)
{

}

/**
 * Draws Output pin
 * @param graphics Graphics context to draw on
 */
void OutputPin::Draw(std::shared_ptr<wxGraphicsContext> graphics)
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
 path2.MoveToPoint(loc.x - DefaultLineLength - PinSize/2,loc.y);
 path2.AddLineToPoint(loc.x - PinSize/2,loc.y);
 graphics->SetPen(wxPen(connectionColor, LineWidth));
 path2.CloseSubpath();

 graphics->DrawPath(path2);

 graphics->SetBrush(connectionColor);
 graphics->SetPen(*wxBLACK_PEN);

 path.AddCircle(loc.x, loc.y, PinSize/2);
 path.CloseSubpath();
 graphics->DrawPath(path);

 graphics->SetPen(wxPen(connectionColor, LineWidth));
 graphics->SetBrush(wxBrush());
 if (mDragging)
 {
  wxGraphicsPath path3 = graphics->CreatePath();

  wxPoint2DDouble p1(loc.x, loc.y);                      // Start point
  wxPoint2DDouble p4(mLineEnd.x, mLineEnd.y);           // End point (mouse position)

  // Calculate the distance between p1 and p4
  double distance = sqrt(pow(mLineEnd.x - loc.x, 2) + pow(mLineEnd.y - loc.y, 2));

  // Calculate the offset
  double offset = std::min(BezierMaxOffset, distance);

  // Set control points
  wxPoint2DDouble p2(loc.x + offset, loc.y);               // Control point for starting curvature
  wxPoint2DDouble p3(mLineEnd.x- offset, mLineEnd.y);               // Control point for ending curvature

  path3.MoveToPoint(p1);
  path3.AddCurveToPoint(p2, p3, p4);
  graphics->DrawPath(path3);
 }

  for (auto connectedPin : mConnected)
  {
   if (connectedPin != nullptr)
   {
    wxGraphicsPath path4 = graphics->CreatePath();
    auto mouth = connectedPin->GetAbsoluteLocation();

    wxPoint2DDouble p1(loc.x, loc.y);                      // Start point
    wxPoint2DDouble p4(mouth.x, mouth.y);           // End point (mouse position)

    // Calculate the distance between p1 and p4
    double distance = sqrt(pow(mouth.x - loc.x, 2) + pow(mouth.y - loc.y, 2));

    // Calculate the offset
    double offset = std::min(BezierMaxOffset, distance);

    // Set control points
    wxPoint2DDouble p2(loc.x + offset, loc.y);               // Control point for starting curvature
    wxPoint2DDouble p3(mouth.x- offset, mouth.y);               // Control point for ending curvature

    path4.MoveToPoint(p1);
    path4.AddCurveToPoint(p2, p3, p4);
    graphics->DrawPath(path4);
   }
 }

}

/**
 * Returns absolute location of pin
 * @return wxPoint Absolute location of pin
 */
wxPoint OutputPin::GetAbsoluteLocation()
{
 if (mOwner != nullptr )
 {
  auto x = mOwner->GetX() + mLocation.x  + DefaultLineLength + PinSize/2;
  auto y = mOwner->GetY() + mLocation.y;
  return wxPoint(x, y);
 }
 return wxPoint(mLocation.x, mLocation.y);
}

/**
 * Sets location of pin
 * @param x X location of pin
 * @param y Y location of pin
 */
void OutputPin::SetLocation(double x, double y)
{
 mDragging = true;
 mLineEnd = wxPoint(int(x), int(y));
}


/**
 * Did we click on the rod end?
 * @param x X location clicked on
 * @param y Y location clicked on
 * @return Did we click on it?
 */
bool OutputPin::HitTest(int x, int y)
{
 auto loc = GetAbsoluteLocation();

 if ((loc.x -x) * (loc.x - x) + (loc.y - y) * (loc.y - y) < PinSize * PinSize)
 {
  return true;
 }

 return false;

}

/**
 * Checks if there is an InputPin to connect to
 */
void OutputPin::Release()
{
 if(mDragging)
 {
  // The mouse has been released. See if there
  // is a pin we can connect?
  mOwner->GetGame()->TryToConnect(this, mLineEnd);
 }

 mDragging = false;
}

/**
 * Sets the InputPin connection to the list
 * @param connected InputPin that needs to be connected
 */
void OutputPin::SetConnection(InputPin* connected)
{
 // Add connected pin to the vector
 if (connected != nullptr)
 {
  // Avoid duplicates
  if (std::find(mConnected.begin(), mConnected.end(), connected) == mConnected.end())
  {
   mConnected.push_back(connected);
   connected->SetLine(this); // Assuming SetLine sets a reference back to this OutputPin
  }
 }
}

/**
 * Erases input pin from list
 * @param connected Input Pin that needs to be erased
 */
void OutputPin::Release(InputPin* connected)
{
 if (connected != nullptr)
 {
  // Check if the connected pin is in the list
  auto it = std::find(mConnected.begin(), mConnected.end(), connected);
  if (it != mConnected.end())
  {
   // Remove the pin from the vector
   mConnected.erase(it); // Erase it from the vector
  }
 }
}

void OutputPin::Update()
{
 for (InputPin* pin : mConnected)
 {
  pin->SetState(mState);
 }
}
