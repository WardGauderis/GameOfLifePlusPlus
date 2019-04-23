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
#include "../automata/tm.h"
#include "../automata/pa.h"
#include "json/json.h"

using nlohmann::json;

class Parser
{
public:
    static Automaton* parseAutomaton(const std::string& path)
    {
        std::ifstream file(path);
        if(!file.is_open()) throw std::runtime_error("could not find specified file: " + path);
        auto json = json::parse(file);

        std::string type = json["type"];
        if     (type == "fa" ) return parseFA(path);
        else if(type == "pda") return parsePDA(path);
        else if(type == "tm" ) return parseTM(path);
        else if(type == "pa") return parsePA(path);
        else throw std::runtime_error("unknown automaton type");
    }

    static FA* parseFA(const std::string& path)
    {
        std::ifstream file(path);
        if(!file.is_open()) throw std::runtime_error("could not find specified file: " + path);
        auto json = json::parse(file);

        std::vector<char>   alphabet    = parseAlphabet(json["alphabet"], json["eps"]);
        std::vector<const State*> states = parseStates(json["states"]);
        FATransition          transition  = parseFATransitions(json["transitions"], json["states"], states);

        return new FA{alphabet, states, transition, json["type"] };
    }

    static PDA* parsePDA(const std::string& path)
    {
        std::ifstream file(path);
        if(!file.is_open()) throw std::runtime_error("could not find specified file: " + path);
        auto json = json::parse(file);

        std::vector<char>   alphabet        = parseAlphabet(json["alphabet"], json["eps"]);
        std::vector<char>   stackAlphabet   = parseAlphabet(json["stack_alphabet"], json["stack_start"]);
        std::vector<const State*> states    = parseStates(json["states"]);
        PDATransition       transition      = parsePDATransitions(json["transitions"], json["states"], states);

        return new PDA{alphabet, stackAlphabet, states, transition};
    }
    static TM* parseTM(const std::string& path)
    {
        std::ifstream file(path);
        if(!file.is_open()) throw std::runtime_error("could not find specified file: " + path);
        auto json = json::parse(file);

        std::vector<char> alphabet      = parseAlphabet(json["alphabet"], nullptr);
        std::vector<char> tapeAlphabet  = parseAlphabet(json["tape_alphabet"], nullptr);
        std::vector<TMState*> states    = parseTMStates(json["states"]);
        TMTransition transition         = parseTMTransitions(json["transitions"], json["states"], states);

        return new TM{alphabet, tapeAlphabet, states, transition};
    }
    static PA* parsePA(const std::string& path)
    {
        std::ifstream file(path);
        if(!file.is_open()) throw std::runtime_error("could not find specified file: " + path);
        auto json = json::parse(file);

        std::vector<char> alphabet       = parseAlphabet(json["alphabet"], nullptr);
        std::vector<const State*> states = parseStates(json["states"]);
        PATransition transition          = parsePATransitions(json["transitions"], json["states"], states);
        return new PA{alphabet, states, transition};
    }

private:
    static std::vector<char> parseAlphabet(const json& alphabet_values, const json& epsilon)
    {
        std::vector<char> alphabet(alphabet_values.size());
        for(uint32_t i = 0; i < alphabet.size(); i++) alphabet[i] = static_cast<std::string>(alphabet_values[i])[0];
        if(epsilon != nullptr) alphabet.push_back(static_cast<std::string>(epsilon)[0]);
        return alphabet;
    }

    static std::vector<const State*> parseStates(const json& state_values)
    {
        std::vector<const State*> states;
        states.resize(state_values.size());
        for(uint32_t i = 0; i < state_values.size(); i++)
        {
            states[i] = new State{state_values[i]["name"], state_values[i]["starting"], state_values[i]["accepting"], i};
        }
        return states;
    }

    static std::vector<TMState*> parseTMStates(const json& state_values)
    {
        std::vector<TMState*> states(state_values.size());
        for(uint32_t i = 0; i < states.size(); i++)
        {
            states[i] = new TMState;
            states[i]->name = state_values[i]["name"];

            const std::string& type = state_values[i]["type"];
            if     (type == "accepting") states[i]->type = TMState::Type::accept;
            else if(type == "normal"   ) states[i]->type = TMState::Type::normal;
            else if(type == "rejecting") states[i]->type = TMState::Type::reject;
            else if(type == "starting" ) states[i]->type = TMState::Type::start ;
            else throw std::runtime_error("unknown TM type\n");
        }
        return states;
    }

    static FATransition parseFATransitions(const json& transition_values, const json& state_values, const std::vector<const State*>& states)
    {
        FATransition result;

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

    static PDATransition parsePDATransitions(const json& transition_values, const json& state_values, const std::vector<const State*>& states)
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
            result(input, from) = {to, push, pop};
        }
        return result;
    }
    static TMTransition parseTMTransitions(const json& transition_values, const json& state_values, const std::vector<TMState*>& states)
    {
        TMTransition result;

        std::map<std::string, uint32_t> converter;
        for(uint32_t i = 0; i < state_values.size(); i++) converter[ state_values[i]["name"] ] = i;

        for(const auto& transition : transition_values)
        {
            TMState const* from = states[converter[transition["from"]]];
            TMState const* to   = states[converter[transition["to"  ]]];
            char input = static_cast<std::string> (transition["input"    ])[0];
            char dir   = static_cast<std::string> (transition["direction"])[0];
            char write = static_cast<std::string> (transition["write"    ])[0];
            result(input, from) = {to, dir, write};
        }
        return result;
    }

    static PATransition parsePATransitions(const json& transition_values, const json& state_values, const std::vector<const State*>& states)
    {
        PATransition result;

        std::map<std::string, uint32_t> converter;
        for(uint32_t i = 0; i < state_values.size(); i++) converter[ state_values[i]["name"] ] = i;

        for(const auto& transition : transition_values)
        {
            State const* from  = states[converter[transition["from"]]];
            State const* to    = states[converter[transition["to"  ]]];
            double probability = transition["probability"];
            char c = static_cast<std::string> (transition["input"])[0];
            result(c, from).emplace_back(probability, to);
        }
        return result;
    }
};
