//============================================================================
// @name        : Statement.cpp
// @author      : Ward Gauderis
// @date        : 5/16/19
// @version     :
// @copyright   : BA1 Informatica - Ward Gauderis - University of Antwerp
// @description :
//============================================================================
#include <fstream>
#include "statement.h"

POSITION::POSITION(int pos, char state, char next) : pos(pos), state(state), next(next) {}

TempDFA POSITION::generateTempDFA(const char def) {
    TempDFA dfa;
    auto prev = new StatePlusPlus(def);
    dfa.start = prev;
    dfa.states.emplace_back(prev);

    for (int i = 0; i < pos; ++i) {
        auto now = new StatePlusPlus(def);
        dfa.states.emplace_back(now);
        dfa.forAll(prev, now);
        prev = now;
    }

    auto accepted = new StatePlusPlus(next);
    dfa.states.emplace_back(accepted);
    auto rejected = new StatePlusPlus(def);
    dfa.states.emplace_back(rejected);

    dfa.forAllExcept(prev, rejected, state, accepted);
    dfa.forAll(accepted, accepted);
    dfa.forAll(rejected, rejected);

    return dfa;
}

AMOUNT::AMOUNT(int amount, char state, char next) : amount(amount), state(state), next(next) {}

TempDFA AMOUNT::generateTempDFA(const char def) {
    TempDFA dfa;
    auto prev = new StatePlusPlus((amount == 0) ? next : def);
    dfa.start = prev;
    dfa.states.emplace_back(prev);

    for (int i = 0; i < amount + 1; ++i) {
        auto now = new StatePlusPlus((i == amount - 1) ? next : def);
        dfa.states.emplace_back(now);
        dfa.forAllExcept(prev, prev, state, now);
        prev = now;
    }

    dfa.forAll(prev, prev);

    return dfa;
}

RANGEAND::RANGEAND(int min, int max, char state, char next) : min(min), max(max), state(state), next(next) {}

TempDFA RANGEAND::generateTempDFA(const char def) {
    TempDFA dfa;
    auto prev = new StatePlusPlus(def);
    dfa.start = prev;
    dfa.states.emplace_back(prev);

    for (int i = 0; i < min; ++i) {
        auto now = new StatePlusPlus(def);
        dfa.states.emplace_back(now);
        dfa.forAll(prev, now);
        prev = now;
    }

    auto rejected = new StatePlusPlus(def);
    dfa.states.emplace_back(rejected);

    for (int i = 0; i < max - min; ++i) {
        auto now = new StatePlusPlus(def);
        dfa.states.emplace_back(now);
        dfa.forAllExcept(prev, rejected, state, now);
        prev = now;
    }

    auto accepted = new StatePlusPlus(next);
    dfa.states.emplace_back(accepted);

    dfa.forAllExcept(prev, rejected, state, accepted);
    dfa.forAll(accepted, accepted);
    dfa.forAll(rejected, rejected);

    return dfa;
}

RANGEOR::RANGEOR(int min, int max, char state, char next) : min(min), max(max), state(state), next(next) {}

TempDFA RANGEOR::generateTempDFA(const char def) {
    TempDFA dfa;
    auto prev = new StatePlusPlus(def);
    dfa.start = prev;
    dfa.states.emplace_back(prev);

    for (int i = 0; i < min; ++i) {
        auto now = new StatePlusPlus(def);
        dfa.states.emplace_back(now);
        dfa.forAll(prev, now);
        prev = now;
    }

    auto accepted = new StatePlusPlus(next);
    dfa.states.emplace_back(accepted);

    for (int i = 0; i < max - min; ++i) {
        auto now = new StatePlusPlus(def);
        dfa.states.emplace_back(now);
        dfa.forAllExcept(prev, now, state, accepted);
        prev = now;
    }

    auto rejected = new StatePlusPlus(def);
    dfa.states.emplace_back(rejected);

    dfa.forAllExcept(prev, rejected, state, accepted);
    dfa.forAll(rejected, rejected);
    dfa.forAll(accepted, accepted);

    return dfa;
}

