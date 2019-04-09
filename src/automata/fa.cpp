//============================================================================
// @name        : FA.cpp
// @author      : Thomas Dooms
// @date        : 3/16/19
// @version     : 
// @copyright   : BA1 Informatica - Thomas Dooms - University of Antwerp
// @description : 
//============================================================================

#include "fa.h"
#include "json/json.h"
#include <algorithm>
#include <iostream>
#include <queue>


FA::FA(const std::vector<char>& alphabet, const std::vector<State*>& states, const Transition& transition, const std::string& type) : alphabet(alphabet), states(states), transition(transition), type(type)
{
    start = *std::find_if(begin(states), end(states), [](const auto& state){ return state->starting; });
}
FA::~FA()
{
    for(State* state : states) delete state;
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




