//============================================================================
// @name        : DFA.cpp
// @author      : Ward Gauderis
// @date        : 5/4/19
// @version     : 
// @copyright   : BA1 Informatica - Ward Gauderis - University of Antwerp
// @description : 
//============================================================================
#include "DFA.h"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <json.h>

std::vector<char> DFAPlusPlus::alphabet = {};

std::vector<StatePlusPlus *> DFAPlusPlus::states = {};

DFAPlusPlusTransition DFAPlusPlus::transition = DFAPlusPlusTransition();

DFAPlusPlus::DFAPlusPlus(const StatePlusPlus *current) : current(current) {}

char DFAPlusPlus::operator()(const std::string &word) const {
    auto temp = current;
    for (const auto &c: word) current = transition(c, current);
    char type = current->type;
    current = temp;
    return type;
}

char DFAPlusPlus::getCurrent() const {
    return current->type;
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

void DFAPlusPlus::print(const std::string &fileName) const {
    std::ofstream wFile(fileName + ".dot");
    if (!wFile.is_open()) {
        std::cerr << "Error opening file " + fileName + ".dot\n";
        return;
    }
    wFile << "digraph DFAPlusPlus{\n"
             "\tresolution=250;\n"
             "\trankdir=LR;\n"
             "\tnode [fontname = \"roboto\"]\n"
             "\tedge [fontname = \"roboto\"]\n"
             "\tnode [ shape = circle ];\n"
             "\tstart [ style = invis, label = \"\" ];\n"
             "\tstart -> \"" + current->name + "\";\n";

    for (const auto &state: states) {
        wFile << "\t\"" + state->name + "\" [ label = <" + std::string(1, state->type) +
                 "<br/><FONT POINT-SIZE=\"8\">" + state->name +
                 "</FONT>> ];\n";
        std::map<std::string, std::vector<char >> arrows;
        for (const auto &symbol: alphabet) {
            auto next = transition(symbol, state);
            arrows[next->name].push_back(symbol);
        }
        for (const auto &arrow: arrows) {
            std::string labels = std::string(1, arrow.second[0]);
            for (int i = 1; i < arrow.second.size(); ++i) {
                labels += ", " + std::string(1, arrow.second[i]);
            }
            wFile << "\t\"" + state->name + "\" -> \"" + arrow.first + "\" [ label = \"" + labels + "\" ];\n";
        }
    }

    wFile << "}";
    wFile.close();
    const std::string command = "dot -Tpng " + fileName + ".dot -o" + fileName + ".png\n";
    system(command.c_str());
}

DFAPlusPlus::DFAPlusPlus(const std::string &fileName) {
    std::ifstream rFile(fileName);
    if (!rFile.is_open()) {
        std::cerr << "Error opening file " + fileName + ": no such file\n";
        return;
    }

    auto parser = nlohmann::json::parse(rFile);
    const std::string type = parser["type"];

    if (type != "DFAPlusPlus") {
        std::cerr << "Error parsing file " + fileName + ": type isn't DFAPlusPlus\n";
        return;
    }
    std::vector<std::string> alpha = parser["alphabet"];
    alphabet.clear();
    for (const auto &input: alpha) {
        alphabet.push_back(input[0]);
    }

    std::map<std::string, StatePlusPlus *> dict;
    for (const auto &state: states) delete state;
    for (const auto &state: parser["states"]) {
        std::string name = state["name"];
        char type = std::string(state["type"])[0];
        StatePlusPlus *newState = new StatePlusPlus(name, type);
        states.emplace_back(newState);
        dict[name] = newState;
        if (state["starting"]) current = newState;
    }

    transition = DFAPlusPlusTransition();
    for (const auto &trans: parser["transitions"]) {
        StatePlusPlus *from = dict[trans["from"]];
        StatePlusPlus *to = dict[trans["to"]];
        char input = std::string(trans["input"])[0];
        transition(input, from) = to;
    }
    rFile.close();
}