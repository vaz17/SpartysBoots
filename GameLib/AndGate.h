/**
* @file AndGate.h
 * @author matthew vazquez
 *
 * Describes the AndGate object for the game.
 *
 * Draws AndGate, computes output, and connects to pins.
 */

#ifndef ANDGATE_H
#define ANDGATE_H

class InputPin;
class OutputPin;

#include "Gates.h"

/**
 * Class for And gate
 */
class AndGate : public Gates {
private:
 /// wxGraphics path of gate
 wxGraphicsPath mPath;
 /// Pointer to Input Pin A
 std::shared_ptr<InputPin> mInputA;
 /// Pointer to Input Pin B
 std::shared_ptr<InputPin> mInputB;
 /// Pointer to Output Pin
 std::shared_ptr<OutputPin> mOutput;
public:
 /**
  * Constructor for or gate
  * @param game the game instance
  */
 AndGate(Game *game);

 void ComputeOutput();
 void Draw(std::shared_ptr<wxGraphicsContext> graphics) override;
 void OnClick(double x, double y) override; /// Questionable as to why it's here
 double getWidth() override;
 double getHeight() override;
 std::shared_ptr<IDraggable> HitDraggable(int x, int y) override;
 bool Connect(OutputPin *pin, wxPoint lineEnd) override;
 /// Updates Gate based on elapsed time
 void Update(double elapsed) override;

};



#endif //ANDGATE_H
