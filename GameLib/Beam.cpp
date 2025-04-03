/**
 * @file Beam.cpp
 * @author Silas Hodges
 *
 * Class representing a beam in the game.
 */

#include "pch.h"
#include "Beam.h"
#include "Game.h"
#include "ItemVisitor.h"
#include "Product.h"
#include "Sensor.h"
#include <unordered_map>
#include "Gates.h"

/// Image for the beam sender and receiver when red
const std::wstring BeamRedImage = L"beam-red.png";

/// Image for the beam sender and receiver when green
const std::wstring BeamGreenImage = L"beam-green.png";

/// X offset for the beam pin in pixels
/// This is larger than normal to get it past Sparty's feet
const int BeamPinOffset = 80;

using namespace std;

/**
 * Visitor for detecting collisions with products
 */
class CollisionVisitor : public ItemVisitor
{
private:
	/// Pointer to the beam instance
	Beam* mBeam;

	/// False indicating if the beam is broken
	bool mBeamBroken;

	/// Pointer to the product that collides
	Product* mCollidingProduct;

	/**
	 * Checks if a product collides with the beam
	 *
	 * @param product Pointer to the product
	 * @return True if collision occurs, false if not
	 */
	bool CheckCollisionWithProduct(Product* product)
	{
		double beamXStart = mBeam->GetX() + mBeam->GetSenderOffset();
		double beamXEnd = mBeam->GetX();
		double beamY = mBeam->GetY();

		double rectX = min(beamXStart, beamXEnd);
		double rectWidth = abs(beamXEnd - beamXStart);
		double rectY = beamY - 1;
		double rectHeight = 2.0;

		wxRect2DDouble beamRect(rectX, rectY, rectWidth, rectHeight);

		double productSize = product->GetSize();
		double productX = product->GetX() - productSize / 2;
		double productY = product->GetY() - productSize / 2;
		wxRect2DDouble productRect(productX, productY, productSize, productSize);

		return beamRect.Intersects(productRect);
	}

public:
	/**
	 * Constructs a CollisionVisitor
	 * @param beam Pointer to the Beam instance
	 */
	CollisionVisitor(Beam* beam) : mBeam(beam), mBeamBroken(false), mCollidingProduct(nullptr) {}

	/**
	 * Visits a product to check for collision
	 * @param product Pointer to the product
	 */
	void VisitProduct(Product* product) override
	{
		if (CheckCollisionWithProduct(product))
		{
			mBeamBroken = true;
			mCollidingProduct = product;
		}
	}

	/**
	 * Checks if the beam is broken
	 * @return True if beam broken, false if not
	 */
	bool IsBeamBroken() const { return mBeamBroken; }

	/**
	 * Getter for the colliding product
	 * @return Product that is colliding
	 */
	Product* GetCollidingProduct() const { return mCollidingProduct; }
};

/**
 * Visitor that searches for a sensor object.
 * Used to find and store a sensor reference.
 */
class SensorFinderVisitor : public ItemVisitor
{
private:
	/// Pointer to sensor object
	Sensor* mSensor = nullptr;

public:
	/**
	 * Visits sensor object to store its reference.
	 * @param sensor pointer to visited sensor object.
	 */
	void VisitSensor(Sensor* sensor) override
	{
		mSensor = sensor;
	}

	/**
	 * Gets sensor pointer.
	 * @return pointer to stored sensor object.
	 */
	Sensor* GetSensor() const { return mSensor; }
};

/**
 * Associates product properties with string representations.
 * Map used by 'ProportyToString' to convert product properties to strings.
 */
static const unordered_map<Product::Properties, wstring> PropertyToStringMap = {
	{Product::Properties::None, L"None"},
	{Product::Properties::Red, L"Red"},
	{Product::Properties::Green, L"Green"},
	{Product::Properties::Blue, L"Blue"},
	{Product::Properties::White, L"White"},
	{Product::Properties::Square, L"Square"},
	{Product::Properties::Circle, L"Circle"},
	{Product::Properties::Diamond, L"Diamond"},
	{Product::Properties::Izzo, L"Izzo"},
	{Product::Properties::Smith, L"Smith"},
	{Product::Properties::Football, L"Football"},
	{Product::Properties::Basketball, L"Basketball"}
};

/**
 * Searches PropertyToStringMap for input string.
 * If property is not found, returns "Unknown."
 * @param property product property being converted.
 * @return wstring representing found property.
 */
wstring PropertyToString(Product::Properties property)
{
	auto it = PropertyToStringMap.find(property);
	if (it != PropertyToStringMap.end())
	{
		return it->second;
	}
	else
	{
		return L"Unknown";
	}
}

/**
 * Constructs a Beam object and loads images
 * @param game Pointer to the game instance
 */
Beam::Beam(Game* game): Item(game)
{
    // Load images
	mBeamImageGreen = make_shared<wxImage>(L"images/beam-green.png", wxBITMAP_TYPE_PNG);
	mBeamBitmapGreen = make_shared<wxBitmap>(*mBeamImageGreen);

	mBeamImageRed = make_shared<wxImage>(L"images/beam-red.png", wxBITMAP_TYPE_PNG);
	mBeamBitmapRed = make_shared<wxBitmap>(*mBeamImageRed);


	wxPoint point(GetX(), GetY());

	mBeamPin = make_shared<OutputPin>(this, point);
	mBeamPin->ChangeDefaultLineLength(BeamPinOffset);
}

