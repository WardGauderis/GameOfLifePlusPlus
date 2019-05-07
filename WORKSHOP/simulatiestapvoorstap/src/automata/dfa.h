//============================================================================
// @name        : dfa.h
// @author      : Thomas Dooms
// @date        : 5/7/19
// @version     : 
// @copyright   : BA1 Informatica - Thomas Dooms - University of Antwerp
// @description : 
//============================================================================


#ifndef GOL_DFA_H
#define GOL_DFA_H

#include <vector>
#include <set>
#include <fstream>

#include "states.h"
#include "transitions.h"
#include "automaton.h"
#include "nfa.h"


class DFA : public Automaton
{
public:
    DFA(const std::vector<char>& alphabet, const std::vector<const State*>& states, const DFATransition& transition);
    ~DFA() override;

    bool operator()(const std::string& word) const override;
    void dot(const std::string& name) const;

    static DFA* SSC(const NFA* fa);

    static const DFA* minimize(const DFA* fa);
    static std::string makeName(const std::set<const State*>& states);

    static std::vector<std::vector<bool>> createTable(const DFA* fa);
    static bool fillTable(std::vector<std::vector<bool>>& table, const DFA* fa);

private:
    std::vector<char> alphabet;
    std::vector<const State*> states;
    const State* start;
    DFATransition transition;
};


#endif //GOL_DFA_H
