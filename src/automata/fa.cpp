//============================================================================
// @name        : FA.cpp
// @author      : Thomas Dooms
// @date        : 3/16/19
// @version     : 
// @copyright   : BA1 Informatica - Thomas Dooms - University of Antwerp
// @description : 
//============================================================================

#include "fa.h"
#include <algorithm>
#include <iostream>
#include <queue>


FA::FA(const std::vector<char>& alphabet, const std::vector<const State*>& states, const FATransition& transition, const std::string& type) : alphabet(alphabet), states(states), transition(transition), type(type)
{
    start = *std::find_if(begin(states), end(states), [](const auto& state){ return state->starting; });
}
FA::~FA()
{
    for(const State* state : states) delete state;
}

bool FA::operator()(const std::string& word) const
{
    std::set<const State*> current = {start};
    if(type == "ENFA") ecloseCurrent(current);
    for(const char c : word)
    {
        std::set<const State*> newStates;
        for(const State* state : current)
        {
            const auto temp = transition(c, state);
            newStates.insert(begin(temp), end(temp));
        }
        current.clear();
        current = newStates;
        if(type == "ENFA") ecloseCurrent(current);
    }
    return std::any_of(begin(current), end(current), [](const auto& state){ return state->accepting; });
}

void FA::ecloseCurrent(std::set<const State*>& current) const
{
    if(type != "ENFA") return;
    char epsilon = alphabet.back();
    for(const State* state : current)
    {
        const auto temp = transition(epsilon, state);
        current.insert(begin(temp), end(temp));
    }
}


void FA::dot(const std::string& path) const
{
    std::ofstream file(path);

    file << "digraph G {\n";
    file << "NULL -> " << start->name << ";\n";
    file << "NULL [style=invis];\n";

    for(const State* state: states)
    {
        for(const char c : alphabet)
        {
            for(const auto& newState : transition(c, state)) file << state->name << " -> " << newState->name << "[label=\"" << c << "\"];\n";
        }
    }
    for(const State* state : states)
    {
        if(state->accepting) file << state->name << "[peripheries=2];\n";
    }

    file << "}" << std::endl; // Flushing is very important.
    std::string pngPath = path.substr(0, path.size() - 3) + "png";
    system(("dot -Tpng " + path + " -o " + pngPath).c_str());
}




