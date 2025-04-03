/**
 * @file Sparty.cpp
 * @author Shawn Porto
 *
 *
 */

#include "pch.h"
#include "Sparty.h"

#include <wx/dcbuffer.h>

#include "Game.h"
#include "Gates.h"
#include "InputPin.h"

using namespace std;

/// Image for the sparty background, what is behind the boot
const std::wstring SpartyBackImage = L"images/sparty-back.png";

/// Image for the Sparty boot
const std::wstring SpartyBootImage = L"images/sparty-boot.png";

/// Image for the Sparty front, what is in front of the boot
const std::wstring SpartyFrontImage = L"images/sparty-front.png";

/// Pivot point for the Sparty boot image as a fraction of
/// the width and height.
wxPoint2DDouble SpartyBootPivot = wxPoint2DDouble(0.5, 0.55);

/// The maximum rotation for Sparty's boot in radians
const double SpartyBootMaxRotation = 0.8;

/// The point in the kick animation when the product
/// actually is kicked. If the kick duration is 0.25,
/// we kick when 0.25 * 0.35 seconds have elapsed.
const double SpartyKickPoint = 0.35;

/// What percentage of the way down Sparty do we find the tip of his boot?
/// This means the location of the boot when kicking is 80% of the
/// way from the top of the Sparty image.
const double SpartyBootPercentage = 0.80;

///ADDED BY SHAWN
/// The point that products will get to on the conveyor in relation to sparty's x position
const double ProductKickPointX = 100;

/// Color to use for drawing a zero connection wire
const wxColour ConnectionColorZero = *wxBLACK;

/// Color to use for drawing a one connection wire
const wxColour ConnectionColorOne = *wxRED;

/// Color to use for drawing an unknown state connection wire
const wxColour ConnectionColorUnknown= wxColour(128, 128, 128);

/// Constants used for drawing Wire
/// Offset from pin connect to Sparty.
const int WirePinOffset = 25;
/// Point where wire is drawn up to.
const int WireUpwardPointY = 320;
/// Point where wire is drawn to left side of screen.
const int WireLeftwardPointX = 80;
/// Point where wire is drawn into Sparty.
const int WireOffsetSparty = 70;

/**
 * Constructs the Sparty Game Object
 * @param game the Game that this is apart of
 */
Sparty::Sparty(Game* game) : Item(game, SpartyBackImage)
{
    mSpartyBackImage = make_shared<wxImage>(SpartyBackImage, wxBITMAP_TYPE_PNG);
    mSpartyBackBitmap = make_shared<wxBitmap>(*mSpartyBackImage);

    mSpartyBootImage = make_shared<wxImage>(SpartyBootImage, wxBITMAP_TYPE_PNG);
    mSpartyBootBitmap = make_shared<wxBitmap>(*mSpartyBootImage);

    mSpartyFrontImage = make_shared<wxImage>(SpartyFrontImage, wxBITMAP_TYPE_PNG);
    mSpartyFrontBitmap = make_shared<wxBitmap>(*mSpartyFrontImage);
}

/**
 * Loads the Sparty Game Object from the xml node
 * @param node the node that has the Sparty Information
 */
void Sparty::XmlLoad(wxXmlNode* node)
{
    if (node == nullptr)
    {
        return;
    }
    Item::XmlLoad(node);

    node->GetAttribute(L"height", L"100").ToDouble(&mHeight);
    node->GetAttribute(L"kick-duration", L"10").ToDouble(&mKickDuration);
    node->GetAttribute(L"kick-speed", L"1").ToDouble(&mKickSpeed);

    mWidth = mHeight * (double)mSpartyBackImage->GetWidth() / (double)mSpartyBackImage->GetHeight();

    wxString pinCoordinates = node->GetAttribute(L"pin", L"0, 0");
    double pinX = 0, pinY = 0;
    bool validPinParse = (wxSscanf(pinCoordinates, L"%lf, %lf", &pinX, &pinY) == 2);
    mPinLocation = validPinParse ? wxPoint(pinX, pinY) : wxPoint(0, 0);

    const double verticalOffset = mHeight/2;
    const double bootTipLocation = mHeight * SpartyBootPercentage;
    mYPositionOfKick = GetY() - verticalOffset + bootTipLocation;

    mInput = std::make_shared<InputPin>(nullptr, mPinLocation);
    mInput -> SetState(States::Unknown);
}

std::shared_ptr<IDraggable> Sparty::GrabProductForKicking()
{
    if (GetGame() == nullptr)
    {
        return nullptr;
    }

    double sensorDetectionX = GetX() - ProductKickPointX;

    if (sensorDetectionX < 0 || mYPositionOfKick < 0)
    {
        return nullptr;
    }

    auto detectedProduct = GetGame()->HitTest(sensorDetectionX, mYPositionOfKick);

    if (detectedProduct == nullptr)
    {
        return nullptr;
    }

    return detectedProduct;
}


