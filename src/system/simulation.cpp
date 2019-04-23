//============================================================================
// @name        : simulation.cpp
// @author      : Thomas Dooms
// @date        : 4/23/19
// @version     : 
// @copyright   : BA1 Informatica - Thomas Dooms - University of Antwerp
// @description : 
//============================================================================

#include "simulation.h"

Simulation::Simulation(uint32_t width, uint32_t height)
{
    window.init(width, height, Color(255,0,0));
    window.show();
}

bool Simulation::simulate()
{
    bool done = false;
    window.showPlayButton();

    while(!done)
    {
        if (window.isPlay())
        {
            window.update();

        }
        Window::delay(500);
        window.repaint();

    }

    return true;
}