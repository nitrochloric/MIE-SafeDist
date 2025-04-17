#include <gui/main_screen/MainView.hpp>
#include <gui/main_screen/MainPresenter.hpp>

MainPresenter::MainPresenter(MainView& v)
    : view(v)
{

}

void MainPresenter::activate()
{
	model->setScanJunctionAccel(true);
}

void MainPresenter::deactivate()
{

}

void MainPresenter::newJunctionAccelValue(float value)
{
    view.setJunctionAccelValue(value);
}

void MainPresenter::newJunctionTempValue(uint32_t value)
{
    view.setJunctionTempValue(value);
}
