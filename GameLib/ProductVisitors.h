/**
 * @file ProductVisitors.h
 * @author Shawn_Porto
 *
 * Contains classes for visiting products.
 * Derived from item visitor. Can manipulate products by
 * adding, moving, resetting, and setting location
 */
 
#ifndef PRODUCTVISITORS_H
#define PRODUCTVISITORS_H

#include "ItemVisitor.h"

/**
 * Visitor to add a product.
 */
class AddProduct : public ItemVisitor
{
private:

public:
    /**
     * Visits products
     * @param product Pointer to visited product
     */
    void VisitProduct(Product* product) override;
};

/**
 * Visitor to set new location for a product.
 */
class SetLocationVisitor : public ItemVisitor
{
    private:
        /// New X for the product.
        double newX;
        /// New Y for the product.
        double newY;

    public:
        /**
         * Sets new location of product
         * @param x New X coordinate.
         * @param y New Y coordinate.
         */
        void SetLocation(double x, double y){newX = x; newY = y;}

        /**
         * Vists product to update location
         * @param product Pointer to visited product.
         */
        void VisitProduct(Product* product) override;
};

/**
 * Resets the products positions to it's initial positions
 */
class ResetProduct : public ItemVisitor
{
    private:

public:
    /**
     * Visits product to reset position to initial state
     * @param product Pointer to product being visited
     */
    void VisitProduct(Product* product) override;
};

/**
 * Visitor to move products by amount specified.
 */
class MoveProduct : public ItemVisitor
{
private:
    /// Amount to change X coordinate.
    double changeX;
    /// Amount to change Y coordinate.
    double changeY;
public:
    /**
     * Constructor to initialize coordinates.
     * @param x Amount to change X coord.
     * @param y Amount to change Y coord.
     */
    MoveProduct(double x, double y) {changeX = x, changeY = y;}

    /**
     * Visits products in order to move them.
     * @param product pointer to product object.
     */
    void VisitProduct(Product* product) override;
};

#endif //PRODUCTVISITORS_H
