/**
 * @file AndGate.cpp
 * @author matthew vazquez
 *
 * Implementation of AndGate objects are declared in this file.
 */

#include "pch.h"
#include "AndGate.h"
#include "Game.h"
#include "InputPin.h"
#include "OutputPin.h"

/**
 * The size of an And Gate
 * @returns Int size of gate in pixels
 */
const wxSize AndGateSize(75, 50);

/// How far the Bezier control points are for the AND
/// gate to the right of ends of the curve as a percentage
/// of the gate height.
const double AndGateControlPointOffset = 0.75;

/**
 * Constructor for or gate
 * @param game the game instance
 */
AndGate::AndGate(Game *game) : Gates(game)
{
    auto x = GetX();
    auto y = GetY();
    auto w = AndGateSize.GetWidth();
    auto h = AndGateSize.GetHeight();

    wxPoint pointA(x - w/2,y - h/4);
    wxPoint pointB(x - w/2,y + h/4);
    wxPoint pointC(x + w/2,y);

    mInputA = std::make_shared<InputPin>(this, pointA);
    mInputB = std::make_shared<InputPin>(this,pointB);
    mOutput = std::make_shared<OutputPin>(this, pointC);

    mInputA -> SetState(States::Unknown);
    mInputB -> SetState(States::Unknown);
    mOutput -> SetState(States::Unknown);
}

/**
 * Computes the output using inputs and or gate logic
 */
void AndGate::ComputeOutput() {
    if (mInputA->GetState() == States::Unknown || mInputB->GetState()  == States::Unknown) {
        mOutput->SetState(States::Unknown);
    } else {
        mOutput->SetState((mInputA->GetState() == States::One && mInputB->GetState() == States::One) ? States::One : States::Zero);
    }
}

/**
 * Draws or gate
 * @param graphics Graphics context to draw on
 */
void AndGate::Draw(std::shared_ptr<wxGraphicsContext> graphics) {
    // Create a path to draw the gate shape

    mInputA->Draw(graphics);
    mInputB->Draw(graphics);
    mOutput->Draw(graphics);

    auto path = graphics->CreatePath();

    // Get the location and size
    auto x = GetX();
    auto y = GetY();
    auto w = AndGateSize.GetWidth();
    auto h = AndGateSize.GetHeight();

    // The three corner points of an OR gate
    wxPoint2DDouble p1(x - w / 2, y + h / 2); // Bottom left
    wxPoint2DDouble p2(x + w / 2, y);         // Center right
    wxPoint2DDouble p3(x - w / 2, y - h / 2); // Top left
    wxPoint2DDouble p4(x, y + h / 2); // Bottom center-right
    wxPoint2DDouble p5(x, y - h / 2); // Top center-right

    // Control points for the curved side (Bezier curve) of the AND gate
    auto controlPointOffset = wxPoint2DDouble(w * AndGateControlPointOffset, 0);

    // Create the path for the gate
    path.MoveToPoint(p1);  // Start at bottom left
    path.AddLineToPoint(p3);  // Draw straight line to Top left
    path.AddLineToPoint(p5); //Draw straight line to top middle
    path.AddCurveToPoint(p5 + controlPointOffset, p4 + controlPointOffset, p4);  // Draw the curved top right
    //path.AddCurveToPoint(p1 + controlPointOffset, p1 + controlPointOffset, p1);  // Draw the curved top right
    path.CloseSubpath();

    // Draw the path
    graphics->SetPen(*wxBLACK_PEN);
    graphics->SetBrush(*wxWHITE_BRUSH);
    graphics->DrawPath(path);
}

/**
 * Handle updates for animation
 * @param elapsed The time since the last update
 */
void AndGate::Update(double elapsed)
{
    mOutput->Update();
    ComputeOutput(); // Recompute the output on each update
}

/**
 * Handle a click on the item
 * @param x X location clicked on
 * @param y Y location clicked on
 */
void AndGate::OnClick(double x, double y)
{
    // For this implementation, clicking on the gate will toggle input A
    // You might want to implement more sophisticated behavior
    //mInputA->SetState(mInputA->GetState() == States::Zero ? States::One : States::Zero);
    ComputeOutput();
}


/**
 * Get the Gate Width
 * @return NotGate Width in pixels
 */
double AndGate::getWidth()
{
    return AndGateSize.GetWidth();
}

/**
 * Get the Gate Height
 * @return AndGate Height in pixels
 */
double AndGate::getHeight()
{
    return AndGateSize.GetHeight();
}

/**
 * Test to see if we clicked on some draggable inside the item.
 * @param x X location clicked on
 * @param y Y location clicked on
 * @return Whatever we clicked on or NULL if none
 */
std::shared_ptr<IDraggable> AndGate::HitDraggable(int x, int y)
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
bool AndGate::Connect(OutputPin *pin, wxPoint lineEnd)
{
    return mInputA->Catch(pin, lineEnd) || mInputB->Catch(pin, lineEnd);
}