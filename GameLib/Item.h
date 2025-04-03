/**
 * @file Item.h
 * @author karan choudhary
 *
 * Base class for any item in our game
 */

#ifndef ITEM_H
#define ITEM_H

#include <string>
#include <memory>
#include <wx/graphics.h>
#include <wx/xml/xml.h>

#include "IDraggable.h"
#include "ItemVisitor.h"
#include "OutputPin.h"

class Game;

/**
 * Base class for any item in our game.
 */
class Item : public IDraggable{
private:
    /// The game this item is contained in
    Game *mGame;

    // Item location in the game
    double mX = 0;     ///< X location for the center of the item
    double mY = 0;     ///< Y location for the center of the item

    /// The underlying item image
    std::unique_ptr<wxImage> mItemImage;

    /// The bitmap we can display for this item
    std::unique_ptr<wxBitmap> mItemBitmap;

protected:
    /**
    * Constructor
    * @param game The game this item is a member of
    */

    Item (Game *game) : mGame(game)
    {
    };
    Item(Game *game, const std::wstring &filename);

public:
    virtual ~Item();

    /**
     * Default constructor (disabled)
     */
    Item() = delete;

    /**
     * Copy constructor (disabled)
     */
    Item(const Item &) = delete;

    /**
     * Assignment operator
     * @param other The other item we are assigning from
     * @return A reference to this object
     */
    virtual Item& operator=(const Item &other) = delete;
    virtual void Draw(std::shared_ptr<wxGraphicsContext> graphics);
    virtual bool HitTest(double x, double y);

    /**
     * Get the X location of the item
     * @return X location in pixels
     */
    virtual double GetX() const { return mX; }

    /**
     * Get the Y location of the item
     * @return Y location in pixels
     */
    virtual double GetY() const { return mY; }

    /**
     * Set the item location
     * @param x X location in pixels
     * @param y Y location in pixels
     */
     void SetLocation(double x, double y) override { mX = x; mY = y; }

    /**
     * Accept a visitor
      * @param visitor The visitor we accept
      */
    virtual void Accept(ItemVisitor* visitor) = 0;

    virtual void XmlLoad(wxXmlNode* node);

    /**
     * Handle updates for animation
     * @param elapsed The time since the last update
     */
    virtual void Update(double elapsed) {}

    /**
     * Get the pointer to the Game object
     * @return Pointer to Game object
     */
    Game* GetGame() const { return mGame; }

    /**
     * Handle a mouse click on this item
     * @param x X location relative to the game
     * @param y Y location relative to the game
     */
    virtual void OnLeftDown(double x, double y) {}

    /**
     * Handle a mouse up on this item
     * @param x X location relative to the game
     * @param y Y location relative to the game
     */
    virtual void OnLeftUp(double x, double y) {}

    /**
     * Handle a mouse move event
     * @param x X location relative to the game
     * @param y Y location relative to the game
     */
    virtual void OnMouseMove(double x, double y) {}
    /**
     * Check if this item is a pin.
     * @return True if the item is a pin, otherwise false.
     */
    virtual bool IsPin() const { return false; }

    /**
     * Check if item was clicked at location
     * @param x X location relative to the game
     * @param y Y location relative to the game
     */
    void OnClick(double x, double y) override {};

    /**
     * Tests if an item can be grabbed
     * @return false if not grabbable and true if it is
     */
    bool IsGrabbable() override { return false; };

    /**
    * Test to see if we clicked on some draggable inside the item.
    * @param x X location clicked on
    * @param y Y location clicked on
    * @return Whatever we clicked on or NULL if none
    */
    virtual std::shared_ptr<IDraggable> HitDraggable(int x, int y) {return nullptr;}

    /**
     * Releases state or held item.
     */
    void Release() override {};

    /**
     * Functions attempts to connect output pin to specified endpoint.
     * @param pin pointer to output pin.
     * @param lineEnd endpoint of connection wire.
     * @return True if successful, false otherwise.
     */
    virtual bool Connect(OutputPin *pin, wxPoint lineEnd) { return false;}

    /**
     * Kicks item with given speed
     * @param kick speed of kick
     */
    void Kick(double kick) override {};

};

#endif //PROJECT1_ITEM_H