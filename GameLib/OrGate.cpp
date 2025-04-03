/**
 * @file OrGate.cpp
 * @author matthew vazquez
 */

#include "pch.h"
#include "OrGate.h"
#include "Game.h"
#include "InputPin.h"
#include "OutputPin.h"

/**
 * The size of an Or Gate
 * @returns Int size of gate in pixels
 */
const wxSize OrGateSize(75, 50);

/**
 * Constructor for or gate
 * @param game the game instance
 */
OrGate::OrGate(Game *game) : Gates(game)
{
    auto x = GetX();
    auto y = GetY();
    auto w = OrGateSize.GetWidth();
    auto h = OrGateSize.GetHeight();

    wxPoint pointA(x - w/3,y - h/4);
    wxPoint pointB(x - w/3,y + h/4);
    wxPoint pointC(x + w/2,y);

    mInputA = std::make_shared<InputPin>(this, pointA);
    mInputB = std::make_shared<InputPin>(this, pointB);
    mOutput = std::make_shared<OutputPin>(this, pointC);

    mInputA->SetState(States::Unknown);
    mInputB->SetState(States::Unknown);
    mOutput->SetState(States::Unknown);



}

/**
 * Computes the output using inputs and or gate logic
 */
void OrGate::ComputeOutput() {
    if (mInputA->GetState() == States::Unknown || mInputB->GetState() == States::Unknown) {
        mOutput->SetState(States::Unknown);
    } else {
        mOutput->SetState((mInputA->GetState() == States::One || mInputB->GetState() == States::One) ? States::One : States::Zero);
    }
}

/**
 * Draws or gate
 * @param graphics Graphics context to draw on
 */
void OrGate::Draw(std::shared_ptr<wxGraphicsContext> graphics) {
    if (!graphics) return;

    // Create a path to draw the gate shape
    auto path = graphics->CreatePath();


    mInputA->Draw(graphics);
    mInputB->Draw(graphics);
    mOutput->Draw(graphics);

    // The location and size
    auto x = GetX();
    auto y = GetY();
    auto w = OrGateSize.GetWidth();
    auto h = OrGateSize.GetHeight();

    // The three corner points of an OR gate
    wxPoint2DDouble p1(x - w / 2, y + h / 2); // Bottom left
    wxPoint2DDouble p2(x + w / 2, y);         // Center right
    wxPoint2DDouble p3(x - w / 2, y - h / 2); // Top left

    // Control points used to create the Bezier curves
    auto controlPointOffset1 = wxPoint2DDouble(w * 0.5, 0);
    auto controlPointOffset2 = wxPoint2DDouble(w * 0.75, 0);
    auto controlPointOffset3 = wxPoint2DDouble(w * 0.2, 0);

    // Create the path for the gate
    path.MoveToPoint(p1);
    path.AddCurveToPoint(p1 + controlPointOffset1, p1 + controlPointOffset2, p2);
    path.AddCurveToPoint(p3 + controlPointOffset2, p3 + controlPointOffset1, p3);
    path.AddCurveToPoint(p3 + controlPointOffset3, p1 + controlPointOffset3, p1);
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
void OrGate::Update(double elapsed)
{
    mOutput->Update();
    ComputeOutput(); // Recompute the output on each update
}

/**
 * Handle a click on the item
 * @param x X location clicked on
 * @param y Y location clicked on
 */
void OrGate::OnClick(double x, double y)
{
    ComputeOutput();
}

/**
 * Get the Gate Width
 * @return OrGate Width in pixels
 */
double OrGate::getWidth()
{
    return OrGateSize.GetWidth();
}

/**
 * Get the Gate Height
 * @return OrGate Height in pixels
 */
double OrGate::getHeight()
{
    return OrGateSize.GetHeight();
}

/**
 * Test to see if we clicked on some draggable inside the item.
 * @param x X location clicked on
 * @param y Y location clicked on
 * @return Whatever we clicked on or NULL if none
 */
std::shared_ptr<IDraggable> OrGate::HitDraggable(int x, int y)
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
bool OrGate::Connect(OutputPin *pin, wxPoint lineEnd)
{
    return mInputA->Catch(pin, lineEnd) || mInputB->Catch(pin, lineEnd);
}