//============================================================================
// @name        : dfa.cpp
// @author      : Thomas Dooms
// @date        : 5/7/19
// @version     : 
// @copyright   : BA1 Informatica - Thomas Dooms - University of Antwerp
// @description : 
//============================================================================

#include "dfa.h"
#include <algorithm>
#include <iostream>
#include <queue>

DFA::DFA(const std::vector<char>& alphabet, const std::vector<const State*>& states, const DFATransition& transition) : alphabet(alphabet), states(states), transition(transition)
{
    start = *std::find_if(begin(states), end(states), [](const auto& state){ return state->starting; });
}
DFA::~DFA()
{
    for(const State* state : states) delete state;
}

bool DFA::operator()(const std::string& word, [[maybe_unused]] std::stack<char>& stack) const
{
    const State* current = start;
    for(char c : word) current = transition[{c, current}];
    return current->accepting;
}

void DFA::dot(const std::string& path) const
{
    std::ofstream file(path + ".dot");

    file << "digraph G {\n";
    file << "NULL -> " << start->name << ";\n";
    file << "NULL [style=invis];\n";

    for(const State* state: states)
    {
        for(const char c : alphabet)
        {
            file << state->name << " -> " << transition[{c, state}]->name << "[label=\"" << c << "\"];\n";
        }
    }
    for(const State* state : states)
    {
        if(state->accepting) file << state->name << "[peripheries=2];\n";
    }

    file << "}" << std::endl; // Flushing is very important.
    [[maybe_unused]] int res = system(("dot -Tpng " + path + ".dot -o " + path + ".png").c_str());
}

DFA* DFA::SSC(const NFA* fa)
{
    std::map<std::set<const State*>, const State*> existing;
    std::queue<std::set<const State*>> queue;
    uint32_t index = 0;

    std::vector<const State*> newStates;
    DFATransition newTransition;

    std::vector<char> newAlphabet = fa->alphabet;
    newAlphabet.pop_back();

    std::set<const State*> startStates = {fa->start};
    fa->ecloseCurrent(startStates);

    State* newState = new State{makeName(startStates), true, fa->start->accepting, 0};
    existing.emplace(startStates, newState);
    newStates.push_back(newState);
    queue.push(startStates);

    while(!queue.empty())
    {
        for(const char c : fa->alphabet)
        {
            if(fa->type == "enfa" and c == fa->alphabet.back()) continue;

            std::set<const State*> nextStates;
            for(const State* state : queue.front())
                for(const State* next : fa->transition[{c, state}])
                    nextStates.insert(next);

            fa->ecloseCurrent(nextStates);
            const auto& iter = existing.find(nextStates);
            if(iter != end(existing))
            {
                newTransition[{c, newStates[index]}] = iter->second;
                continue;
            }

            bool accepting = std::find_if(begin(nextStates), end(nextStates), [](const auto& state){ return state->accepting; }) == nextStates.end();
            newStates.push_back(new State{makeName(nextStates), false, accepting, static_cast<uint32_t>(newStates.size())});

            newTransition[{c, newStates[index]}] = newStates.back();
            existing.emplace(nextStates, newStates.back());
            queue.push(nextStates);
        }
        index++;
        queue.pop();
    }
    auto res = new DFA{newAlphabet, newStates, newTransition};
    system("[ -d output ] || mkdir output");
    res->dot("output/test.dot");
    return res;
}


const DFA* DFA::minimize(const DFA* fa)
{
    const auto size = static_cast<uint32_t>(fa->states.size());
    auto table = createTable(fa);

    std::vector<const State*> newStates;
    std::map<const State*, const State*> converter;
    std::vector<bool> skip(size, false);

    for(uint32_t i = 0; i < size; i++)
    {
        if(skip[i]) continue;
        auto newState = new State(*fa->states[i]);
        std::set<const State*> addedStates = {fa->states[i]};

        for(uint32_t j = 0; j < i; j++)
            if(!table[i][j])
            {
                addedStates.insert(fa->states[j]);
                if(fa->states[j]->starting) newState->starting = true;
                converter[fa->states[j]] = newState;
                skip[j] = true;
            }

        newState->name = makeName(addedStates);
        newStates.push_back(newState);
        converter[fa->states[i]] = newStates.back();
    }

    DFATransition newTransition;
    for(auto& element : fa->transition.getMap())
    {
        newTransition[{element.first.first, converter[element.first.second]}] = converter[element.second];
    }

    return new DFA(fa->alphabet, newStates, newTransition);
}

std::string DFA::makeName(const std::set<const State*>& states)
{
    if(states.empty()) return "\"{}\"";
    std::string result = "\"{";
    for(const State* state : states) result += state->name + ',';
    result.back() = '}';
    return result + '\"';
}



std::vector<std::vector<bool>> DFA::createTable(const DFA* fa)
{
    const auto size = static_cast<uint32_t>(fa->states.size());
    std::vector<std::vector<bool>> table(size);
    for(auto& row : table) row = std::vector<bool>(size, false);

    for(const auto& state : fa->states)
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

bool DFA::fillTable(std::vector<std::vector<bool>>& table, const DFA* fa)
{
    bool done = true;
    for(uint32_t i = 0; i < fa->states.size(); i++)
        for(uint32_t j = 0; j < i; j++)
        {
            if(table[i][j]) continue;
            for(const char c : fa->alphabet)
            {
                const auto i1 = fa->transition[{c, fa->states[i]}]->index;
                const auto j1 = fa->transition[{c, fa->states[j]}]->index;
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