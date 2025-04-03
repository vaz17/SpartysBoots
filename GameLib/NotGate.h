/**
* @file NotGate.h
 * @author matthew vazquez
 *
 *
 */

#ifndef NOTGATE_H
#define NOTGATE_H

class InputPin;
class OutputPin;


#include "Gates.h"

/**
 * Class for or gate
 */
class NotGate : public Gates {
private:
    /// InputPin
    std::shared_ptr<InputPin> mInput;
    /// OutputPin
    std::shared_ptr<OutputPin> mOutput;
public:
    /**
     * Constructor for or gate
     * @param game the game instance
     */
    NotGate(Game *game);

    void ComputeOutput();
    void Draw(std::shared_ptr<wxGraphicsContext> graphics) override;
    void OnClick(double x, double y) override;
    void Update(double elapsed) override;

    double getWidth() override;
    double getHeight() override;

    std::shared_ptr<IDraggable> HitDraggable(int x, int y) override;

    bool Connect(OutputPin *pin, wxPoint lineEnd) override;
};



#endif //NOTGATE_H
