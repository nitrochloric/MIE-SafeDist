#include <gui/main_screen/MainView.hpp>
#include <touchgfx/utils.hpp>
#include <vector>
#include <stdio.h>
#include "stm32f7xx_hal.h"

MainView::MainView()
: lastVitesseValue(0)
{

}

void MainView::setupScreen()
{
    MainViewBase::setupScreen();
}

void MainView::tearDownScreen()
{
    MainViewBase::tearDownScreen();
}

void MainView::setJunctionAccelValue(float value)
{
	uint32_t time = HAL_GetTick() - lastTime;
	lastTime = HAL_GetTick();
	if(time < 0){
		time = lastTime;
	}
	accelVals += (float)value/100 * (float)time/1000 * 3.6;
	printf("vitesse %f\r\n",accelVals);
	if(accelVals > 300){
		accelVals = 300;
	}else if(accelVals <-300){
		accelVals = -300;
	}
	if(accelVals >= 80){
		BUZZER_control(1);
	}else{
		BUZZER_control(0);
	}
	Vitesse.setValue((int)accelVals);
	Vitesse.invalidate();
	Unicode::snprintf(textVitesseBuffer, TEXTVITESSE_SIZE, "%d", (int)accelVals);
	textVitesse.invalidate();
}

void MainView::setJunctionTempValue(uint32_t value)
{
	tempVal = value;
	printf("temp = %i\r\n",tempVal);
	if(tempVal >= 30){
		BUZZER_control(1);
	}
	Distance.setValue(tempVal);
	Distance.invalidate();
}
