//============================================================================
// @name        : pa.cpp
// @author      : Thomas Dooms
// @date        : 4/10/19
// @version     : 
// @copyright   : BA1 Informatica - Thomas Dooms - University of Antwerp
// @description : 
//============================================================================

#include <algorithm>
#include <fstream>
#include "pa.h"

PA::PA(const std::vector<char>& alphabet, const std::vector<const State*>& states, const PATransition& transition) : alphabet(alphabet), states(states), transition(transition)
{
    start = *std::find_if(begin(states), end(states), [](const auto& state){ return state->starting; });
}

PA::~PA()
{
    for(const State* state : states) delete state;
}

bool PA::operator()(const std::string& word, [[maybe_unused]] std::stack<char>& stack) const
{
    const State* current = start;
    for(char c : word)
    {
        double value = random(generator);
        const auto next = transition[{c, current}];

        for(const auto& pair : next)
        {
            value -= pair.first;
            if(value < 0)
            {
                current = pair.second;
                break;
            }
        }
    }
    return current->accepting;
}

void PA::dot(const std::string& path) const
{
    std::ofstream file(path + ".dot");

    file << "digraph G {\n";
    file << "NULL -> " << start->name << ";\n";
    file << "NULL [style=invis];\n";

    for(const State* state: states)
    {
        for(const char c : alphabet)
        {
            const auto transitions = transition[{c, state}];
            for(const auto next : transitions) file << state->name << " -> " << next.second->name << "[label=\"" << c << ", " << next.first << "\"];\n";
        }
    }
    for(const State* state : states)
    {
        if(state->accepting) file << state->name << "[peripheries=2];\n";
    }

    file << "}" << std::endl; // Flushing is very important.
    [[maybe_unused]] int res = system(("dot -Tpng " + path + ".dot -o " + path + ".png").c_str());
}
