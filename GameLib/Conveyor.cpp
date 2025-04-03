/**
 * @file Conveyor.cpp
 *
 * @author Karan Choudhary
 *
 * Implements objects of type Conveyor.
 */

#include "pch.h"
#include "Conveyor.h"
#include "Game.h"
#include "Beam.h"
#include "ItemVisitor.h"
#include "ProductVisitors.h"
#include <wx/tokenzr.h>


/// Image for the background (base) of the conveyor
const std::wstring ConveyorBackgroundImage = L"images/conveyor-back.png";

/// Image for the conveyor belt
const std::wstring ConveyorBeltImage = L"images/conveyor-belt.png";

/// Image for the conveyor control panel when stopped.
const std::wstring ConveyorPanelStoppedImage = L"images/conveyor-switch-stop.png";

/// Image for the conveyor control panel when started.
const std::wstring ConveyorPanelStartedImage = L"images/conveyor-switch-start.png";

/// Speed of conveyor
const int ConveyorSpeed = 100;

/**
 * @return wxRect rect to represent the start button.
 * Location of start button is relative to the panel.
 */
const wxRect StartButtonRect(35, 29, 95, 36);

/**
 * @return wxRect rect to represent the stop button.
 * Location of stop button is relative to the panel.
 */
const wxRect StopButtonRect(35, 87, 95, 36);

/// Offset used to place products above the beam
int beamOffset = 75;

/**
 * Visitor to find and store reference to beam object.
 *
 */
class BeamFinderVisitor : public ItemVisitor
{
private:
    /// Pointer to beam object.
    Beam* mBeam = nullptr;
public:
    /**
     * Visits beam object and stores reference.
     * @param beam Pointer to beam object being visited.
     */
    void VisitBeam(Beam* beam) override
    {
        mBeam = beam;
    }

    /**
     * Gets the stored beam object.
     * @return Pointer of stored beam.
     */
    Beam* GetBeam()const { return mBeam;}
};

/**
 * Constructor
 * @param game Pointer to conveyor
 */
Conveyor::Conveyor(Game* game) : Item(game, ConveyorBackgroundImage)
{

    mBackgroundImage = std::make_unique<wxImage>(ConveyorBackgroundImage, wxBITMAP_TYPE_ANY);
    mBackgroundBitmap = std::make_unique<wxBitmap>(*mBackgroundImage);

    mBeltImage = std::make_unique<wxImage>(ConveyorBeltImage, wxBITMAP_TYPE_ANY);
    mBeltBitmap = std::make_unique<wxBitmap>(*mBeltImage);

    mPanelStoppedImage = std::make_unique<wxImage>(ConveyorPanelStoppedImage, wxBITMAP_TYPE_ANY);
    mPanelStoppedBitmap = std::make_unique<wxBitmap>(*mPanelStoppedImage);

    mPanelStartedImage = std::make_unique<wxImage>(ConveyorPanelStartedImage, wxBITMAP_TYPE_ANY);
    mPanelStartedBitmap = std::make_unique<wxBitmap>(*mPanelStartedImage);

    mBeltSpeed = ConveyorSpeed;
    mBeltPosition = 0;
    mIsRunning = false;
}

/**
 * Renders the conveyor
 * @param graphics The graphics for rendering
 */
void Conveyor::Draw(std::shared_ptr<wxGraphicsContext> graphics)
{
    if (!graphics) return;

    graphics->PushState();
    graphics->Translate(GetX(), GetY());


    if (mBackgroundBitmap->IsOk())
    {
        graphics->DrawBitmap(*mBackgroundBitmap, -mWidth/2, -mHeight/2, mWidth, mHeight);
    }


    if (mBeltBitmap->IsOk())
    {
        double beltHeight = mHeight * ((double)mBeltImage->GetHeight() / (double)mBackgroundImage->GetHeight());
        graphics->DrawBitmap(*mBeltBitmap, -mWidth/2, -beltHeight/2 + mBeltPosition, mWidth, beltHeight);
        graphics->DrawBitmap(*mBeltBitmap, -mWidth/2, -beltHeight/2 + mBeltPosition - beltHeight, mWidth, beltHeight);
    }


    wxBitmap* panelBitmap = mIsRunning ? mPanelStartedBitmap.get() : mPanelStoppedBitmap.get();
    if (panelBitmap->IsOk())
    {
        graphics->DrawBitmap(*panelBitmap, mPanelLocation.x, mPanelLocation.y,
                             panelBitmap->GetWidth(), panelBitmap->GetHeight());
    }

    graphics->PopState();

}

/**
 * Checks if mouse click hit with conveyor or nott
 * @param x The x-coordinate of the mouse click
 * @param y The y-coordinate of the mouse click
 * @return true if both are intersecting
 */
