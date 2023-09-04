#pragma once
#include <cstdint>
class Settings 
{
public:
	static float getConversionFactor();
	static void setConversionFactor(float convFact);
	//static uint32_t getSubSteps();
	//void setSubSteps(uint32_t steps);
	//static float getTimeStep();
private:
	static float _conversionFactor;
	//static uint32_t _subStep;
};