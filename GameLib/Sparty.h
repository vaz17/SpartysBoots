/**
 * @file Sparty.h
 * @author matthew vazquez
 *
 *
 */

#ifndef SPARTY_H
#define SPARTY_H

#include "Item.h"

/**
 * Objects of this class represent Sparty in the game.
 * Sparty class objects are responsible for kicking products on the conveyor belt.
 */
class Sparty : public Item
{
private:
    /// the speed the products are kicked off the conveyor belt in pixels per second
    double mKickSpeed;
    /// how long it takes to complete a kick in seconds
    double mKickDuration;

    std::shared_ptr<wxImage> mSpartyBackImage; ///< Image of Sparty backward.
    std::shared_ptr<wxBitmap> mSpartyBackBitmap; ///< Bitmap for backward Sparty.
    std::shared_ptr<wxImage> mSpartyBootImage; ///<  Image for Sparty's boot.
    std::shared_ptr<wxBitmap> mSpartyBootBitmap; ///< Bitmap for Sparty's boot.
    std::shared_ptr<wxImage> mSpartyFrontImage; ///< Image of Spary forward.
    std::shared_ptr<wxBitmap> mSpartyFrontBitmap; ///< Bitmap for Sparty forward.

    /// Height of Sparty.
    double mHeight;
    /// Width of Sparty.
    double mWidth;

    /// Location of Sparty's pin.
    wxPoint mPinLocation;

    /// if Sparty is currently kicking
    bool mKickState = false;

    /// Records the last state of Sparty.
    States mLastState;

    /// Angle of sparty's boots
    double mKickAngle = 0;

    /// Shows if sparty's boots changed direction (positive is forward, and negative is backward)
    int mChangedDirection = 1;

    /// Tracks rotation angle of Sparty's boots.
    double mBootRotation;

    /// Previous state of Sparty's pin.
    bool mPinStateFromEarlier = false;

    /// Y coordinate of boot during Sparty's kick.
    double mYPositionOfKick;

    /// Pointer to Sparty's input pin.
    std::shared_ptr<InputPin> mInput;

public:
    Sparty(Game* game);
    void XmlLoad(wxXmlNode* node) override;
    void Draw(std::shared_ptr<wxGraphicsContext> graphics) override;
    void OnClick(double x, double y) override;
    void Update(double elapsed) override;

    /**
     * Grab product to kick.
     * @return shared pointer of draggable object (product).
     */
    std::shared_ptr<IDraggable> GrabProductForKicking();

    /**
     * Accept a visitor
     * @param visitor The visitor we accept
     */
    void Accept(ItemVisitor* visitor) override { visitor->VisitSparty(this); }

    bool Connect(OutputPin *pin, wxPoint lineEnd) override;

    /**
     * Draws wire connected to Sparty.
     * @param graphics graphics context to draw on
     */
    void DrawWire(std::shared_ptr<wxGraphicsContext> graphics);

    /**
     * Starts kick animation.
     */
    void StartKickAnimation();

    /**
     * Updates kick angle for animation.
     * @param elapsed time since last update
     */
    void UpdateKickAngle(double elapsed);

    /**
     * Ends kick animation.
     */
    void EndKickAnimation();

};



#endif //SPARTY_H
