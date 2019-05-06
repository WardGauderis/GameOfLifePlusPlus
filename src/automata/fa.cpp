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

const FA* FA::minimize(const FA& fa)
{
    const auto size = static_cast<uint32_t>(fa.states.size());
    auto table = createTable(fa);

    std::vector<const State*> newStates;
    std::map<const State*, const State*> converter;
    std::vector<bool> skip(size, false);

    for(uint32_t i = 0; i < size; i++)
    {
        if(skip[i]) continue;
        auto newState = new State(*fa.states[i]);
        std::set<const State*> addedStates = {fa.states[i]};

        for(uint32_t j = 0; j < i; j++)
            if(!table[i][j])
            {
                addedStates.insert(fa.states[j]);
                if(fa.states[j]->starting) newState->starting = true;
                converter[fa.states[j]] = newState;
                skip[j] = true;
            }

        newState->name = makeName(addedStates);
        newStates.push_back(newState);
        converter[fa.states[i]] = newStates.back();
    }

    FATransition newTransition;
    for(auto& element : fa.transition.getMap())
    {
        auto& temp = newTransition(element.first.second, converter[element.first.first]);
        if(temp.empty()) temp.push_back(converter[element.second[0]]);
    }

    return new FA(fa.alphabet, newStates, newTransition, "dfa");
}

std::string FA::makeName(const std::set<const State*>& states)
{
    if(states.empty()) return "\"{}\"";
    std::string result = "\"{";
    for(const State* state : states) result += state->name + ',';
    result.back() = '}';
    return result + '\"';
}



std::vector<std::vector<bool>> FA::createTable(const FA& fa)
{
    const auto size = static_cast<uint32_t>(fa.states.size());
    std::vector<std::vector<bool>> table(size);
    for(auto& row : table) row = std::vector<bool>(size, false);

    for(const auto& state : fa.states)
    {
        if(!state->accepting) continue;
        const uint32_t i = state->index;
        for(uint32_t j = 0; j < size; j++)
        {
            table[i][j] = !table[i][j];
            table[j][i] = !table[j][i];
        }
    }

    bool done = false;
    while(!done) done = fillTable(table, fa);
    return table;
}

bool FA::fillTable(std::vector<std::vector<bool>>& table, const FA& fa)
{
    bool done = true;
    for(uint32_t i = 0; i < fa.states.size(); i++)
        for(uint32_t j = 0; j < i; j++)
        {
            if(table[i][j]) continue;
            for(const char c : fa.alphabet)
            {
                const auto i1 = fa.transition(c, fa.states[i])[0]->index;
                const auto j1 = fa.transition(c, fa.states[j])[0]->index;
                if(table[i1][j1])
                {
                    done = false;
                    table[i][j] = true;
                    break;
                }
            }
        }
    return done;
}




