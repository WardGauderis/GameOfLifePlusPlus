#include <utility>

//============================================================================
// @name        : dfaPlusPlus.cpp
// @author      : Ward Gauderis
// @date        : 4/16/19
// @version     : 1.0
// @copyright   : BA1 Informatica - Ward Gauderis - University of Antwerp
// @description : Subclass of Cell with DFA++
//============================================================================
#include "dfaPlusPlus.h"

void dfaPlusPlus::operator()(const std::string &word) const {
    for (const auto &c: word) current = transition(c, current);
}

const std::string &dfaPlusPlus::getState() const {
    return current->type;
}

dfaPlusPlus::~dfaPlusPlus() {
    for (const auto &state: states) delete state;
}

dfaPlusPlus::dfaPlusPlus(const std::vector<const StatePlusPlus *const> &states, DFAPlusPlusTransition transition,
                         const StatePlusPlus *current) : states(states), transition(std::move(transition)),
                                                         current(current) {}
