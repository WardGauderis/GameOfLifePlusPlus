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
    if(!CAIO::generate("./input/thomas_tests/test.ini")) exit(69);

    window.init(CA::getWidth(), CA::getHeight(), {Color(0, 1, 0)});
    window.show();
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
                window.repaint();
                window.delay(getDelay(window.getSliderValue()));
                break;

            case Window::next:
                draw(++iteration);
                window.repaint();
                window.delay(10);
                break;

            case Window::previous:
                if(iteration > 0) draw(--iteration);
                window.repaint();
                window.delay(10);
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

    for(uint32_t i = 0; i < CA::getData(iteration).size(); i++) window.setColor(i, CA::getColor(i, iteration));
}

uint32_t Simulation::getDelay(uint32_t val)
{
    if(val > 100) return 0;
    else return 50 + static_cast<uint32_t>(std::pow((100-val), 1.2) * 5);
}