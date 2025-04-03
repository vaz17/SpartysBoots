/**
 * @file Sensor.cpp
 * @author Silas Hodges
 *
 * Class representing a sensor in the game
 */

#include "pch.h"
#include "Sensor.h"
#include "Game.h"
#include "Gates.h"

using namespace std;

// Static constants defined
const double Sensor::PanelOffsetY = 87.0;
const wxSize Sensor::PropertySize = wxSize(100, 40);
const wxColour Sensor::PanelBackgroundColor = wxColour(128, 128, 128);

/// Path to the sensor camera image
const wstring SensorCameraImagePath = L"images/sensor-camera.png";

/// Path to the sensor cable image
const wstring SensorCableImagePath = L"images/sensor-cable.png";

/// Sensor range
const int Sensor::SensorRange[2] = {-40, 15};

/// Sensor Range x axis
const int Sensor::SensorRangeX[2] = {-10, 110};

/**
 * Visitor class to detect collisions between sensor and products
 *
 * Objects of this class visit products and check if they are in sensor range.
 */
class SensorCollisionVisitor : public ItemVisitor
{
private:
	/// Pointer to sensor that detects products.
	Sensor* mSensor;
	/// Properties of detected products.
	vector<Product::Properties> mDetectedProperties;

public:
	/**
	 * Collision constructor for a given sensor.
	 * @param sensor pointer to sensor for detection.
	 */
	SensorCollisionVisitor(Sensor* sensor) : mSensor(sensor) {}

	/**
	 * Visits products and checks if they are within sensor's range.
	 * @param product pointer to visited product.
	 */
	void VisitProduct(Product* product) override
	{
		if (mSensor->IsProductInRange(product))
		{
			const auto& properties = product->GetProperties();
			mDetectedProperties.insert(mDetectedProperties.end(), properties.begin(), properties.end());
		}
	}

	/**
	 * Gets list of properties of detected products.
	 * @return reference to properties of detected products.
	 */
	const vector<Product::Properties>& GetDetectedProperties() const { return mDetectedProperties;}
};

/**
 * Constructs a Sensor object and loads images
 * @param game Pointer to game instance
 */
Sensor::Sensor(Game* game) : Item(game)
{
    // Load camera image
	mSensorCameraImage = make_shared<wxImage>(SensorCameraImagePath, wxBITMAP_TYPE_PNG);
	mSensorCameraBitmap = make_shared<wxBitmap>(*mSensorCameraImage);

	mSensorCableImage = make_unique<wxImage>(SensorCableImagePath, wxBITMAP_TYPE_PNG);
	mSensorCableBitmap = make_unique<wxBitmap>(*mSensorCableImage);
}

/**
 * Draws sensor components and properties panel
 * @param graphics The graphics context to draw on
 */
void Sensor::Draw(shared_ptr<wxGraphicsContext> graphics)
{
	// Stuff for sensor camera
	double cameraWidth = mSensorCameraBitmap->GetWidth();
	double cameraHeight = mSensorCameraBitmap->GetHeight();

	// Stuff for sensor cable
	double cableWidth = mSensorCableBitmap->GetWidth();
	double cableHeight = mSensorCableBitmap->GetHeight();

	// Draw sensor cable
	graphics->DrawBitmap(*mSensorCableBitmap, GetX() - cableWidth / 2, GetY() - cameraHeight / 2, cableWidth, cableHeight);

	// Draw sensor camera
	graphics->DrawBitmap(*mSensorCameraBitmap, GetX() - cameraWidth / 2, GetY() - cameraHeight / 2, cameraWidth,
						cameraHeight);

	// Draw properties panel
	if (!mProperties.empty())
	{
		// Calculate panel position
		double panelTopY = GetY() + PanelOffsetY;
		double panelHeight = PropertySize.GetHeight() * mProperties.size();
		double panelLeftX = GetX() + cableWidth / 2;

		// Panel background
		graphics->SetBrush(wxBrush(PanelBackgroundColor));
		graphics->SetPen(*wxTRANSPARENT_PEN);
		graphics->DrawRectangle(panelLeftX, panelTopY, PropertySize.GetWidth(), panelHeight);

		// Draw each SensorPanel
		for (auto& panel : mSensorPanels)
		{
			if (panel)
			{
				panel->Draw(graphics);
			}
		}
	}
}

/**
 * Loads sensor properties from an XML node
 * @param node The XML node containing sensor data
 */
void Sensor::XmlLoad(wxXmlNode* node)
{
	Item::XmlLoad(node);

	// Load properties from XML
	wxXmlNode* child = node->GetChildren();
	while (child)
	{
		auto name = child->GetName().ToStdWstring();
		mProperties.push_back(name);
		child = child->GetNext();
	}

	// Initialize pins for each property
	mSensorPanels.resize(mProperties.size());

	// Set positions
	double panelLeftX = GetX() + mSensorCableBitmap->GetWidth() / 2;
	double panelTopY = GetY() + PanelOffsetY;

	for (size_t i = 0; i < mProperties.size(); ++i)
	{
		//double pinX = panelLeftX;
		//double pinY = panelTopY + i * PropertySize.GetHeight() + PropertySize.GetHeight() / 2;

		double panelX = panelLeftX + PropertySize.GetWidth() / 2;
		double panelY = panelTopY + i * PropertySize.GetHeight() + PropertySize.GetHeight() / 2;


		mSensorPanels[i] = make_shared<SensorPanel>(GetGame(), mProperties[i], panelX, panelY);
	}
}

/**
 * Handles mouse click events on sensor
 * @param x X-coordinate of click
 * @param y Y-coordinate of click
 */
void Sensor::OnClick(double x, double y)
{
	wxLogMessage(L"Sensor clicked at %f, %f)", x, y);
}

void Sensor::UpdatePins(const vector<Product::Properties>& detectedProperties)
{
	for (auto& panel : mSensorPanels)
	{
		if (panel)
		{
			panel->UpdateState(detectedProperties);
		}
	}
}

/**
 * Test to see if we clicked on some draggable inside the item.
 * @param x X location clicked on
 * @param y Y location clicked on
 * @return Whatever we clicked on or NULL if none
 */
std::shared_ptr<IDraggable> Sensor::HitDraggable(int x, int y)
{
	for (auto& panel : mSensorPanels)
	{
		if (panel->GetOutputPin()->HitTest(x,y ))
		{
			return panel->GetOutputPin();
		}
	}
	return nullptr;
}

/**
 * Updates sensor by checking for products within the range.
 * It then updates the pins based on properties of detected product.
 * @param elapsed time since last update in seconds
 */
void Sensor::Update(double elapsed)
{
	SensorCollisionVisitor visitor(this);
	GetGame()->Accept(&visitor);
	UpdatePins(visitor.GetDetectedProperties());
}

bool Sensor::IsProductInRange(Product* product)
{
	double sensorX = GetX();
	double sensorY = GetY();

	double productWidth = product->GetSize();
	double productHeight = product->GetSize();

	double productYTop = product->GetY() - productHeight / 2;
	double productYBottom = product->GetY() + productHeight / 2;
	double productXLeft = product->GetX() - productWidth / 2;
	double productXRight = product->GetX() + productWidth / 2;

	double rangeTop = sensorY + SensorRange[0];
	double rangeBottom = sensorY + SensorRange[1];
	double rangeLeft = sensorX + SensorRangeX[0];
	double rangeRight = sensorX + SensorRangeX[1];

	if (productYBottom >= rangeTop && productYTop <= rangeBottom && productXLeft >= rangeLeft && productXRight <= rangeRight)
	{
		return true;
	}

	return false;
}