RANGEAMOUNT::RANGEAMOUNT(int min, int max, char state, char next) : min(min), max(max), state(state), next(next) {}

TempDFA RANGEAMOUNT::generateTempDFA(const char def) {
    TempDFA dfa;
    auto prev = new StatePlusPlus((min == 0) ? next : def);
    dfa.start = prev;
    dfa.states.emplace_back(prev);

    for (int i = 0; i < max + 1; ++i) {
        auto now = new StatePlusPlus((i >= min - 1 && i <= max - 1) ? next : def);
        dfa.states.emplace_back(now);
        dfa.forAllExcept(prev, prev, state, now);
        prev = now;
    }

    dfa.forAll(prev, prev);

    return dfa;
}

AND::AND(Statement *first, Statement *second) : first(first), second(second) {}

AND::~AND() {
    delete first;
    delete second;
}

TempDFA AND::generateTempDFA(const char def) {
    auto dfa1 = first->generateTempDFA(def);
    auto dfa2 = second->generateTempDFA(def);
    auto comb = dfa1.multiply(dfa2, def, cross);
    comb.TFAPlusPlus();
    return comb;
}

OR::OR(Statement *first, Statement *second) : first(first), second(second) {}

OR::~OR() {
    delete first;
    delete second;
}

TempDFA OR::generateTempDFA(const char def) {
    auto dfa1 = first->generateTempDFA(def);
    auto dfa2 = second->generateTempDFA(def);
    auto comb = dfa1.multiply(dfa2, def, prioritizedUnion);
    comb.TFAPlusPlus();
    return comb;
}

NOT::~NOT() {
    delete statement;
}

TempDFA NOT::generateTempDFA(const char def) {
    auto dfa = statement->generateTempDFA(def);
    for (auto &state: dfa.states) {
        if (state->type == def) {
            state->type = next;
        } else {
            state->type = def;
        }
    }
    return dfa;
}

NOT::NOT(Statement *statement, char next) : statement(statement), next(next) {}

Statement::~Statement() {}

void TempDFA::print(const std::string &fileName, const StateMap &stateMap) const {
    std::ofstream wFile(fileName + ".dot");
    if (!wFile.is_open()) {
        std::cerr << "Error opening file '" + fileName + ".dot'\n";
        return;
    }

    wFile << "digraph DFAPlusPlus{\n"
             "\trankdir=LR;\n"
             "\tnode [fontname = \"roboto\"]\n"
             "\tedge [fontname = \"roboto\"]\n"
             "\tnode [ shape = circle, style=filled ];\n"
             "\tbeginz [ style = invis, label = \"\" ];\n"
             "\tbeginz -> \"" + std::to_string(intptr_t(start)) + "\";\n";

    for (const auto &state: states) {
        Color color = stateMap.color(state->type);
        wFile << "\t\"" + std::to_string(intptr_t(state)) + "\" [ label = \"\", fillcolor = \"" + color.to_string() +
                 "\" ];\n";
        std::map<std::string, std::vector<char >> arrows;
        for (const auto &symbol: stateMap) {
            auto next = transition[{std::get<1>(symbol), state}];
            arrows[std::to_string(intptr_t(next))].push_back(std::get<1>(symbol));
        }
        for (const auto &arrow: arrows) {
            std::string labels = stateMap.name(arrow.second[0]);
            for (unsigned int i = 1; i < arrow.second.size(); ++i) {
                labels += ", " + stateMap.name(arrow.second[i]);
            }
            wFile << "\t\"" + std::to_string(intptr_t(state)) + "\" -> \"" + arrow.first + "\" [ label = \"" + labels +
                     "\" ];\n";
        }
    }

    wFile << "}";
    wFile.close();
    const std::string command = "dot -Tpng " + fileName + ".dot -o" + fileName + ".png &\n";
    system(command.c_str());
}

