//============================================================================
// @name        : dfaPlusPlus.cpp
// @author      : Ward Gauderis
// @date        : 4/16/19
// @version     : 1.0
// @copyright   : BA1 Informatica - Ward Gauderis - University of Antwerp
// @description : Subclass of Cell with DFA++
//============================================================================

#include <algorithm>
#include "dfaPlusPlus.h"

std::vector<char> DFAPlusPlus::alphabet = {};

std::vector<StatePlusPlus *> DFAPlusPlus::states = {};

DFAPlusPlusTransition DFAPlusPlus::transition = DFAPlusPlusTransition();

DFAPlusPlus::DFAPlusPlus(const StatePlusPlus *current) : current(current) {}

void DFAPlusPlus::operator()(const std::string &word) const {
    for (const auto &c: word) current = transition(c, current);
}

// was eerst const string& return current->type
char DFAPlusPlus::getState() const {
    return 65;
}

void DFAPlusPlus::TFAPlusPlus() {
    auto compare = [](const StatePlusPlus *a, const StatePlusPlus *b) {
        if ((*a).name < (*b).name) return true;
        if ((*b).name < (*a).name) return false;
        return (*a).type < (*b).type;
    };
    std::sort(states.begin(), states.end(), compare);
    std::map<const StatePlusPlus *, std::map<const StatePlusPlus *, bool >> table;
    for (auto it = states.begin(); it != states.end(); ++it) {  //Fill table and do a first pass on distinct states
        for (auto it2 = it + 1; it2 != states.end(); ++it2) {
            table[*it][*it2] = (*it)->type != (*it2)->type; //true if distinct
        }
    }
    bool changed = true;
    while (changed) {   //keep doing iterations of marking distinct states until all are discovered
        changed = false;
        for (const auto &pairs: table) {
            for (const auto &pair: pairs.second) {
                if (!pair.second) {
                    for (const auto &c: alphabet) {
                        const StatePlusPlus *aNext = transition(c, pair.first);
                        const StatePlusPlus *bNext = transition(c, pairs.first);
                        if (aNext != bNext &&
                            table[std::min(aNext, bNext, compare)][std::max(aNext, bNext, compare)]) {
                            table[pairs.first][pair.first] = true;
                            changed = true;
                        }
                    }
                }
            }
        }
    }
    std::map<std::set<const StatePlusPlus *>, StatePlusPlus *> minStatesmap;
    std::set<const StatePlusPlus *> minState;
    std::set<const StatePlusPlus *> minCurrent;
    for (const auto &state: states) {   //fill in minStates with sets of equivalent states;
        minState = {state};
        for (const auto &pair: table[state]) {
            if (!pair.second) {
                minState.emplace(pair.first);
            }
        }
        for (const auto &pairs: table) {
            if (pairs.second.find(state) != pairs.second.end() && !pairs.second.at(state)) {
                minState.emplace(pairs.first);
            }
        }
        minStatesmap[minState] = nullptr;
        if (state == current) {
            minCurrent = minState;
        }
    }
    DFAPlusPlusTransition minTransition;
    std::vector<StatePlusPlus *> minStates;
    upgradeToMin(minTransition, minStates, minStatesmap, minCurrent);
    current = minStatesmap[minCurrent];
    transition = minTransition;
    for (const auto &state: states) delete state;
    states = minStates;
    int i = 5;
}

StatePlusPlus *DFAPlusPlus::upgradeToMin(DFAPlusPlusTransition &minTransition, std::vector<StatePlusPlus *> &minStates,
                                         std::map<std::set<const StatePlusPlus *>, StatePlusPlus *> &minStatesMap,
                                         const std::set<const StatePlusPlus *> &minState) {
    std::string name = "{";
    for (const auto &oldState: minState) {
        name += oldState->name + ", ";
    }
    auto *temp = new StatePlusPlus(name.substr(0, name.size() - 2) + "}", (*minState.begin())->type);
    minStatesMap[minState] = temp;
    minStates.push_back(temp);

    for (const auto &c: alphabet) {
        const StatePlusPlus *next = transition(c, (*minState.begin()));
        for (auto &minStatePair: minStatesMap) {
            if (minStatePair.first.find(next) != minStatePair.first.end()) {   //minState met next gevonden
                if (!minStatePair.second) { //  state nog niet aangemaakt
                    minStatePair.second = upgradeToMin(minTransition, minStates, minStatesMap, minStatePair.first);
                }
                minTransition.getMap()[{temp, c}] = minStatePair.second;
                break;
            }
        }
    }
    return minStatesMap[minState];
}