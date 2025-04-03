/**
 * @file Gates.h
 * @author matthew vazquez
 *
 * Declares implementation for Gate Object used to play the game.
 */
 
#ifndef GATES_H
#define GATES_H

#include "Item.h"

/// The possible pin states
enum class States {One, Zero, Unknown};

/**
 * Base class for all gates
 */
class Gates : public Item {
private:

public:
 /// Default constructor (disabled)
 Gates() = delete;

 /// Copy constructor (disabled)
 Gates(const Gates &) = delete;

 /// Assignment operator
 void operator=(const Gates &) = delete;

 bool HitTest(double x, double y) override;

 /**
  * Checks if an item should be grabbed
  * @return true True if the item is a gate
  */
 bool IsGrabbable() override { return true; }

 /**
  * Accepts a visitor to visit the gates
  * @param visitor The visitor who will visit the gates
  */
 void Accept(ItemVisitor* visitor) override { visitor->VisitGates(this); }

 /**
  * Virtual function for getting Width of gates
  * @return Width of gates
  */
 virtual double getWidth() = 0;

 /**
 * Virtual function for getting Height of gates
 * @return Height of gates
 */
 virtual double getHeight() = 0;

protected:
 Gates(Game *game);

};

#endif //GATES_H
