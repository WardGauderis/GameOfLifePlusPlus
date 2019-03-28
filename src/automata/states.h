//============================================================================
// @name        : state.h
// @author      : Thomas Dooms
// @date        : 3/16/19
// @version     : 
// @copyright   : BA1 Informatica - Thomas Dooms - University of Antwerp
// @description : 
//============================================================================

#pragma once

#include <string>

struct State
{
    std::string name;
    bool starting;
    bool accepting;
    uint32_t index;
};