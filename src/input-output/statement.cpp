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
    if (amount == 0) {
        dfa.start = new StatePlusPlus(next);
        dfa.states.emplace_back(dfa.start);
        auto rejected = new StatePlusPlus(def);
        dfa.states.emplace_back(rejected);

        dfa.forAllExcept(dfa.start, dfa.start, state, rejected);
        dfa.forAll(rejected, rejected);

        return dfa;
    }

    auto prev = new StatePlusPlus(def);
    dfa.start = prev;
    dfa.states.emplace_back(prev);

    for (const auto &symbol: DFAPlusPlus::alphabet) {
        if (symbol == state) continue;
        dfa.transition[{symbol, prev}] = prev;
    }

    for (int i = 0; i < amount - 1; ++i) {
        auto now = new StatePlusPlus(def);
        dfa.states.emplace_back(now);
        dfa.transition[{state, prev}] = now;
        for (const auto &symbol: DFAPlusPlus::alphabet) {
            if (symbol == state) continue;
            dfa.transition[{symbol, now}] = now;
        }
        prev = now;
    }

    auto accepted = new StatePlusPlus(next);
    dfa.states.emplace_back(accepted);
    dfa.transition[{state, prev}] = accepted;

    auto rejected = new StatePlusPlus(def);
    dfa.states.emplace_back(rejected);

    dfa.forAllExcept(accepted, accepted, state, rejected);
    dfa.forAll(rejected, rejected);

    return dfa;
}

RANGEAND::RANGEAND(int min, int max, char state, char next) : min(min), max(max), state(state), next(next) {}

TempDFA RANGEAND::generateTempDFA(const char def) {
    return TempDFA();
}

RANGEOR::RANGEOR(int min, int max, char state, char next) : min(min), max(max), state(state), next(next) {}

TempDFA RANGEOR::generateTempDFA(const char def) {
    return TempDFA();
}

RANGEAMOUNT::RANGEAMOUNT(int min, int max, char state, char next) : min(min), max(max), state(state), next(next) {}

TempDFA RANGEAMOUNT::generateTempDFA(const char def) {
    return TempDFA();
}

AND::AND(Statement *first, Statement *second) : first(first), second(second) {}

AND::~AND() {
    delete first;
    delete second;
}

TempDFA AND::generateTempDFA(const char def) {
    return TempDFA();
}

OR::OR(Statement *first, Statement *second) : first(first), second(second) {}

OR::~OR() {
    delete first;
    delete second;
}

TempDFA OR::generateTempDFA(const char def) {
    return TempDFA();
}

NOT::NOT(Statement *statement) : statement(statement) {}

NOT::~NOT() {
    delete statement;
}

TempDFA NOT::generateTempDFA(const char def) {
    return TempDFA();
}

Statement::~Statement() {

}

void TempDFA::print(const std::string &fileName, const StateMap &stateMap) const {
    std::ofstream wFile(fileName + ".dot");
    if (!wFile.is_open()) {
        std::cerr << "Error opening file " + fileName + ".dot\n";
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
            std::string labels = std::string(1, arrow.second[0]);
            for (unsigned int i = 1; i < arrow.second.size(); ++i) {
                labels += ", " + std::string(1, arrow.second[i]);
            }
            wFile << "\t\"" + std::to_string(intptr_t(state)) + "\" -> \"" + arrow.first + "\" [ label = \"" + labels +
                     "\" ];\n";
        }
    }

    wFile << "}";
    wFile.close();
    const std::string command = "dot -Tpng " + fileName + ".dot -o" + fileName + ".png\n";
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
