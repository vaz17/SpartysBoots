/**
 * @file ItemVisitor.h
 * @author Shawn_Porto
 *
 * Visitor that goes into Items
 * Declares virtual visit functions for beam, conveyor, gates, sensors
 * Serves as base class for visitors.
 */
 
#ifndef ITEMVISITOR_H
#define ITEMVISITOR_H

// Forward Declarations
class Beam;
class Conveyor;
class Scoreboard;
class Sensor;
class SensorPanel;
class Sparty;
class Product;
class Gates;
class AndGate;
class DFlipFlopGate;
class NotGate;
class OrGate;
class SrFlipFlopGate;

/**
 * Base class for visiting items.
 *
 * Derived classes override these functions
 */
class ItemVisitor
{
protected:
    /**
     * Constructor
     * insures this is an abstract class
     */
    ItemVisitor();
public:
    /**
     * Object destructor.
     */
    virtual ~ItemVisitor();

    /**
     * Visit a Beam object
     * @param beam Beam we are visiting
     */
    virtual void VisitBeam(Beam* beam) {}

    /**
     * Visit a Conveyor object
     * @param conveyor Conveyor we are visiting
     */
    virtual void VisitConveyor(Conveyor* conveyor) {}

    /**
     * Visit a Scoreboard object
     * @param scoreboard Scoreboard we are visiting
     */
    virtual void VisitScoreboard(Scoreboard* scoreboard) {}

    /**
     * Visit a Sensor object
     * @param sensor Sensor we are visiting
     */
    virtual void VisitSensor(Sensor* sensor) {}

    /**
     * Visit a Sparty object
     * @param sparty Sparty we are visiting
     */
    virtual void VisitSparty(Sparty* sparty) {}

    /**
     * Visit a Product object
     * @param product Product we are visiting
     */
    virtual void VisitProduct(Product* product) {}

    /**
     * Visit a Gate object
     * @param gates Gate we are visiting
     */
    virtual void VisitGates(Gates* gates) {}

    /**
       * Visit a SensorPanel object
       * @param sensorPanel SensorPanel we are visiting
       */
    virtual void VisitSensorPanel(SensorPanel* sensorPanel) {}
};



#endif //ITEMVISITOR_H