void TempDFA::forAll(const StatePlusPlus *from, const StatePlusPlus *to) {
    for (const auto &symbol: DFAPlusPlus::alphabet) {
        transition[{symbol, from}] = to;
    }
}

void TempDFA::forAllExcept(const StatePlusPlus *from, const StatePlusPlus *to, const char except,
                           const StatePlusPlus *toExcept) {
    for (const auto &symbol: DFAPlusPlus::alphabet) {
        if (symbol == except) continue;
        transition[{symbol, from}] = to;
    }
    transition[{except, from}] = toExcept;
}

TempDFA TempDFA::multiply(const TempDFA &dfa, const char def, const Mult type) {
    TempDFA comb;
    std::map<std::pair<const StatePlusPlus *, const StatePlusPlus *>, const StatePlusPlus *> map;
    comb.start = addStatesRec(start, dfa.start, map, comb, dfa, def, type);
    for (const auto &state: states) delete state;
    for (const auto &state: dfa.states) delete state;
    return comb;
}

StatePlusPlus *TempDFA::addStatesRec(const StatePlusPlus *first, const StatePlusPlus *second,
                                     std::map<std::pair<const StatePlusPlus *, const StatePlusPlus *>, const StatePlusPlus *> &map,
                                     TempDFA &comb, const TempDFA &dfa, const char def, const Mult multType) {
    char type = 0;
    switch (multType) {
        case cross:
            type = (first->type != def) && (second->type != def) ? first->type : def;
            break;
        case prioritizedUnion:
            if (first->type == def)
                type = second->type;
            else
                type = first->type;
            break;
    }
    auto combState = new StatePlusPlus(type);
    comb.states.emplace_back(combState);

    map[{first, second}] = combState;
    for (const auto &c: DFAPlusPlus::alphabet) {
        auto next1 = transition[{c, first}];
        auto next2 = dfa.transition[{c, second}];
        if (map.find({next1, next2}) == map.end()) {
            map[{next1, next2}] = addStatesRec(next1, next2, map, comb, dfa, def, multType);
        }
        comb.transition[{c, combState}] = map[{next1, next2}];
    }
    return combState;
}

void TempDFA::TFAPlusPlus() {
    auto compare = [](const StatePlusPlus *a, const StatePlusPlus *b) {
        return a < b;
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
                    for (const auto &c: DFAPlusPlus::alphabet) {
                        const StatePlusPlus *aNext = transition[{c, pair.first}];
                        const StatePlusPlus *bNext = transition[{c, pairs.first}];
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
        if (state == start) {
            minCurrent = minState;
        }
    }
    DFAPlusPlusTransition minTransition;
    std::vector<StatePlusPlus *> minStates;
    upgradeToMin(minTransition, minStates, minStatesmap, minCurrent);
    start = minStatesmap[minCurrent];
    transition = minTransition;
    for (const auto &state: states) delete state;
    states = minStates;
}

StatePlusPlus *TempDFA::upgradeToMin(DFAPlusPlusTransition &minTransition, std::vector<StatePlusPlus *> &minStates,
                                     std::map<std::set<const StatePlusPlus *>, StatePlusPlus *> &minStatesMap,
                                     const std::set<const StatePlusPlus *> &minState) {
    auto *temp = new StatePlusPlus((*minState.begin())->type);
    minStatesMap[minState] = temp;
    minStates.push_back(temp);

    for (const auto &c: DFAPlusPlus::alphabet) {
        const StatePlusPlus *next = transition[{c, (*minState.begin())}];
        for (auto &minStatePair: minStatesMap) {
            if (minStatePair.first.find(next) != minStatePair.first.end()) {   //minState met next gevonden
                if (!minStatePair.second) { //  state nog niet aangemaakt
                    minStatePair.second = upgradeToMin(minTransition, minStates, minStatesMap, minStatePair.first);
                }
                minTransition[{c, temp}] = minStatePair.second;
                break;
            }
        }
    }
    return minStatesMap[minState];
}