/**
 * Draws the beam with appropriate color based on its state
 * @param graphics the graphics context to draw on
 */
void Beam::Draw(shared_ptr<wxGraphicsContext> graphics)
{
	// Choose image based on beam state
	auto beamBitmap = mBeamBroken ? mBeamBitmapRed : mBeamBitmapGreen;

	// Draw beam line between sender and receiver
	wxPen laser1(wxColour(255, 200, 200, 100), 8);
	wxPen laser2(wxColour(255, 0, 0, 175), 4);

	// First pass thick pen
	graphics->SetPen(laser1);
	graphics->StrokeLine(GetX() + mSenderOffset, GetY(), GetX(), GetY());

	// Second pass thin pen
	graphics->SetPen(laser2);
	graphics->StrokeLine(GetX() + mSenderOffset, GetY(), GetX(), GetY());

	double width = beamBitmap->GetWidth();
	double height = beamBitmap->GetHeight();

	double receiverX, receiverY;
	double pinX, pinY;

	// Determine if the sender is on the left or right
	if (mSenderOffset < 0)
	{
		// Sender is on the left

		mBeamPin->Draw(graphics);

		// Draw sender image without flipping
		graphics->DrawBitmap(*beamBitmap, GetX() + mSenderOffset - width / 2, GetY() - height / 2, width, height);

		// Draw receiver image flipped horizontally
		graphics->PushState();
		graphics->Translate(GetX(), GetY());
		graphics->Scale(-1, 1);
		graphics->DrawBitmap(*beamBitmap, -width / 2, -height / 2, width, height);
		graphics->PopState();
	}
	else
	{
		// Sender is on the right
		// Draw sender image flipped horizontally
		graphics->PushState();
		graphics->Translate(GetX() + mSenderOffset, GetY());
		graphics->Scale(-1, 1);
		graphics->DrawBitmap(*beamBitmap, -width / 2, -height / 2, width, height);
		graphics->PopState();

		// Draw receiver image without flipping
		graphics->DrawBitmap(*beamBitmap, GetX() - width / 2, GetY() - height / 2, width, height);
	}
}

/**
 * Loads beam data from an XML node
 * @param node The XML node containing beam data
 */
void Beam::XmlLoad(wxXmlNode* node)
{
	Item::XmlLoad(node);

	wxString senderStr = node->GetAttribute(L"sender", L"0");
	senderStr.ToDouble(&mSenderOffset);
}

/**
 * Handles mouse click events on beam
 * @param x X-coordinate of click
 * @param y Y-coordinate of click
 */
void Beam::OnClick(double x, double y)
{
	wxLogMessage(L"Beam clicked at (%f, %f)", x, y);
}

/**
 * Updates the beam's state based on collisions with products
 * Updates score of products as they pass through the beam
 * Helps signal game to end.
 * @param elapsed Time elapsed since last update
 */
void Beam::Update(double elapsed)
{
	CollisionVisitor visitor(this);

	GetGame()->Accept(&visitor);

	mBeamBroken = visitor.IsBeamBroken();

	// Set product to check kick status of after beam is broken
	if (mBeamBroken)
	{
		mLastProduct = visitor.GetCollidingProduct();
	}

	// Check if product was kicked after beam is repaired
	if (mWasBroken && !mBeamBroken)
	{
		if (mLastProduct && !(mLastProduct->GetWasKicked()))
		{
			// If product was supposed to be kicked, but passed through beam, give bad score.
			if (mLastProduct->GetKick())
			{
				GetGame()->GetScore()->SetLevelScoreBad();
			}
			// If product passed through beam but shouldn't have been kicked, award good score.
			else
			{
				GetGame()->GetScore()->SetLevelScoreGood();
			}
			mLastProduct->SetScoreUpdated(); // Set score as updated so it doesn't happen more than once.
		}
		mLastProduct = nullptr;

		if (mNumBroken == GetGame()->GetNumProducts())
		{
			GetGame()->EndGame();
		}
	}

	if (!mWasBroken && mBeamBroken)
	{
		mNumBroken++;
	}

	if (mBeamBroken)
	{
		mBeamPin->SetState(States::One);
	} else
	{
		mBeamPin->SetState(States::Zero);
	}

	mWasBroken = mBeamBroken;
	mBeamPin->Update();
}

/**
 * Test to see if we clicked on some draggable inside the item.
 * @param x X location clicked on
 * @param y Y location clicked on
 * @return Whatever we clicked on or NULL if none
 */
std::shared_ptr<IDraggable> Beam::HitDraggable(int x, int y)
{
	if(mBeamPin->HitTest(x, y))
	{
		return mBeamPin;
	}

	return nullptr;
}

/**
 * Method that resets the product count
 */
void Beam::ResetCount()
{
	mNumBroken = 0;
	mWasBroken = false;
}