/**
 * @file ProductVisitors.cpp
 * @author Shawn_Porto
 */

#include "pch.h"
#include "ProductVisitors.h"

#include "Product.h"

void SetLocationVisitor::VisitProduct(Product* product)
{
    product->SetLocation(newX, newY);
}

void ResetProduct::VisitProduct(Product* product)
{
    product->Reset();
}

void MoveProduct::VisitProduct(Product* product)
{
    product->MovePosition(changeX, changeY);
}
