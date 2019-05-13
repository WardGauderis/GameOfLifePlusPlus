//============================================================================
// @name        : state.h
// @author      : Thomas Dooms
// @date        : 3/20/19
// @version     :
// @copyright   : BA1 Informatica - Thomas Dooms - University of Antwerp
// @description :
//============================================================================
#include <algorithm>
#include <fstream>
#include "pda.h"

const char PDA::epsilon = '~';

PDA::PDA(const std::vector<char>& alphabet, const std::vector<char>& stackAlphabet, const std::vector<const PDAState*>& states, const PDATransition& transition)
: alphabet(alphabet), stackAlphabet(stackAlphabet), states(states), transition(transition)
{
    start = *std::find_if(begin(states), end(states), [](const auto& state){ return state->starting; });
}

bool PDA::operator()(const std::string& word, std::stack<char>& stack) const
{
    const PDAState* current = start;
    for(char c : word)
    {
        const auto next = transition[{c, current}];
        current = std::get<0>(next);
        const std::string& push = std::get<1>(next);
        const std::string& pop = std::get<2>(next);

        // we first pop, then push, goes against convention but we don't care.
        for(char character : pop)
        {
            if(!stack.empty() and character != epsilon)
            {
                if(stack.top() == character) stack.pop();
                else return false;
            }
        }
        for(char character : push)
        {
            if(character != epsilon) stack.push(character);
        }
    }
    return stack.empty();
}

void PDA::dot(const std::string& path) const
{
    std::ofstream file(path + ".dot");

    file << "digraph G {\n";
    file << "NULL -> " << start->name << ";\n";
    file << "NULL [style=invis];\n";

    for(const PDAState* state: states)
        for(const char c : alphabet)
        {
            const auto next = transition[{c, state}];
            file << state->name << " -> " << std::get<0>(next)->name << "[label=\"" << c << " " << std::get<1>(next) << " / " << std::get<2>(next) << "\"];\n";
        }

    file << "}" << std::endl; // Flushing is very important.
    [[maybe_unused]] int res = system(("dot -Tpng " + path + ".dot -o " + path + ".png").c_str());
}

