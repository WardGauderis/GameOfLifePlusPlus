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
#include <iostream>
#include "states.h"
#include "automaton.h"

template<typename Key, typename Value>
struct Transition
{
    Transition() = default;
    explicit Transition(const Value& empty) : empty(empty) {}

    // returns value of certain key, if not found, returns default
    const Value& operator[](const Key& key) const
    {
        const auto iter = map.find(key);
        if(iter == end(map)) return empty;
        else return iter->second;
    }

    // creates a new element if not found, replaces it if not unique
    Value& operator[](const Key& key)
    {
        return map[key];
    }

    // creates a new element if not found, throws error if not unique
    Value& find_if_unique(const Key& key)
    {
        if(map.find(key) != end(map)) throw std::runtime_error("element already found in transition");
        return map[key];
    }

    // insert if it doesn't exist yet
    void insert_if_not_found(const Key& key, const Value& value)
    {
        if(map.find(key) != end(map)) return;
        else map[key] = value;
    }

    const std::map<Key, Value>& getMap() const { return map; }
    std::map<Key, Value>& getMap() { return map; }

    std::map<Key, Value> map;
    Value empty;
};

typedef Transition<std::pair<char, const State*>, const State*> DFATransition;
typedef Transition<std::pair<char, const State*>, std::vector<const State*>> NFATransition;
typedef Transition<std::pair<char, const PDAState*>, std::tuple<const PDAState*, std::string, std::string>> PDATransition;
typedef Transition<std::pair<char, const TMState*>, std::tuple<const TMState*, char, char>> TMTransition;
typedef Transition<std::pair<char, const State*>, std::vector<std::pair<double, const State*>>> PATransition;
typedef Transition<std::pair<bool, const Automaton*>, const Automaton*> FSMTransition;
typedef Transition<std::pair<char, const StatePlusPlus *>, const StatePlusPlus *> DFAPlusPlusTransition;





