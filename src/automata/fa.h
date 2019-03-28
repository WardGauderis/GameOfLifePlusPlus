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
#include "HtmlExporter.h"

class FA
{
public:
    FA(const std::vector<char>& alphabet, const std::vector<State*>& states, const Transition& transition, const std::string& type);
    ~FA();

    bool operator()(const std::string& word) const;
    void ecloseCurrent(std::set<const State*>& current) const;

    //============================================================================//

    void dot(const std::string& name) const;
    void json(const std::string& name) const;

    //============================================================================//

    static FA SSC(const FA& fa);

    static std::string makeName(const std::set<const State*>& states);

    //============================================================================//

    static FA product(const FA& a, const FA& b, bool isAnd);
    static FA minimize(const FA& fa, const std::string& path);

    static void compare(const FA& a, const FA& b, const std::string& path);

    static std::vector<std::vector<bool>> createTable(const FA& fa, HtmlExporter& exporter);
    static void printTable(const std::vector<std::vector<bool>>& table, const FA& fa);
    static bool fillTable (      std::vector<std::vector<bool>>& table, const FA& fa);

private:
    std::vector<char> alphabet;
    std::vector<State*> states;
    const State* start;
    Transition transition;
    std::string type;
};





