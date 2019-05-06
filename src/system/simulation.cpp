//============================================================================
// @name        : simulation.cpp
// @author      : Thomas Dooms
// @date        : 4/23/19
// @version     : 
// @copyright   : BA1 Informatica - Thomas Dooms - University of Antwerp
// @description : 
//============================================================================

#include "simulation.h"
#include "../input-output/CAGenerator.h"
#include <cassert>

Simulation::Simulation(uint32_t width, uint32_t height)
{
    assert(CAGenerator::generate("./input/GameOfLife.ini"));
    window.init(width, height, Color(0.5,0.5,0.5));
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
                Window::delay(10);
                break;

            case Window::play:
                draw(++iteration);
                window.repaint();
                Window::delay(500);
                break;

            case Window::next:
                draw(++iteration);
                window.repaint();
                Window::delay(500);
                break;

            case Window::previous:
                if(iteration > 0) draw(--iteration);
                window.repaint();
                Window::delay(500);
                break;

            default:
                Window::delay(500);
        }
    }

    return true;
}

void Simulation::draw(uint32_t iteration)
{
    if(iteration == CA::getSize()) CA::update();
    else if(iteration > CA::getSize()) throw std::runtime_error("multiple steps at once");

    for(uint32_t i = 0; i < CA::getData(iteration).size(); i++) window[i] = CA::getColor(i, iteration);
}