/**
 * Draws Sparty
 * @param graphics The graphics context that sparty's on
 */
void Sparty::Draw(std::shared_ptr<wxGraphicsContext> graphics)
{
    if (!graphics) return;

    DrawWire(graphics);
    graphics->PushState();
    graphics->Translate(GetX(), GetY());

    if (mSpartyBackBitmap->IsOk())
        graphics->DrawBitmap(*mSpartyBackBitmap, -mWidth/2, -mHeight/2, mWidth, mHeight);

    if (mSpartyFrontBitmap->IsOk())
        graphics->DrawBitmap(*mSpartyFrontBitmap, -mWidth/2, -mHeight/2, mWidth, mHeight);

    if (mSpartyBootBitmap->IsOk())
    {
        graphics->Rotate(mKickAngle);
        graphics->DrawBitmap(*mSpartyBootBitmap, -mWidth/2, -mHeight/2, mWidth, mHeight);
    }

    graphics->PopState();

    mInput->Draw(graphics);
    //graphics->PopState();
}

/**
 * Test on Click function
 * (MAKE SURE TO MAKE THE CONSTRUCTOR USE SpartyFrontImage TO TEST)
 * @param x don't matter
 * @param y don't matter
 */
void Sparty::OnClick(double x, double y)
{
    //Kick();
}

/**
 * Will be called by the pin attached to Sparty
 */
void Sparty::StartKickAnimation()
{
    mKickState = true;
    mKickAngle = 0;
}

/**
 * Calls most frames (Most of the kick animation is in here)
 * @param elapsed Time elapsed since last update in seconds
 */
void Sparty::Update(double elapsed)
{

    if (mInput->GetState() == States::One && mLastState != States::One)
    {
        StartKickAnimation();
    }

    if (mKickState)
    {
        UpdateKickAngle(elapsed);
    }
    mLastState = mInput->GetState();
}

void Sparty::UpdateKickAngle(double elapsed)
{
    mKickAngle += (elapsed * mChangedDirection) * 1.8 / mKickDuration;

    if (mKickAngle >= SpartyBootMaxRotation)
    {
        mChangedDirection = -1;
        mKickAngle = SpartyBootMaxRotation;
    }
    else if (mKickAngle <= 0)
    {
        EndKickAnimation();
    }

    if (mKickAngle >= SpartyKickPoint * SpartyBootMaxRotation * 2)
    {
        auto productToKick = GrabProductForKicking();
        if (productToKick != nullptr && productToKick->IsGrabbable())
        {
            productToKick->Kick(mKickSpeed);
        }
    }
}


void Sparty::EndKickAnimation()
{
    mKickState = false;
    mKickAngle = 0;
    mChangedDirection = 1;
}

/**
 * Tests if an output pin has an input pin to connect to
 * @param pin The pin that is connected
 * @param lineEnd The spot of the mouse
 * @return True if there is a pin to be connected to
 */
bool Sparty::Connect(OutputPin *pin, wxPoint lineEnd)
{
    return mInput->Catch(pin, lineEnd);
}

void Sparty::DrawWire(std::shared_ptr<wxGraphicsContext> graphics)
{
    if (!mInput)
    {
        return;
    }

    wxColour connectionColor;
    if (mInput->GetState() == States::Zero)
    {
        connectionColor = ConnectionColorZero;
    }
    else if (mInput->GetState() == States::One)
    {
        connectionColor = ConnectionColorOne;
    }
    else
    {
        connectionColor = ConnectionColorUnknown;
    }

    wxPen wirePen(connectionColor, 3);  // Thickness 3 for better visibility
    graphics->SetPen(wirePen);

    auto spartyPin = mInput->GetAbsoluteLocation();
    int spartyX = GetX();
    int spartyY = GetY();

    wxPoint rightPinStart = wxPoint(spartyPin.x + WirePinOffset, spartyPin.y);
    wxPoint upwardPoint = wxPoint(rightPinStart.x, spartyY - WireUpwardPointY);
    wxPoint leftwardPoint = wxPoint(spartyX + WireLeftwardPointX, upwardPoint.y);
    wxPoint downwardPoint = wxPoint(leftwardPoint.x, spartyY);
    wxPoint leftwardPoint2 = wxPoint(downwardPoint.x - WireOffsetSparty, downwardPoint.y);

    graphics->StrokeLine(rightPinStart.x, rightPinStart.y, upwardPoint.x, upwardPoint.y);
    graphics->StrokeLine(upwardPoint.x, upwardPoint.y, leftwardPoint.x, leftwardPoint.y);
    graphics->StrokeLine(leftwardPoint.x, leftwardPoint.y, downwardPoint.x, downwardPoint.y);
    graphics->StrokeLine(downwardPoint.x, downwardPoint.y, leftwardPoint2.x, leftwardPoint2.y);

}