bool Conveyor::HitTest(double x, double y)
{
    double testX = x - GetX();
    double testY = y - GetY();
    if (testX >= -mWidth/2 && testX <= mWidth/2 && testY >= -mHeight/2 && testY <= mHeight/2)
        return true;
    testX -= mPanelLocation.x;
    testY -= mPanelLocation.y;
    if (testX >= 0 && testX <= mPanelStartedImage->GetWidth() &&
        testY >= 0 && testY <= mPanelStartedImage->GetHeight())
        return true;

    return false;
}

/**
 * Starts the conveyor to move itself
 */
void Conveyor::Start()
{
    mIsRunning = true;
    ResetProducts();

    BeamFinderVisitor beamFinder;
    GetGame()->Accept(&beamFinder);
    Beam* beam = beamFinder.GetBeam();
    if (beam)
    {
        beam->ResetCount();
    }
}

/**
 * Starts the conveyor so it doesn't move
 */
void Conveyor::Stop()
{
    mIsRunning = false;
}

/**
 * Resets the products to its deafult state being non-movng
 */
void Conveyor::ResetProducts()
{
    ResetProduct visitor = ResetProduct();
    GetGame()->Accept(&visitor);
    GetGame()->GetScore()->ResetLevelScore();
}

/**
 * Updates the conveyor for products etc.
 * @param elapsed The time that has passed since the previous update
 */
void Conveyor::Update(double elapsed)
{
    if (!mIsRunning)
        return;

    if (elapsed <= 0)
        return;

    if (mBeltSpeed <= 0)
        return;

    mBeltPosition += mBeltSpeed * elapsed;
    if (mBeltPosition > mHeight)
    {
        mBeltPosition -= mHeight;
    }

    MoveProduct visitor = MoveProduct(0, mBeltSpeed * elapsed);
    GetGame()->Accept(&visitor);

}

/**
 * Processes a mouse click
 * @param x The x-coordinate of the click
 * @param y The y-coordinate of the click
 */
void Conveyor::OnClick(double x, double y)
{
    double testX = x - GetX() - mPanelLocation.x;
    double testY = y - GetY() - mPanelLocation.y;

    if (StartButtonRect.Contains(testX, testY))
        Start();
    else if (StopButtonRect.Contains(testX, testY))
        Stop();
}

/**
 * Initializes conveyor from xml
 * @param node The xml node that has conveyor in it
 */
void Conveyor::XmlLoad(wxXmlNode* node)
{
    // Load basic conveyor attributes
    Item::XmlLoad(node);
    node->GetAttribute(L"speed", L"100").ToDouble(&mBeltSpeed);
    node->GetAttribute(L"height", L"800").ToDouble(&mHeight);

    // Get panel location
    wxStringTokenizer tokenizer(node->GetAttribute(L"panel"), L",");
    double panelX = wxAtof(tokenizer.GetNextToken());
    double panelY = wxAtof(tokenizer.GetNextToken());
    mPanelLocation = wxPoint(panelX, panelY);

    // Get width and height with aspect ratio
    mWidth = mHeight * (double)mBackgroundImage->GetWidth() / (double)mBackgroundImage->GetHeight();

    // Inital placement of first product above the beam
    double conveyorCenterX = GetX();
    double conveyorCenterY = GetY();

    // Initialize the y we are subtracting from as center of conveyor
    double currentY = conveyorCenterY;

    for (auto child = node->GetChildren(); child; child = child->GetNext())
    {
        if (child->GetName() == L"product")
        {
            auto product = std::make_shared<Product>(GetGame());
            product->XmlLoad(child);
            GetGame()->Add(product);

            // Process placement string for double without +
            wxString placementString = child->GetAttribute(L"placement", L"0");
            placementString.Replace("+", "");

            double placement = 0;
            placementString.ToDouble(&placement);

            // Check if placement starts with a +, if so accumulate to y position
            if (child->GetAttribute(L"placement").StartsWith("+"))
            {
                currentY -= placement;
            }
            else
            {
                currentY = conveyorCenterY - placement;
            }

            double productX = GetX();
            product->SetInitalPosition(productX, currentY);
            product->SetLocation(productX, currentY);
            mNumberOfProductsOnConveyor++;
        }
    }
    GetGame()->SetNumProducts(mNumberOfProductsOnConveyor);
}

/**
 * Get the speed
 * @return Speed of the conveyor
 */
double Conveyor::GetSpeed() const
{
    return mBeltSpeed;
}

/**
 * See is conveyor is running
 * @return true if running else false
 */
bool Conveyor::IsRunning() const
{
    return mIsRunning;
}

void Conveyor::Accept(ItemVisitor* visitor)
{
    visitor->VisitConveyor(this);
}