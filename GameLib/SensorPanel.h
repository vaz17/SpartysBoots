/**
 * @file SensorPanel.h
 * @author Silas Hodges
 *
 * Class representing a single property panel in Sensor
 */
 
#ifndef SENSORPANEL_H
#define SENSORPANEL_H

#include "Item.h"
#include "OutputPin.h"
#include "Product.h"
#include <memory>
#include <string>

/**
 * Class representing a single property panel in Sensor
 */
class SensorPanel : public Item
{
private:
    /// Name of the property
    std::wstring mProperty;

    /// OutputPin associated
    std::shared_ptr<OutputPin> mOutputPin;

	/// X-coordinate of panel
	double mX;

	/// Y-coordinate of panel
	double mY;

	/// Size of each property box in properties panel
	static const wxSize PropertySize;

public:
    SensorPanel(Game* game, const std::wstring& property, double x, double y);

	void Draw(std::shared_ptr<wxGraphicsContext> graphics) override;

	void UpdateState(const std::vector<Product::Properties>& detectedProperties);

	/**
	 * Getter for the OutputPin
	 * @return Shared pointer to OutputPin
	 */
	std::shared_ptr<OutputPin> GetOutputPin() const { return mOutputPin; }

	/**
	 * Getter for property name
	 * @return Name of property
	 */
	std::wstring GetProperty() const { return mProperty; }

	/**
	 * Sets location of SensorPanel
	 * @param x X-coordinate
	 * @param y Y-coordinate
	 */
	void SetLocation(double x, double y) override { mX = x; mY = y; }

	/**
	 * Gets x-coordinate of SensorPanel
	 * @return X-coordinate
	 */
	double GetXCoord() const { return mX; }

	/**
	 * Gets y-coordinate of SensorPanel
	 * @return Y-coordinate
	 */
	double GetYCoord() const { return mY; }

	void OnClick(double x, double y) override;

	/**
	 * Accept a visitor
	 * @param visitor The visitor we accept
	 */
	void Accept(ItemVisitor* visitor) override { visitor->VisitSensorPanel(this); }

};



#endif //SENSORPANEL_H
