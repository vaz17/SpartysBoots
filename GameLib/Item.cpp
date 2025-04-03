/**
 * @file Item.cpp
 * @author karan choudhary
 */

#include "pch.h"
#include "Item.h"
#include "Game.h"

using namespace std;

/**
 * Constructor
 * @param game The game this item is a member of
 * @param filename The filename for the item image
 */
Item::Item(Game* game, const std::wstring &filename) : mGame(game)
{
    mItemImage = make_unique<wxImage>(filename, wxBITMAP_TYPE_ANY);
    mItemBitmap = make_unique<wxBitmap>(*mItemImage);

}

/**
 * Destructor
 */
Item::~Item() {}

/**
 * Test if this item has been clicked on
 * @param x X location on the game to test
 * @param y Y location on the game to test
 * @return true if clicked on
 */
bool Item::HitTest(double x, double y)
{
    if (!mItemBitmap || !mItemBitmap->IsOk())
    {
        return false;
    }
    double wid = mItemBitmap->GetWidth();
    double hit = mItemBitmap->GetHeight();
    double testX = x - GetX() + wid / 2;
    double testY = y - GetY() + hit / 2;

    if (testX < 0 || testY < 0 || testX >= wid || testY >= hit)
    {
        return false;
    }
    if (mItemImage && mItemImage->IsOk())
    {
        return !mItemImage->IsTransparent((int)testX, (int)testY);
    }

    return true;
}

/**
 * Draw this item
 * @param graphics Graphics context to draw on
 */
void Item::Draw(std::shared_ptr<wxGraphicsContext> graphics)
{
    double wid = mItemBitmap->GetWidth();
    double hit = mItemBitmap->GetHeight();
    graphics->DrawBitmap(*mItemBitmap,
        (int)(GetX() - wid / 2),
        (int)(GetY() - hit / 2),
        wid, hit);
}



/**
 * Load the attributes for an item node.
 * @param node The XML node we are loading the item from
 */
void Item::XmlLoad(wxXmlNode* node)
{
    node->GetAttribute(L"x", L"0").ToDouble(&mX);
    node->GetAttribute(L"y", L"0").ToDouble(&mY);
}