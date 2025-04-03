/**
* @file SrFlipFlopGate.h
 * @author matthew vazquez
 *
 */

#ifndef SRFLIPFLOPGATE_H
#define SRFLIPFLOPGATE_H

class InputPin;
class OutputPin;

#include "Gates.h"

/**
 * Class for or gate
 */
class SrFlipFlopGate : public Gates {
private:

 /// Opposite output used for Q' in the gate.
 States mNotOutput;

 /// Input pin A
 std::shared_ptr<InputPin> mInputA;
 /// Input pin B
 std::shared_ptr<InputPin> mInputB;
 /// Output pin A
 std::shared_ptr<OutputPin> mOutputA;
 /// Output pin B
 std::shared_ptr<OutputPin> mOutputB;

public:
 /**
  * Constructor for or gate
  * @param game the game instance
  */
 SrFlipFlopGate(Game *game);

 void ComputeOutput();
 void Draw(std::shared_ptr<wxGraphicsContext> graphics) override;
 void OnClick(double x, double y) override;
 void Update(double elapsed) override;

 /**
  * Sets the output for Q'
  * @param output output to be set
  */
 void setNotOutput(const States output) { mNotOutput = output; }

 /**
  * Gets the NotOutput.
  * @return current NotOutput of gate
  */
 States getNotOutput() const { return mNotOutput; }


 double getWidth() override;
 double getHeight() override;

 std::shared_ptr<IDraggable> HitDraggable(int x, int y) override;

 bool Connect(OutputPin *pin, wxPoint lineEnd) override;
};



#endif //SRFLIPFLOPGATE_H
