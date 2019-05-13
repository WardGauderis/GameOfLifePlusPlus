//============================================================================
// @name        : pa.h
// @author      : Thomas Dooms
// @date        : 4/10/19
// @version     : 
// @copyright   : BA1 Informatica - Thomas Dooms - University of Antwerp
// @description : 
//============================================================================


#ifndef GOL_PA_H
#define GOL_PA_H

#include "automaton.h"
#include "states.h"
#include "transitions.h"
#include <vector>
#include <map>
#include <random>

class PA : public Automaton
{
public:
    PA(const std::vector<char>& alphabet, const std::vector<const State*>& states, const PATransition& transition);
    ~PA() override;

    bool operator()(const std::string& word, std::stack<char>& stack) const override;
    Automaton::Type getType() const override { return Automaton::Type::pa; }

private:
    std::vector<char> alphabet;
    std::vector<const State*> states;
    const State* start;
    PATransition transition;

    mutable std::default_random_engine generator;
    mutable std::uniform_real_distribution<double> random;
};


#endif //GOL_PA_H
