/**
 * @file Product.h
 *
 * @author Karan Choudhary
 *
 *This file here does the implementation of the products being displayed on
 *the conveyor belt
 */
 
#ifndef PRODUCT_H
#define PRODUCT_H



#include "Item.h"
#include <map>
#include <string>
#include <memory>


/**
 * Class for the Conveyor
 *
 *Calls conveyor class in here to be used alongside product
 */
class Conveyor;


/**
 * Class for the Prducts on the conveyor belt
 *
 *This class is the implementation of the products that are displayed
 *on the conveyor belt and also inherits from item for some of its functianality
 */
class Product : public Item {
public:

 /**
 * Properties enum
 *
 * Has the properites of product such as square diamond, circle etc.
 */
 enum class Properties {None, Red, Green, Blue, White, Square, Circle, Diamond,
     Izzo, Smith, Football, Basketball};


 /**
 * Types enum
 *
 * Shows what type of properties you can have for ur product being color shape contrente
 */
 enum class Types {Color, Shape, Content}; ///< The types of our prodcuts on conveyor

 static const std::map<std::wstring, Properties> NamesToProperties; ///< A map of the names corresponding to properties
 static const std::map<Properties, Types> PropertiesToTypes; ///< A map coressponding the properties to types
 static const std::map<Properties, std::wstring> PropertiesToContentImages; ///< A map coressponding the properites to actual image output regarding it

 Product(Game* game);
 Product(Game* game, const std::wstring& filename);

 void Draw(std::shared_ptr<wxGraphicsContext> graphics) override;
 bool HitTest(double x, double y) override;
 void XmlLoad(wxXmlNode* node) override;
 void Update(double elapsed) override;

 /**
 * This makes it so taht whether kicking is enabled or not
 *
 * @param kick Value that determies whether to kick product off or not
 */
 void SetKick(bool kick) { mKick = kick; }

 /**
 * It gets the stats of the prodcut that is being kicked
 *
 * @return Returns true if product should be kicked off if not just makes it false
 */
 bool GetKick() const { return mKick; }
    void OnClick(double x, double y) override;

    /**
     * Resets product state.
     */
    void Reset();

 /**
 * It adds a property to the procut
 *
 * @param property What kind of property we want to add from the enum
 */
 void AddProperty(Properties property) { mProperties.push_back(property); }


   /**
    * It get sthe properties of the product
    *
    * @return It returns a vector regarding what kind of properties this product has
    */
    std::vector<Properties> GetProperties() const { return mProperties; }


   /**
   * It allows u to accept a visitor for the product
   *
   * @param visitor The visitor that we're usin for this product
   */
    void Accept(ItemVisitor* visitor) override { visitor->VisitProduct(this); }

    virtual double GetSize() const;

   /**
    * Sets the initial position of a product.
    * @param x X coordinate to set.
    * @param y Y coordinate to set.
    */
    void SetInitalPosition(double x, double y);

   /**
    * Moves product to specified position.
    * @param x new X coord.
    * @param y new Y coord.
    */
    void MovePosition(double x, double y);



   /**
    * We determine if the product can be grabbed or not
    *
    * @return Return true saying we can grab the product
    */
    bool IsGrabbable() override { return true ;};

    void Kick(double kick) override;


    /**
     * Returns flag for score updating
     * @return Whether score was updated
     */
    bool GetScoreUpdated() const { return mScoreUpdated; }

    /**
     * Set the flag for score being updated
     */
    void SetScoreUpdated() {mScoreUpdated = true;}

    /**
     * Get flag for whether product was kicked.
     * @return Whether product was kicked or not
     */
    bool GetWasKicked() const { return mWasKicked; }

private:
    double mInitialPlacementX = 0;///<The initial placement of the X
    double mInitialPlacementY = 0; ///< The current placement of product on conveyor belt
    bool mKick = false;           ///< Tells whether the product should be kicked off conveyor or not
    bool mWasKicked = false; ///< Keeps track of whether the product was kicked for scoring purposes.
    double mKickSpeed = 0; ///< Speed to move Product in X direction.
    bool mScoreUpdated = false; ///< Helps score be updated once per product
    std::vector<Properties> mProperties; ///< Vector for produts showing its various characteristics

};

#endif //PRODUCT_H
