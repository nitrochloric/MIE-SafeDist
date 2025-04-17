#ifndef MODELLISTENER_HPP
#define MODELLISTENER_HPP

#include <gui/model/Model.hpp>

class ModelListener
{
public:
    ModelListener() : model(0) {}
    
    virtual ~ModelListener() {}

    void bind(Model* m)
    {
        model = m;
    }
    virtual void newJunctionAccelValue(float value) {}
    virtual void newJunctionTempValue(uint32_t value) {}
protected:
    Model* model;
};

#endif // MODELLISTENER_HPP
