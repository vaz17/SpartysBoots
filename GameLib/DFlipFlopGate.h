/**
* @file DFlipFlopGate.h
 * @author matthew vazquez
 */

#ifndef DFLIPFLOPGATE_H
#define DFLIPFLOPGATE_H

class InputPin;
class OutputPin;

#include "Gates.h"

/**
 * Class for or gate.
 */
class DFlipFlopGate : public Gates {
private:
    /// Previous state of the clock, used to track transitions.
    States mPreviousClock = States::Zero;

    /// InputPin A
    std::shared_ptr<InputPin> mInputA;
    /// InputPin B
    std::shared_ptr<InputPin> mInputB;
    /// OutputPin A
    std::shared_ptr<OutputPin> mOutputA;
    /// OutputPin B
    std::shared_ptr<OutputPin> mOutputB;
public:
    /**
     * Constructor for or gate
     * @param game the game instance
     */
    DFlipFlopGate(Game *game);

    void ComputeOutput();
    void Draw(std::shared_ptr<wxGraphicsContext> graphics) override;
    void OnClick(double x, double y) override;
    void Update(double elapsed) override;


    double getWidth() override;
    double getHeight() override;

    std::shared_ptr<IDraggable> HitDraggable(int x, int y) override;

    bool Connect(OutputPin *pin, wxPoint lineEnd) override;
};



#endif //DFLIPFLOPGATE_H
