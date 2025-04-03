/**
* @file OrGate.h
 * @author matthew vazquez
 */
#ifndef ORGATE_H
#define ORGATE_H

#include "Gates.h"

class InputPin;
class OutputPin;

/**
 * Class for or gate
 */
class OrGate : public Gates {
private:

    /// InputPin A
    std::shared_ptr<InputPin> mInputA;
    /// InputPin B
    std::shared_ptr<InputPin> mInputB;
    /// OutputPin
    std::shared_ptr<OutputPin> mOutput;
public:
    /**
     * Constructor for or gate
     * @param game the game instance
     */
    OrGate(Game *game);

    void ComputeOutput();
    void Draw(std::shared_ptr<wxGraphicsContext> graphics) override;
    void OnClick(double x, double y) override;
    void Update(double elapsed) override;


    double getWidth() override;
    double getHeight() override;

    std::shared_ptr<IDraggable> HitDraggable(int x, int y) override;

    bool Connect(OutputPin *pin, wxPoint lineEnd) override;
};

#endif // ORGATE_H