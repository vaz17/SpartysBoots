/**
 * @file Conveyor.h
 *
 * @author karan choudhary
 *
 * This file here manages the implementation of the conveyor belt and allows it to be drawn
 * and manage products
 */

#ifndef CONVEYOR_H
#define CONVEYOR_H

#include "Item.h"
#include <wx/graphics.h>
#include <memory>
#include "Product.h"

/**
 * Underlying work for the conveyor belt
 *
 * This class manages the conveyor belt as well as moving the products around etc.
 */
class Conveyor : public Item
{
private:
    double mSpeed = 0;                    ///< Speed for the conveyor belt

    double mHeight = 0;                   ///< Height of the conveyor belt placed

    double mWidth = 0;                    ///< Width of the conveyor belt placed

    double mBeltSpeed;                    ///< Speed of the conveyor belt placed

    wxPoint mPanelLocation;               ///< Where the control panel is located

    bool mIsRunning = false;              ///< Determines whether he conveyor belt is running or not

    double mBeltPosition = 0;             ///< The current position of the conveyor belt

    std::unique_ptr<wxImage> mBackgroundImage;    ///< background for conveyor belt

    std::unique_ptr<wxBitmap> mBackgroundBitmap;  ///< background bitmap for conveyor belt

    std::unique_ptr<wxImage> mBeltImage;          ///< image fo the conveyor belt

    std::unique_ptr<wxBitmap> mBeltBitmap;        ///< image bitmap for conveyor belt

    std::unique_ptr<wxImage> mPanelStoppedImage;  ///< image for the conveoyr belt panel stopped

    std::unique_ptr<wxBitmap> mPanelStoppedBitmap;  ///< image bitmap fo convetyor belt panel stopped

    std::unique_ptr<wxImage> mPanelStartedImage;  ///< image of the panel when it runs

    std::unique_ptr<wxBitmap> mPanelStartedBitmap;  ///< image bitmap of the panek runnning

    /// Track number of products added to conveyor
    int mNumberOfProductsOnConveyor = 0;

    bool mPreviousProduct = false; ///< The previous product on the conveyor

public:
    Conveyor(Game* game);

    Conveyor() = delete;

    Conveyor(const Conveyor&) = delete;

    void operator=(const Conveyor&) = delete;

    void Draw(std::shared_ptr<wxGraphicsContext> graphics) override;
    bool HitTest(double x, double y) override;
    void Update(double elapsed) override;
    void XmlLoad(wxXmlNode* node) override;
    void OnClick(double x, double y) override;

    /**
     * We add the product to conveyor herer
     *
     * @param product A pointer to product to add to the conveyor
     */
    void AddProduct(std::shared_ptr<Product> product);

    void ResetProducts();

    void Start();

    void Stop();

    double GetSpeed() const;

    bool IsRunning() const;

   /**
    * We take a visitor for the convgeyor
    *
    * @param visitor Its the visitor that we use e for the conveyor
    */
	   void Accept(ItemVisitor* visitor) override;

};

#endif // CONVEYOR_H