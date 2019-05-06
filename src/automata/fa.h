//============================================================================
// @name        : FA.h
// @author      : Thomas Dooms
// @date        : 3/16/19
// @version     : 
// @copyright   : BA1 Informatica - Thomas Dooms - University of Antwerp
// @description : 
//============================================================================

#pragma once

#include <vector>
#include <set>
#include <fstream>

#include "states.h"
#include "transitions.h"
#include "automaton.h"

class FA : public Automaton
{
public:
    FA(const std::vector<char>& alphabet, const std::vector<const State*>& states, const FATransition& transition, const std::string& type);
    ~FA() override;

    bool operator()(const std::string& word) const override;
    void ecloseCurrent(std::set<const State*>& current) const;

    void dot(const std::string& name) const;
    static const FA* minimize(const FA& fa);
    static std::string makeName(const std::set<const State*>& states);

    static std::vector<std::vector<bool>> createTable(const FA& fa);
    static bool fillTable(std::vector<std::vector<bool>>& table, const FA& fa);

private:
    std::vector<char> alphabet;
    std::vector<const State*> states;
    const State* start;
    FATransition transition;
    std::string type;
};





