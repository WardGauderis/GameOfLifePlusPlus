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

struct Transition
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
    const std::vector<const State*> empty;
};



struct PDATransition
{
public:
    std::vector<std::tuple<const State*, char, char>>& operator()(char c, const State* state)
    {
        return transition[ {state, c} ];
    }
    const std::vector<std::tuple<const State*, char, char>>& operator()(char c, const State* state) const
    {
        try{ return transition.at( {state, c} ); }
        catch(std::exception& e){ return empty; }
    }
    const std::map<std::pair<const State*, char>, std::vector<std::tuple<const State*, char, char>>>& getMap() const
    {
        return transition;
    }
    std::map<std::pair<const State*, char>, std::vector<std::tuple<const State*, char, char>>>& getMap()
    {
        return transition;
    }
private:
    // this is a map with key a {state and a char}, and a value that contains the vector of transitions from that state for that character
    std::map<std::pair<const State*, char>, std::vector<std::tuple<const State*, char, char>>> transition;
    const std::vector<std::tuple<const State*, char, char>> empty;
};


struct TMTransition
{
public:
    std::vector<std::tuple<const State*, char, char>>& operator()(char c, const State* state)
    {
        return transition[ {state, c} ];
    }
    const std::vector<std::tuple<const State*, char, char>>& operator()(char c, const State* state) const
    {
        try{ return transition.at( {state, c} ); }
        catch(std::exception& e){ return empty; }
    }
    const std::map<std::pair<const State*, char>, std::vector<std::tuple<const State*, char, char>>>& getMap() const
    {
        return transition;
    }
    std::map<std::pair<const State*, char>, std::vector<std::tuple<const State*, char, char>>>& getMap()
    {
        return transition;
    }
private:
    // this is a map with key a {state and a char}, and a value that contains the vector of transitions from that state for that character
    std::map<std::pair<const State*, char>, std::vector<std::tuple<const State*, char, char>>> transition;
    const std::vector<std::tuple<const State*, char, char>> empty;
};
