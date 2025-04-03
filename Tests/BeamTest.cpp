/**
 * @file BeamTest.cpp
 * @author Silas Hodges
 */

#include <pch.h>
#include "gtest/gtest.h"
#include <Beam.h>
#include <Conveyor.h>
#include <Game.h>
#include <Product.h>
#include <Sensor.h>
#include <ItemVisitor.h>
#include <OutputPin.h>
#include <Gates.h>

class MockProduct : public Product
{
public:
    MockProduct(Game* game, double size = 10.0) : Product(game), mMockSize(size)
    {
	       SetLocation(0.0, 0.0);
    }

	   double GetSize() const override
    {
	       return mMockSize;
    }

private:
	   double mMockSize;
};

class MockSensor : public Sensor
{
public:
	   MockSensor(Game* game) : Sensor(game) {}

	   void UpdatePins(const std::vector<Product::Properties>& detectedProperties) override
	   {
		      mDetectedProperties = detectedProperties;
	   }

	   const std::vector<Product::Properties>& GetDetectedProperties() const
	   {
		      return mDetectedProperties;
	   }

private:
	   std::vector<Product::Properties> mDetectedProperties;
};

TEST(BeamTest, Initialization)
{
	Game game;
	Beam beam(&game);

	ASSERT_FALSE(beam.GetBeamBroken());
	ASSERT_EQ(beam.GetSenderOffset(), 0.0);
}
