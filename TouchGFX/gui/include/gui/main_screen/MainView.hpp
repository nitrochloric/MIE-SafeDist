#ifndef MAINVIEW_HPP
#define MAINVIEW_HPP

#include <gui_generated/main_screen/MainViewBase.hpp>
#include <gui/main_screen/MainPresenter.hpp>
#include <vector>
//#include "_stdint.h"

extern "C"{
	extern void BUZZER_control(int select);
}

class MainView : public MainViewBase
{
public:
    MainView();
    virtual ~MainView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
    void setJunctionAccelValue(float value);
    void setJunctionTempValue(uint32_t value);
protected:
    float lastVitesseValue;
    float accelVals;
    uint32_t tempVal;
    uint32_t lastTime = 0;
};

#endif // MAINVIEW_HPP
