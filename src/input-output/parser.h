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
    static const Automaton* parseAutomaton(const std::string& path, const std::map<std::string, char>& alphabet)
    {
        std::ifstream file(path);
        if(!file.is_open()) throw std::runtime_error("could not find specified file: " + path);
        auto json = json::parse(file);

        std::string type = json["type"];
        if     (type == "dfa" or "nfa" or "enfa") return parseFA(path, alphabet);
        else if(type == "pda") return parsePDA(path, alphabet);
        else if(type == "tm" ) return parseTM(path, alphabet);
        else if(type == "pa") return parsePA(path, alphabet);
        else throw std::runtime_error("unknown automaton type");
    }

    static const FA* parseFA(const std::string& path, const std::map<std::string, char>& alphabet)
    {
        std::ifstream file(path);
        if(!file.is_open()) throw std::runtime_error("could not find specified file: " + path);
        auto json = json::parse(file);

        std::vector<const State*> states = parseStates(json["states"]);
        FATransition transition;
        if(json["type"] == "enfa"){ auto newAlphabet = alphabet; newAlphabet.emplace("~", '~');  transition = parseFATransitions(json["transitions"], json["states"], states, newAlphabet); }
        else transition = parseFATransitions(json["transitions"], json["states"], states, alphabet);

        auto chars = getCharacters(alphabet);
        if(json["type"] == "enfa") chars.push_back('~');

        auto temp = new FA{chars, states, transition, json["type"] };
        auto res = FA::minimize(*temp);

        system("[ -d \"output\" ] || mkdir output");
        temp->dot("output/test.dot");

        return res;
    }

    static const PDA* parsePDA(const std::string& path, const std::map<std::string, char>& alphabet)
    {
        std::ifstream file(path);
        if(!file.is_open()) throw std::runtime_error("could not find specified file: " + path);
        auto json = json::parse(file);

        std::vector<char> stackAlphabet     = parseAlphabet(json["stack_alphabet"], json["stack_eps"]);
        std::vector<const PDAState*> states = parsePDAStates(json["states"]);
        PDATransition transition            = parsePDATransitions(json["transitions"], json["states"], states, alphabet);

        return new PDA{getCharacters(alphabet), stackAlphabet, states, transition};
    }
    static const TM* parseTM(const std::string& path, const std::map<std::string, char>& alphabet)
    {
        std::ifstream file(path);
        if(!file.is_open()) throw std::runtime_error("could not find specified file: " + path);
        auto json = json::parse(file);

        std::vector<char> tapeAlphabet  = parseAlphabet(json["tape_alphabet"], nullptr);
        std::vector<TMState*> states    = parseTMStates(json["states"]);
        TMTransition transition         = parseTMTransitions(json["transitions"], json["states"], states, alphabet);

        return new TM{getCharacters(alphabet), tapeAlphabet, states, transition};
    }
    static const PA* parsePA(const std::string& path, const std::map<std::string, char>& alphabet)
    {
        std::ifstream file(path);
        if(!file.is_open()) throw std::runtime_error("could not find specified file: " + path);
        auto json = json::parse(file);

        std::vector<const State*> states = parseStates(json["states"]);
        PATransition transition          = parsePATransitions(json["transitions"], json["states"], states, alphabet);
        return new PA{getCharacters(alphabet), states, transition};
    }

private:
    static std::vector<char> getCharacters(const std::map<std::string, char>& alphabet)
    {
        std::vector<char> result;
        result.reserve(alphabet.size());
        for(const auto& value : alphabet) result.push_back(value.second);
        return result;
    }
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

    static std::vector<const PDAState*> parsePDAStates(const json& state_values)
    {
        std::vector<const PDAState*> states;
        states.reserve(state_values.size());
        for(const auto& value : state_values)
        {
            states.push_back(new PDAState{value["name"], value["starting"]});
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

    static FATransition parseFATransitions(const json& transition_values, const json& state_values, const std::vector<const State*>& states, const std::map<std::string, char>& alphabet)
    {
        FATransition result;

        std::map<std::string, uint32_t> converter;
        for(uint32_t i = 0; i < state_values.size(); i++) converter[ state_values[i]["name"] ] = i;

        for(const auto& transition : transition_values)
        {
            State const* from = states[converter[transition["from"]]];
            State const* to   = states[converter[transition["to"  ]]];
            char input = alphabet.at(transition["input"]);
            result(input, from).push_back(to);
        }
        return result;
    }

    static PDATransition parsePDATransitions(const json& transition_values, const json& state_values, const std::vector<const PDAState*>& states, const std::map<std::string, char>& alphabet)
    {
        PDATransition result;

        std::map<std::string, uint32_t> converter;
        for(uint32_t i = 0; i < state_values.size(); i++) converter[ state_values[i]["name"] ] = i;

        for(const auto& transition : transition_values)
        {
            PDAState const* from = states[converter[transition["from"]]];
            PDAState const* to   = states[converter[transition["to"  ]]];
            char input = alphabet.at(transition["input"]);
            char push  = static_cast<std::string> (transition["push" ])[0];
            char pop   = static_cast<std::string> (transition["pop"  ])[0];
            result(input, from) = {to, push, pop};
        }
        return result;
    }
    static TMTransition parseTMTransitions(const json& transition_values, const json& state_values, const std::vector<TMState*>& states, const std::map<std::string, char>& alphabet)
    {
        TMTransition result;

        std::map<std::string, uint32_t> converter;
        for(uint32_t i = 0; i < state_values.size(); i++) converter[ state_values[i]["name"] ] = i;

        for(const auto& transition : transition_values)
        {
            TMState const* from = states[converter[transition["from"]]];
            TMState const* to   = states[converter[transition["to"  ]]];
            char input = alphabet.at(transition["input"]);
            char dir   = static_cast<std::string> (transition["direction"])[0];
            char write = static_cast<std::string> (transition["write"    ])[0];
            result(input, from) = {to, dir, write};
        }
        return result;
    }

    static PATransition parsePATransitions(const json& transition_values, const json& state_values, const std::vector<const State*>& states, const std::map<std::string, char>& alphabet)
    {
        PATransition result;

        std::map<std::string, uint32_t> converter;
        for(uint32_t i = 0; i < state_values.size(); i++) converter[ state_values[i]["name"] ] = i;

        for(const auto& transition : transition_values)
        {
            char input = alphabet.at(transition["input"]);
            State const* from  = states[converter[transition["from"]]];
            State const* to    = states[converter[transition["to"  ]]];
            double probability = transition["probability"];
            result(input, from).emplace_back(probability, to);
        }
        return result;
    }
};
