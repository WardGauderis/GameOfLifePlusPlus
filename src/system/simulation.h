//============================================================================
// @name        : simulation.h
// @author      : Thomas Dooms
// @date        : 4/9/19
// @version     : 
// @copyright   : BA1 Informatica - Thomas Dooms - University of Antwerp
// @description : 
//============================================================================


#ifndef GOL_SIMULATION_H
#define GOL_SIMULATION_H

#include "../automata/ca.h"
#include "../gui/gui.h"


struct Simulation
{
public:
    Simulation(uint32_t width, uint32_t height) : ca(width, height)
    {
        window.init(width, height, Color(255,0,0));
        window.show();
    }
    ~Simulation() = default;

    bool execute()
    {
        // some kind of while loop here
        //ca.update();
        window.simulate();

        return true;
    }


private:
    Window window;
    CA ca;
};


#endif //GOL_SIMULATION_H
