/**
 * @file Beam.h
 * @author Silas Hodges
 *
 * Class representing a beam in the game
 */
 
#ifndef BEAM_H
#define BEAM_H

#include "Item.h"
#include "OutputPin.h"
#include <memory>

/**
 * Class representing a beam in the game
 */
class Beam : public Item
{
private:
	/// Offset for the sender relative to receiver
	double mSenderOffset = 0.0;

	/// Image for the green beam
	std::shared_ptr<wxImage> mBeamImageGreen;

	/// Bitmap for the green beam
	std::shared_ptr<wxBitmap> mBeamBitmapGreen;

	/// Image for the red beam
	std::shared_ptr<wxImage> mBeamImageRed;

	/// Bitmap for the red beam
	std::shared_ptr<wxBitmap> mBeamBitmapRed;

	/// Indicates whether the beam is broken
	bool mBeamBroken = false;

	/// Tracks transition states for the beam.
	bool mWasBroken = false;

	/// Tracks number of products passed through the beam
	int mNumBroken = 0;

	/// Track the last product passed through the beam
	Product* mLastProduct;

	/// Pin representing the beam's state
	std::shared_ptr<OutputPin> mBeamPin;

public:
	Beam(Game* game);

	void Draw(std::shared_ptr<wxGraphicsContext> graphics) override;

	void XmlLoad(wxXmlNode* node) override;

	void OnClick(double x, double y) override;

	/**
     * Accept a visitor
     * @param visitor The visitor we accept
     */
    void Accept(ItemVisitor* visitor) override { visitor->VisitBeam(this); }

	void Update(double elapsed) override;

	/**
	 * Gets the sender offset value
	 * @return The sender offset
	 */
	double GetSenderOffset() const { return mSenderOffset; }

	/**
	 * Getter for beam's broken status
	 * @return True if beam is broken, false if not
	 */
	bool GetBeamBroken() const { return mBeamBroken; }

	/**
	 * Getter for beam's output pin
	 * @return Shared pointer to the OutputPin
	 */
	std::shared_ptr<OutputPin> GetOutputPin() const { return mBeamPin; }

	std::shared_ptr<IDraggable> HitDraggable(int x, int y) override;

	void ResetCount();
};



#endif //BEAM_H
