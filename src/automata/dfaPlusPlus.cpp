#include <utility>

#include <utility>

#include <utility>

#include <utility>

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

void dfaPlusPlus::TFAPlusPlus() {
    std::map<StatePlusPlus *, std::pair<StatePlusPlus *, bool >> table;
    for (auto it = states.begin(); it != states.end(); ++it) {  //Fill table and do a first pass on distinct states
        for (auto it2 = it + 1; it2 != states.end(); ++it2) {
            table[*it] = {*it2, (*it)->type != (*it2)->type};
        }
    }
    // a b c d e f g h
    // ab ac ad ae af ag ah
    // bc bd be bf bg bh
    // cd ce cd cg ch
    // de df dg dh
    // ef eg eh
    // fg fh
    // gh
    bool changed = true;
    while (changed) {
        changed = false;
        for (const auto &pair: table) {
            if (!pair.second.second) {
                for (const auto &c: alphabet) {
                    const StatePlusPlus *aNext = transition(c, pair.first);
                    const StatePlusPlus *bNext = transition(c, pair.second.first);

                }
            }
        }
    }
}

dfaPlusPlus::dfaPlusPlus(std::vector<StatePlusPlus *> states, DFAPlusPlusTransition transition,
                         const StatePlusPlus *current, std::vector<char> alphabet) : states(std::move(states)),
                                                                                     transition(std::move(transition)),
                                                                                     current(current),
                                                                                     alphabet(std::move(alphabet)) {}
