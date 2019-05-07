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
    Simulation();
    ~Simulation() = default;

    bool simulate();

    static uint32_t getDelay(uint32_t val);

private:
    void draw(uint32_t iteration);

    Window window;
};


#endif //GOL_SIMULATION_H
