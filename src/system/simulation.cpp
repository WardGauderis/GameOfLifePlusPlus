//============================================================================
// @name        : simulation.cpp
// @author      : Thomas Dooms
// @date        : 4/23/19
// @version     : 
// @copyright   : BA1 Informatica - Thomas Dooms - University of Antwerp
// @description : 
//============================================================================

#include "simulation.h"
#include "../input-output/caio.h"
#include <cassert>

Simulation::Simulation()
{
    window.createIniButtons();
    window.show();

    while(!window.isInitialized())
    {
        window.delay(500);
    }

    if(!CAIO::generate(window.getFilename())) exit(69);

    window.initCA(CA::getWidth(), CA::getHeight(), CA::getColors());

    window.createEditButtons();
    window.setInitialized(false);

    while(!window.isInitialized())
    {
        window.delay(500);
    }
    CA::setStart(window.getStart());
}

bool Simulation::simulate()
{
    bool done = false;
    uint32_t iteration = 0;
    window.showPlayButton();

    draw(0);
    window.repaint();

    while(!done)
    {
        switch(window.getState())
        {
            case Window::pause:
                window.delay(10);
                break;

            case Window::play:
                draw(++iteration);
                window.delay(getDelay(window.getSliderValue()));
                break;

            case Window::next:
                draw(++iteration);
                window.delay(10);
                break;

            case Window::previous:
                if(iteration > 0) draw(--iteration);
                window.delay(10);
                break;
            case Window::playback:
                if(iteration > 0) draw(--iteration);
                window.delay(getDelay(window.getSliderValue()));
                break;
            case Window::quit:
                exit(0);

            default:
                window.delay(500);
        }
    }

    return true;
}

void Simulation::draw(uint32_t iteration)
{
    if(iteration == CA::getSize()) CA::update();
    else if(iteration > CA::getSize()) throw std::runtime_error("multiple steps at once");

    for(uint32_t i = 0; i < CA::getData(iteration).size(); i++) window[i] = CA::getChar(i, iteration);
    window.setTicksPassed(iteration);
}

uint32_t Simulation::getDelay(uint32_t val)
{
    if(val > 100) return 0;
    else return 50 + static_cast<uint32_t>(std::pow((100-val), 1.2) * 5);
}