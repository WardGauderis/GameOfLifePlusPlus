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
    window.showPlayButton();

    bool done = false;
    while(!done)
    {
        if (window.isPlayBtnClicked()) {
            window.update();
            Window::delay(500);
            window.repaint();
        }
    }

    return true;
}