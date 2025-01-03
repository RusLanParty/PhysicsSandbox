#include "Settings.h"
float Settings::_conversionFactor = 1.0f;

float Settings::getConversionFactor()
{
    return _conversionFactor;
}

void Settings::setConversionFactor(float convFact)
{
    _conversionFactor = convFact;
    
}

/*uint32_t Settings::getSubSteps()
{
    return _subStep;
}

void Settings::setSubSteps(uint32_t steps)
{
    this->_subStep = steps;
}

float Settings::getTimeStep()
{
   
}*/
