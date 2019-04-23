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
    Simulation(uint32_t width, uint32_t height);
    ~Simulation() = default;

    bool simulate();

private:
    void draw(uint32_t iteration);

    Window window;
};


#endif //GOL_SIMULATION_H
