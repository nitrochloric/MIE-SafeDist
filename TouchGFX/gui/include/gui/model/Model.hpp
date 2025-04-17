#ifndef MODEL_HPP
#define MODEL_HPP
#include <vector>
#include "stdint.h"
class ModelListener;

class Model
{
public:
    Model();

    void bind(ModelListener* listener)
    {
        modelListener = listener;
    }

    void tick();
    void setScanJunctionAccel(bool scanEnabled);
    void setScanJunctionTemp(bool scanEnabled);
    float getAccelValue();
    uint32_t getTempValue();
protected:

    ModelListener* modelListener;
    int tickCounter;
    bool scanJunctionAccel;
    bool scanJunctionTemp;
};

#endif // MODEL_HPP
