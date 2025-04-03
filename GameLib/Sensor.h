/**
 * @file Sensor.h
 * @author Silas Hodges
 *
 * Class representing a Sensor in the game.
 */
 
#ifndef SENSOR_H
#define SENSOR_H

#include "Item.h"
#include "OutputPin.h"
#include "SensorPanel.h"
#include <memory>
#include <vector>
#include <string>

#include "Product.h"

/**
 * Class representing a Sensor in the game.
 */
class Sensor : public Item
{
private:
    /// Image for the sensor camera
    std::shared_ptr<wxImage> mSensorCameraImage;
	/// Bitmap for the sensor camera
	std::shared_ptr<wxBitmap> mSensorCameraBitmap;

	/// Image for the sensor cable
	std::shared_ptr<wxImage> mSensorCableImage;
	/// Bitmap for the sensor cable
	std::shared_ptr<wxBitmap> mSensorCableBitmap;

	/// List of properties that can be detected
	std::vector<std::wstring> mProperties;

	/// Vertical offset for properties panel relative to sensor
	static const double PanelOffsetY;

	/// Size of each property box in properties panel
	static const wxSize PropertySize;

	/// Background color for properties panel
	static const wxColour PanelBackgroundColor;

	/// Collection of SensorPanels for each property
	std::vector<std::shared_ptr<SensorPanel>> mSensorPanels;

	/// Range where a product is viewed
	static const int SensorRange[2];

	/// Range x-axis where a product is viewed
	static const int SensorRangeX[2];

public:
	Sensor(Game* game);

	void Draw(std::shared_ptr<wxGraphicsContext> graphics) override;
	void XmlLoad(wxXmlNode* node) override;
	//void DrawProperty(std::shared_ptr<wxGraphicsContext> graphics, const std::wstring& property, double x, double y);
	void OnClick(double x, double y) override;

    /**
     * Updates pins based on product properties.
     * @param detectedProperties vector of properties of detected products.
     */
    virtual void UpdatePins(const std::vector<Product::Properties>& detectedProperties);

	/**
     * Accept a visitor
     * @param visitor The visitor we accept
     */
    void Accept(ItemVisitor* visitor) override { visitor->VisitSensor(this); }

	std::shared_ptr<IDraggable> HitDraggable(int x, int y) override;
	void Update(double elapsed) override;

    /**
     * Determines if product is in range of sensor.
     * @param product pointer to product
     * @return True if product in range, false otherwise.
     */
    bool IsProductInRange(Product* product);
};



#endif //SENSOR_H
