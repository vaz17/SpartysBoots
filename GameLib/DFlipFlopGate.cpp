/**
 * @file DFlipFlopGate.cpp
 * @author matthew vazquez
 */

#include "pch.h"
#include "DFlipFlopGate.h"
#include "Game.h"
#include "InputPin.h"
#include "OutputPin.h"

/**
 * The size of an DFlipFlop Gate
 * @returns Int size of gate in pixels
 */
const wxSize DFlipFlopSize(50, 75);

/// Gap between the edge of the flip flop and the labels
const int DFlipFlopLabelMargin = 3;

/// How large the clock input triagle is in pixels width and height
const int DFlipFlopClockSize = 10;

/**
 * Constructor for or gate
 * @param game the game instance
 */
DFlipFlopGate::DFlipFlopGate(Game *game) : Gates(game)
{

    auto x = GetX();
    auto y = GetY();
    auto w = DFlipFlopSize.GetWidth();
    auto h = DFlipFlopSize.GetHeight();
    wxPoint pointA(x - w/2,y - h/4);
    wxPoint pointB(x - w/2,y + h/4);
    wxPoint pointC(x + w/2,y - h/4);
    wxPoint pointD(x + w/2,y + h/4);

    mInputA = std::make_shared<InputPin>(this, pointA);
    mInputB = std::make_shared<InputPin>(this,pointB);
    mOutputA = std::make_shared<OutputPin>(this, pointC);
    mOutputB = std::make_shared<OutputPin>(this, pointD);

    mInputA->SetState(States::Unknown);
    mInputB->SetState(States::Unknown);
    mOutputA->SetState(States::Zero);
    mOutputB->SetState(States::One);

}

/**
 * Computes the output using inputs.
 * When clock transitions from 0 to 1, input A is saved as output.
 */
void DFlipFlopGate::ComputeOutput()
{
    States D = mInputA->GetState();
    States clock = mInputB->GetState();

    // Clock transitions from 0 to 1
    if (mPreviousClock == States::Zero && clock == States::One)
    {
        mOutputA->SetState(D);
        if (D == States::Zero)
        {
            mOutputB->SetState(States::One);
        }
        else
        {
            mOutputB->SetState(States::Zero);
        }
    }
    mPreviousClock = clock;
}

/**
 * Draws or gate
 * @param graphics Graphics context to draw on
 */
void DFlipFlopGate::Draw(std::shared_ptr<wxGraphicsContext> graphics) {
    // Create a path to draw the gate shape
    auto path = graphics->CreatePath();

    mInputA->Draw(graphics);
    mInputB->Draw(graphics);
    mOutputA->Draw(graphics);
    mOutputB->Draw(graphics);

    // Get the location and size
    auto x = GetX();
    auto y = GetY();
    auto w = DFlipFlopSize.GetWidth();
    auto h = DFlipFlopSize.GetHeight();

    // Manually draw the clock triangle using lines
    wxPoint2DDouble clockP1(x - w / 2, y + h/4 + 5);    // Left side (clock input)
    wxPoint2DDouble clockP2(x - w / 2 + 10, y + h /4); // Top right of triangle
    wxPoint2DDouble clockP3(x - w/2, y + h/4 - 5); // Bottom right of triangle

    path.MoveToPoint(clockP1);
    path.AddLineToPoint(clockP2);
    path.AddLineToPoint(clockP3);
    path.CloseSubpath();

    auto path2 = graphics->CreatePath();
    path2.AddRectangle(x - w/2, y - h/2, w, h);
    path2.CloseSubpath();

    // Draw the path
    graphics->SetPen(*wxBLACK_PEN);
    graphics->SetBrush(*wxWHITE_BRUSH);

    graphics->DrawPath(path2);
    graphics->DrawPath(path);

    // Draw the input/output labels
    auto font = graphics->CreateFont(15, L"Arial",wxFONTFLAG_BOLD, *wxBLACK);
    graphics->SetFont(font);
    graphics->DrawText(L"D", x - w / 2 + 3, y - h/3);
    graphics->DrawText(L"Q", x + w/2 - 15, y - h/3);
    graphics->DrawText(L"Q'", x + w/2 - 18, y + h/6);
}

/**
 * Handle updates for animation
 * @param elapsed The time since the last update
 */
void DFlipFlopGate::Update(double elapsed)
{
    mOutputA->Update();
    mOutputB->Update();
    ComputeOutput(); // Recompute the output on each update
}

/**
 * Handle a click on the item
 * @param x X location clicked on
 * @param y Y location clicked on
 */
void DFlipFlopGate::OnClick(double x, double y)
{
    // For this implementation, clicking on the gate will toggle input A
    // You might want to implement more sophisticated behavior
    //setInputA(getInputA() == States::Zero ? States::One : States::Zero);
    ComputeOutput();
}

/**
 * Get the Gate Width
 * @return DFlipFLopGate Width in pixels
 */
double DFlipFlopGate::getWidth()
{
    return DFlipFlopSize.GetWidth();
}

/**
 * Get the Gate Height
 * @return DFlipFlopGate Height in pixels
 */
double DFlipFlopGate::getHeight()
{
    return DFlipFlopSize.GetHeight();
}

/**
 * Test to see if we clicked on some draggable inside the item.
 * @param x X location clicked on
 * @param y Y location clicked on
 * @return Whatever we clicked on or NULL if none
 */
std::shared_ptr<IDraggable> DFlipFlopGate::HitDraggable(int x, int y)
{
    if(mOutputA->HitTest(x, y))
    {
        return mOutputA;
    }
    if (mOutputB->HitTest(x, y))
    {
        return mOutputB;
    }

    return nullptr;
}

/**
 * Tests if an output pin has an input pin to connect to
 * @param pin The pin that is connected
 * @param lineEnd The spot of the mouse
 * @return True if there is a pin to be connected to
 */
bool DFlipFlopGate::Connect(OutputPin *pin, wxPoint lineEnd)
{
    return mInputA->Catch(pin, lineEnd) || mInputB->Catch(pin, lineEnd);
}