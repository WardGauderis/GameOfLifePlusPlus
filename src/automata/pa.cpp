//============================================================================
// @name        : pa.cpp
// @author      : Thomas Dooms
// @date        : 4/10/19
// @version     : 
// @copyright   : BA1 Informatica - Thomas Dooms - University of Antwerp
// @description : 
//============================================================================

#include <algorithm>
#include "pa.h"

PA::PA(const std::vector<char>& alphabet, const std::vector<const State*>& states, const PATransition& transition) : alphabet(alphabet), states(states), transition(transition)
{
    start = *std::find_if(begin(states), end(states), [](const auto& state){ return state->starting; });
}

PA::~PA()
{
    for(const State* state : states) delete state;
}

bool PA::operator()(const std::string& word) const
{
    const State* current = start;
    for(char c : word)
    {
        double random = drand48();
        const auto next = transition[{c, current}];

        for(const auto& pair : next)
        {
            random -= pair.first;
            if(random < 0)
            {
                current = pair.second;
                break;
            }
        }
    }
    return current->accepting;
}
