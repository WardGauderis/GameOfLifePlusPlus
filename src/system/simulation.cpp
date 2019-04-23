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
    uint32_t iteration = 0;
    window.showPlayButton();

    while(!done)
    {
        while(window.isPlay())
        {

        }

    }

    return true;
}

void Simulation::draw(uint32_t iteration)
{
    for(uint32_t i = 0; i < )
}