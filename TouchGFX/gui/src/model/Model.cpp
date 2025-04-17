#include <gui/model/Model.hpp>
#include <gui/model/ModelListener.hpp>
#include <stdint.h>

extern "C" {
	extern float ACCEL_SENSOR_GetValue();
	extern uint32_t TEMP_SENSOR_GetValue();
}

Model::Model() : modelListener(0)
        , tickCounter(0), scanJunctionAccel(false), scanJunctionTemp(false)
{

}

void Model::tick()
{
    tickCounter++;
    if (((tickCounter % 20) == 0) && (scanJunctionAccel))
    {
      if (modelListener != 0)
      {
        modelListener->newJunctionAccelValue(getAccelValue());
        modelListener->newJunctionTempValue(getTempValue());
      }
    }
}

void Model::setScanJunctionAccel(bool scanEnabled)
{
	scanJunctionAccel = scanEnabled;
}

void Model::setScanJunctionTemp(bool scanEnabled)
{
	scanJunctionTemp = scanEnabled;
}

float Model::getAccelValue()
{
	return ACCEL_SENSOR_GetValue();
}

uint32_t Model::getTempValue()
{
	return TEMP_SENSOR_GetValue();
}
