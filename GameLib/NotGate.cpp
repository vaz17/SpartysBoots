/**
 * @file NotGate.cpp
 * @author matthew vazquez
 */

#include "pch.h"
#include "NotGate.h"
#include "Game.h"
#include "InputPin.h"
#include "OutputPin.h"

/**
 * The size of an Not Gate
 * @returns Int size of gate in pixels
 */
const wxSize NotGateSize(50, 50);


/**
 * Constructor for or gate
 * @param game the game instance
 */
NotGate::NotGate(Game *game) : Gates(game)
{
    // Initialize NotGate-specific members

    auto x = GetX();
    auto y = GetY();
    auto w = NotGateSize.GetWidth();
    auto h = NotGateSize.GetHeight();

    wxPoint pointA(x - w/2,y);
    wxPoint pointB(x + w/2,y);

    mInput = std::make_shared<InputPin>(this, pointA);
    mOutput = std::make_shared<OutputPin>(this, pointB);


    mInput->SetState(States::Unknown);
    mOutput->SetState(States::Unknown);
}

/**
 * Computes the output using inputs and or gate logic
 */
void NotGate::ComputeOutput() {
    if (mInput->GetState() == States::Unknown) {
        mOutput->SetState(States::Unknown);
    } else {
        mOutput->SetState((mInput->GetState() == States::One) ? States::Zero : States::One);
    }
}

/**
 * Draws or gate
 * @param graphics Graphics context to draw on
 */
void NotGate::Draw(std::shared_ptr<wxGraphicsContext> graphics) {
    // Create a path to draw the gate shape


    mInput->Draw(graphics);
    mOutput->Draw(graphics);

    auto path = graphics->CreatePath();

    // Get the location and size
    auto x = GetX();
    auto y = GetY();
    auto w = NotGateSize.GetWidth();
    auto h = NotGateSize.GetHeight();

    // The three corner points of an OR gate
    wxPoint2DDouble p1(x - w / 2, y + h / 2); // Bottom left
    wxPoint2DDouble p2(x + w / 2, y);         // Center right
    wxPoint2DDouble p3(x - w / 2, y - h / 2); // Top left

    // Create the path for the gate
    path.MoveToPoint(p1);  // Start at bottom left
    path.AddLineToPoint(p3);  // Draw straight line to top left
    path.AddLineToPoint(p2);  // Draw straight line to center right
    path.AddLineToPoint(p1);  // Draw straight line to bottom left
    path.CloseSubpath();

    // Draw the circle for the NOT gate (inverter)
    path.AddCircle(x + w/2 + 5, y, 5);

    // Draw the path
    graphics->SetPen(*wxBLACK_PEN);
    graphics->SetBrush(*wxWHITE_BRUSH);
    graphics->DrawPath(path);
}

/**
 * Handle updates for animation
 * @param elapsed The time since the last update
 */
void NotGate::Update(double elapsed)
{
    mOutput->Update();
    ComputeOutput(); // Recompute the output on each update
}

/**
 * Handle a click on the item
 * @param x X location clicked on
 * @param y Y location clicked on
 */
void NotGate::OnClick(double x, double y)
{
    ComputeOutput();
}

/**
 * Get the Gate Width
 * @return NotGate Width in pixels
 */
double NotGate::getWidth()
{
    return NotGateSize.GetWidth();
}

/**
 * Get the Gate Height
 * @return NotGate Height in pixels
 */
double NotGate::getHeight()
{
    return NotGateSize.GetHeight();
}

/**
 * Test to see if we clicked on some draggable inside the item.
 * @param x X location clicked on
 * @param y Y location clicked on
 * @return Whatever we clicked on or NULL if none
 */
std::shared_ptr<IDraggable> NotGate::HitDraggable(int x, int y)
{
    if(mOutput->HitTest(x, y))
    {
        return mOutput;
    }

    return nullptr;
}


/**
 * Tests if an output pin has an input pin to connect to
 * @param pin The pin that is connected
 * @param lineEnd The spot of the mouse
 * @return True if there is a pin to be connected to
 */
bool NotGate::Connect(OutputPin *pin, wxPoint lineEnd)
{
    return mInput->Catch(pin, lineEnd);
}