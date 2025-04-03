/**
 * @file SrFlipFlopGate.cpp
 * @author matthew vazquez
 */

#include "pch.h"
#include "SrFlipFlopGate.h"
#include "Game.h"
#include "InputPin.h"
#include "OutputPin.h"



/**
 * The size of an SrFlipFLop Gate
 * @returns Int size of gate in pixels
 */
const wxSize SRFlipFlopSize(50, 75);

/// Gap between the edge of the flip flop and the labels
const int SRFlipFlopLabelMargin = 3;

/**
 * Constructor for or gate
 * @param game the game instance
 */
SrFlipFlopGate::SrFlipFlopGate(Game *game) : Gates(game)
{
    auto x = GetX();
    auto y = GetY();
    auto w = SRFlipFlopSize.GetWidth();
    auto h = SRFlipFlopSize.GetHeight();

    wxPoint pointA(x - w/2,y - h/4);
    wxPoint pointB(x - w/2,y + h/4);
    wxPoint pointC(x + w/2,y - h/4);
    wxPoint pointD(x + w/2,y + h/4);

    mInputA = std::make_shared<InputPin>(this, pointA);
    mInputB = std::make_shared<InputPin>(this,pointB);
    mOutputA = std::make_shared<OutputPin>(this, pointC);
    mOutputB = std::make_shared<OutputPin>(this, pointD);

    mInputA -> SetState(States::Unknown);
    mInputB -> SetState(States::Unknown);
    mOutputA -> SetState(States::Zero);
    mOutputB -> SetState(States::One);
}

/**
 * Computes the output of the gate.
 * When S is true, Q is set to true
 * When R is true, Q is set to false
 * If both are true, input is invalid, set to unknown.
 */
void SrFlipFlopGate::ComputeOutput() {
    States S = mInputA->GetState();
    States R = mInputB->GetState();

    if (S == States::One && R == States::One) // Invalid inputs.
    {
        mOutputA->SetState(States::Unknown);
        mOutputB->SetState(States::Unknown);
    }
    else if (S == States::One) // Sets output to true.
    {
        mOutputA->SetState(States::One);
        mOutputB->SetState(States::Zero);
    }
    else if (R == States::One) // Resetting the output.
    {
        mOutputA->SetState(States::Zero);
        mOutputB->SetState(States::One);
    }
    //If both are zero, no change.
}

/**
 * Draws or gate
 * @param graphics Graphics context to draw on
 */
void SrFlipFlopGate::Draw(std::shared_ptr<wxGraphicsContext> graphics) {
    // Create a path to draw the gate shape
    auto path = graphics->CreatePath();

    mInputA->Draw(graphics);
    mInputB->Draw(graphics);
    mOutputA->Draw(graphics);
    mOutputB->Draw(graphics);

    // Get the location and size
    auto x = GetX();
    auto y = GetY();
    auto w = SRFlipFlopSize.GetWidth();
    auto h = SRFlipFlopSize.GetHeight();

    path.AddRectangle(x - w/2, y - h/2, w, h);

    // Draw the path
    graphics->SetPen(*wxBLACK_PEN);
    graphics->SetBrush(*wxWHITE_BRUSH);
    graphics->DrawPath(path);

    // Draw the input/output labels
    auto font = graphics->CreateFont(15, L"Arial",wxFONTFLAG_BOLD, *wxBLACK);
    graphics->SetFont(font);
    graphics->DrawText(L"S", x - w / 2 + 3, y - h/3);
    graphics->DrawText(L"R", x - w/2 + 3, y + h/6);
    graphics->DrawText(L"Q", x + w/2 - 15, y - h/3);
    graphics->DrawText(L"Q'", x + w/2 - 18, y + h/6);
}

/**
 * Handle updates for animation
 * @param elapsed The time since the last update
 */
void SrFlipFlopGate::Update(double elapsed)
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
void SrFlipFlopGate::OnClick(double x, double y)
{
    // For this implementation, clicking on the gate will toggle input A
    // You might want to implement more sophisticated behavior
    ComputeOutput();
}

/**
 * Get the Gate Width
 * @return SrFlipFlopGate Width in pixels
 */
double SrFlipFlopGate::getWidth()
{
    return SRFlipFlopSize.GetWidth();
}

/**
 * Get the Gate Height
 * @return SrFlipFlopGate Height in pixels
 */
double SrFlipFlopGate::getHeight()
{
    return SRFlipFlopSize.GetHeight();
}

/**
 * Test to see if we clicked on some draggable inside the item.
 * @param x X location clicked on
 * @param y Y location clicked on
 * @return Whatever we clicked on or NULL if none
 */
std::shared_ptr<IDraggable> SrFlipFlopGate::HitDraggable(int x, int y)
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
bool SrFlipFlopGate::Connect(OutputPin *pin, wxPoint lineEnd)
{
    return mInputA->Catch(pin, lineEnd) || mInputB->Catch(pin, lineEnd);
}