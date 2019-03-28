//============================================================================
// @name        : parser.h
// @author      : Thomas Dooms
// @date        : 3/16/19
// @version     : 
// @copyright   : BA1 Informatica - Thomas Dooms - University of Antwerp
// @description : 
//============================================================================

#pragma once

#include "../automata/fa.h"
#include "../automata/pda.h"
#include "json/json.h"

using nlohmann::json;

class Parser
{
public:
    static FA parseFA(const std::string& path)
    {
        std::ifstream file(path);
        if(!file.is_open()) throw std::runtime_error("could not find specified file: " + path);
        auto json = json::parse(file);

        std::vector<char>   alphabet    = parseAlphabet(json["alphabet"], json["eps"]);
        std::vector<State*> states      = parseStates(json["states"]);
        Transition          transition  = parseFATransitions(json["transitions"], json["states"], states);

        return {alphabet, states, transition, json["type"] };
    }

    static PDA parsePDA(const std::string& path)
    {
        std::ifstream file(path);
        if(!file.is_open()) throw std::runtime_error("could not find specified file: " + path);
        auto json = json::parse(file);

        std::vector<char>   alphabet        = parseAlphabet(json["alphabet"], json["eps"]);
        std::vector<char>   stackAlphabet   = parseAlphabet(json["stack_alphabet"], json["stack_start"]);
        std::vector<State*> states          = parseStates(json["states"]);
        PDATransition       transition      = parsePDATransitions(json["transitions"], json["states"], states);

        return {alphabet, stackAlphabet, states, transition};
    }

private:
    static std::vector<char> parseAlphabet(const json& alphabet_values, const json& epsilon)
    {
        std::vector<char> alphabet(alphabet_values.size());
        for(uint32_t i = 0; i < alphabet.size(); i++) alphabet[i] = static_cast<std::string>(alphabet_values[i])[0];
        if(epsilon != nullptr) alphabet.push_back(static_cast<std::string>(epsilon)[0]);
        return alphabet;
    }
    static std::vector<State*> parseStates(const json& state_values)
    {
        std::vector<State*> states(state_values.size());
        for(uint32_t i = 0; i < states.size(); i++)
        {
            states[i] = new State;
            states[i]->name      = state_values[i]["name"];
            states[i]->starting  = state_values[i]["starting"];
            states[i]->accepting = state_values[i]["accepting"];
            states[i]->index     = i;
        }
        return states;
    }
    static Transition parseFATransitions(const json& transition_values, const json& state_values, const std::vector<State*>& states)
    {
        Transition result;

        std::map<std::string, uint32_t> converter;
        for(uint32_t i = 0; i < state_values.size(); i++) converter[ state_values[i]["name"] ] = i;

        for(const auto& transition : transition_values)
        {
            State const* from = states[converter[transition["from"]]];
            State const* to   = states[converter[transition["to"  ]]];
            char c = static_cast<std::string> (transition["input"])[0];
            result(c, from).push_back(to);
        }
        return result;
    }

    static PDATransition parsePDATransitions(const json& transition_values, const json& state_values, const std::vector<State*>& states)
    {
        PDATransition result;

        std::map<std::string, uint32_t> converter;
        for(uint32_t i = 0; i < state_values.size(); i++) converter[ state_values[i]["name"] ] = i;

        for(const auto& transition : transition_values)
        {
            State const* from = states[converter[transition["from"]]];
            State const* to   = states[converter[transition["to"  ]]];
            char input = static_cast<std::string> (transition["input"])[0];
            char push  = static_cast<std::string> (transition["push" ])[0];
            char pop   = static_cast<std::string> (transition["pop"  ])[0];
            result(input, from).emplace_back(to, push, pop);
        }
        return result;
    }
};
