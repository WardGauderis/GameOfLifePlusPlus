//============================================================================
// @name        : transition.h
// @author      : Thomas Dooms
// @date        : 3/16/19
// @version     : 
// @copyright   : BA1 Informatica - Thomas Dooms - University of Antwerp
// @description : 
//============================================================================

#pragma once

#include <map>
#include <vector>
#include "states.h"
#include "automaton.h"

struct FATransition
{
public:
    std::vector<const State*>& operator()(char c, const State* state)
    {
        return transition[ {state, c} ];
    }
    const std::vector<const State*>& operator()(char c, const State* state) const
    {
        try{ return transition.at( {state, c} ); }
        catch(std::exception& e){ return empty; }
    }
    const std::map<std::pair<const State*, char>, std::vector<const State*>>& getMap() const
    {
        return transition;
    }
    std::map<std::pair<const State*, char>, std::vector<const State*>>& getMap()
    {
        return transition;
    }
private:
    // this is a map with key a {state and a char}, and a value that contains the vector of transitions from that state for that character
    std::map<std::pair<const State*, char>, std::vector<const State*>> transition;
    static const std::vector<const State*> empty;
};

const std::vector<const State*> FATransition::empty = {};



struct PDATransition
{
public:
    std::tuple<const State*, char, char>& operator()(char c, const State* state)
    {
        return transition[ {state, c} ];
    }
    const std::tuple<const State*, char, char>& operator()(char c, const State* state) const
    {
        try{ return transition.at( {state, c} ); }
        catch(std::exception& e){ return empty; }
    }
    const std::map<std::pair<const State*, char>, std::tuple<const State*, char, char>>& getMap() const
    {
        return transition;
    }
    std::map<std::pair<const State*, char>, std::tuple<const State*, char, char>>& getMap()
    {
        return transition;
    }
private:
    // this is a map with key a {state and a char}, and a value that contains the vector of transitions from that state for that character
    std::map<std::pair<const State*, char>, std::tuple<const State*, char, char>> transition;
    static const std::tuple<const State*, char, char> empty;
};

const std::tuple<const State*, char, char> PDATransition::empty = {};


struct TMTransition
{
public:
    std::tuple<const TMState*, char, char>& operator()(char c, const TMState* state)
    {
        return transition[ {state, c} ];
    }
    const std::tuple<const TMState*, char, char>& operator()(char c, const TMState* state) const
    {
        try{ return transition.at( {state, c} ); }
        catch(std::exception& e){ return empty; }
    }
    const std::map<std::pair<const TMState*, char>, std::tuple<const TMState*, char, char>>& getMap() const
    {
        return transition;
    }
    std::map<std::pair<const TMState*, char>, std::tuple<const TMState*, char, char>>& getMap()
    {
        return transition;
    }
private:
    // this is a map with key a {state and a char}, and a value that contains the vector of transitions from that state for that character
    std::map<std::pair<const TMState*, char>, std::tuple<const TMState*, char, char>> transition;
    static const std::tuple<const TMState*, char, char> empty;
};

const std::tuple<const TMState*, char, char> TMTransition::empty = {};


struct FSMTransition
{
public:
    const Automaton *&operator()(bool b, const Automaton* state)
    {
        return transition[ {state, b} ];
    }
    const Automaton *const &operator()(bool b, const Automaton* state) const
    {
        try{ return transition.at( {state, b} ); }
        catch(std::exception& e){ return empty; }
    }
    const std::map<std::pair<const Automaton*, bool>, const Automaton*>& getMap() const
    {
        return transition;
    }
    std::map<std::pair<const Automaton*, bool>, const Automaton*>& getMap()
    {
        return transition;
    }
private:
    // this is a map with key a {state and a char}, and a value that contains the transition from that state for that character
    std::map<std::pair<const Automaton*, bool>, const Automaton*> transition;
    static const Automaton* empty;
};

const Automaton* FSMTransition::empty = nullptr;


struct PATransition
{
public:
    std::vector<std::pair<double, const State*>>& operator()(char c, const State* state)
    {
        return transition[ {state, c} ];
    }
    const std::vector<std::pair<double, const State*>>& operator()(char c, const State* state) const
    {
        try{ return transition.at( {state, c} ); }
        catch(std::exception& e){ return empty; }
    }
    const std::map<std::pair<const State*, char>, std::vector<std::pair<double, const State*>>>& getMap() const
    {
        return transition;
    }
    std::map<std::pair<const State*, char>, std::vector<std::pair<double, const State*>>>& getMap()
    {
        return transition;
    }
private:
    // this is a map with key a {state and a char}, and a value that contains the vector of transitions from that state for that character
    std::map<std::pair<const State*, char>, std::vector<std::pair<double, const State*>>> transition;
    static const std::vector<std::pair<double, const State*>> empty;
};

const std::vector<std::pair<double, const State*>> PATransition::empty = {};

struct DFAPlusPlusTransition {
public:
    const StatePlusPlus *&operator()(char c, const StatePlusPlus *state) {
        return transition[{state, c}];
    }

    const StatePlusPlus *const &operator()(char c, const StatePlusPlus *state) const {
        try { return transition.at({state, c}); }
        catch (std::exception &e) { return empty; }
    }

    const std::map<std::pair<const StatePlusPlus *, char>, const StatePlusPlus *> &getMap() const {
        return transition;
    }

    std::map<std::pair<const StatePlusPlus *, char>, const StatePlusPlus *> &getMap() {
        return transition;
    }

private:
    // this is a map with key a {state and a char}, and a value that contains the transition from that state for that character
    std::map<std::pair<const StatePlusPlus *, char>, const StatePlusPlus *> transition;
    static const StatePlusPlus* empty;
};

const StatePlusPlus* DFAPlusPlusTransition::empty = nullptr;








