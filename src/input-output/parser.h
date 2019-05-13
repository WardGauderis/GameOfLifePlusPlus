//============================================================================
// @name        : parser.h
// @author      : Thomas Dooms
// @date        : 3/16/19
// @version     : 
// @copyright   : BA1 Informatica - Thomas Dooms - University of Antwerp
// @description : 
//============================================================================

#pragma once

#include "../automata/dfa.h"
#include "../automata/nfa.h"
#include "../automata/pda.h"
#include "../automata/tm.h"
#include "../automata/pa.h"
#include "json/json.h"

using nlohmann::json;

class Parser
{
private:
    static auto openJson(const std::string& path)
    {
        std::ifstream file(path);
        if(!file.is_open()) throw std::runtime_error("could not find specified file: " + path);
        return json::parse(file);
    }

public:
    static const Automaton* parseAutomaton(const std::string& path, const std::map<std::string, char>& alphabet)
    {
        auto json = openJson(path);
        const Automaton* result;

        std::string type = json["type"];
        if     (type == "dfa") result = parseDFA(path, alphabet);
        else if(type == "nfa" or type == "enfa") result = DFA::SSC(parseNFA(path, alphabet));
        else if(type == "pda") result = parsePDA(path, alphabet);
        else if(type == "tm" ) result = parseTM(path, alphabet);
        else if(type == "pa") result = parsePA(path, alphabet);
        else throw std::runtime_error("unknown automaton type");
        result->dot(path);
        return result;
    }

    static const DFA* parseDFA(const std::string &path, const std::map<std::string, char> &alphabet)
    {
        auto json = openJson(path);
        checkJsonParams<2>(json, std::array<std::string, 2>{"states", "transitions"}, "main file", path);

        const std::vector<const State*> states = parseStates(json["states"]);
        DFATransition transition = parseDFATransitions(json["transitions"], json["states"], states, alphabet, path);
        return new DFA{getCharacters(alphabet), states, transition};
    }

    static const NFA* parseNFA(const std::string &path, const std::map<std::string, char> &alphabet)
    {
        auto json = openJson(path);
        checkJsonParams<2>(json, std::array<std::string, 2>{"states", "transitions"}, "main file", path);

        std::vector<const State*> states = parseStates(json["states"]);
        NFATransition transition;
        if(json["type"] == "enfa"){ auto newAlphabet = alphabet; newAlphabet.emplace("~", '~');  transition = parseNFATransitions(json["transitions"], json["states"], states, newAlphabet, path); }
        else transition = parseNFATransitions(json["transitions"], json["states"], states, alphabet, path);

        auto chars = getCharacters(alphabet);
        if(json["type"] == "enfa") chars.push_back('~');
        return new NFA{chars, states, transition, json["type"] };
    }

    static const PDA* parsePDA(const std::string& path, const std::map<std::string, char>& alphabet)
    {
        auto json = openJson(path);
        checkJsonParams<2>(json, std::array<std::string, 2>{"states", "transitions"}, "main file", path);

        std::vector<char> stackAlphabet     = parseAlphabet(json["stack_alphabet"], nullptr);
        std::vector<const PDAState*> states = parsePDAStates(json["states"]);
        PDATransition transition            = parsePDATransitions(json["transitions"], json["states"], states, alphabet, path);

        return new PDA{getCharacters(alphabet), stackAlphabet, states, transition};
    }

    static const TM* parseTM(const std::string& path, const std::map<std::string, char>& alphabet)
    {
        auto json = openJson(path);
        checkJsonParams<3>(json, std::array<std::string, 3>{"tape_alphabet", "states", "transitions"}, "main file", path);

        std::vector<char> tapeAlphabet  = parseAlphabet(json["tape_alphabet"], nullptr);
        std::vector<TMState*> states    = parseTMStates(json["states"]);
        TMTransition transition         = parseTMTransitions(json["transitions"], json["states"], states, alphabet, path);

        checkStartState(states, path);

        return new TM{getCharacters(alphabet), tapeAlphabet, states, transition};
    }

    static const PA* parsePA(const std::string& path, const std::map<std::string, char>& alphabet)
    {
        auto json = openJson(path);
        checkJsonParams<2>(json, std::array<std::string, 2>{"states", "transitions"}, "main file", path);

        std::vector<const State*> states = parseStates(json["states"]);
        PATransition transition          = parsePATransitions(json["transitions"], json["states"], states, alphabet, path);
        return new PA{getCharacters(alphabet), states, transition};
    }

private:
    template<uint32_t N>
    static void checkJsonParams(const json& json, const std::array<std::string, N>& list, const std::string& origin, const std::string& path)
    {
        for( const auto& word : list)
        {
            auto iter = json.find(word);
            if( iter == end(json)) throw std::runtime_error("could not find parameter " + word + " in " + origin + " located in file " + path);
        }
    }

    template<typename T>
    static void checkStartState(const std::vector<T>& states, const std::string& path)
    {
        const auto iter = std::find_if(begin(states), end(states), [](const auto& state){ return state->start; });
        if(iter == end(states)) throw std::runtime_error("could not find start state located in file " + path);
    }
    static void checkStartState(const std::vector<const TMState*>& states, const std::string& path)
    {
        const auto iter = std::find_if(begin(states), end(states), [](const auto& state){ return state->type == TMState::Type::start; });
        if(iter == end(states)) throw std::runtime_error("could not find start state located in file " + path);
    }


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

    static DFATransition parseDFATransitions(const json &transition_values, const json &state_values,const std::vector<const State*> &states,const std::map<std::string, char> &alphabet, const std::string& path)
    {
        DFATransition result(nullptr);

        std::map<std::string, uint32_t> converter;
        for(uint32_t i = 0; i < state_values.size(); i++) converter[ state_values[i]["name"] ] = i;

        for(const auto& transition : transition_values)
        {
            checkJsonParams<3>(transition, std::array<std::string, 3>{"from", "to", "input"}, "transitions" , path);
            const std::string& fromName = transition["from"];
            const std::string& toName = transition["to"];
            const std::string& inputName = transition["input"];

            try
            {
                const State* from = states[converter.at(fromName)];
                const State* to   = states[converter.at(toName)];
                char input = alphabet.at(inputName);
                result.find_if_unique({input, from}) = to;
            }
            catch(std::exception& e)
            {
                throw std::runtime_error("something went wrong from parsing the transition of state " + fromName + " to state " + toName + " please check all parameters and check for accidental duplicates\n");
            }

        }
        return result;
    }

    static NFATransition parseNFATransitions(const json &transition_values, const json &state_values,const std::vector<const State *> &states,const std::map<std::string, char> &alphabet, const std::string& path)
    {
        NFATransition result;

        std::map<std::string, uint32_t> converter;
        for(uint32_t i = 0; i < state_values.size(); i++) converter[ state_values[i]["name"] ] = i;

        for(const auto& transition : transition_values)
        {
            checkJsonParams<3>(transition, std::array<std::string, 3>{"from", "to", "input"}, "transitions" , path);
            const std::string& fromName = transition["from"];
            const std::string& toName = transition["to"];
            const std::string& inputName = transition["input"];
            try
            {
                State const* from = states[converter.at(fromName)];
                State const* to   = states[converter.at(toName)];
                char input = alphabet.at(inputName);
                result[{input, from}].push_back(to);
            }
            catch(std::exception& e)
            {
                throw std::runtime_error("something went wrong from parsing the transition of state " + fromName + " to state " + toName + " please check all parameters\n");
            }
        }
        return result;
    }

    static PDATransition parsePDATransitions(const json& transition_values, const json& state_values, const std::vector<const PDAState*>& states, const std::map<std::string, char>& alphabet, const std::string& path)
    {
        PDATransition result;

        std::map<std::string, uint32_t> converter;
        for(uint32_t i = 0; i < state_values.size(); i++) converter[ state_values[i]["name"] ] = i;

        for(const auto& transition : transition_values)
        {
            checkJsonParams<3>(transition, std::array<std::string, 3>{"from", "to", "input"}, "transitions" , path);
            const std::string& fromName = transition["from"];
            const std::string& toName = transition["to"];
            const std::string& inputName = transition["input"];

            try
            {
                PDAState const* from = states[converter.at(fromName)];
                PDAState const* to   = states[converter.at(toName)];
                char input = alphabet.at(inputName);
                const std::string& push  = static_cast<std::string> (transition["push" ]);
                const std::string& pop   = static_cast<std::string> (transition["pop"  ]);

                result.find_if_unique({input, from}) = {to, push, pop};
            }
            catch(std::exception& e)
            {
                throw std::runtime_error("something went wrong from parsing the transition of state " + fromName + " to state " + toName + " please check all parameters\n");
            }
        }
        return result;
    }
    static TMTransition parseTMTransitions(const json& transition_values, const json& state_values, const std::vector<TMState*>& states, const std::map<std::string, char>& alphabet, const std::string& path)
    {
        TMTransition result;

        std::map<std::string, uint32_t> converter;
        for(uint32_t i = 0; i < state_values.size(); i++) converter[ state_values[i]["name"] ] = i;

        for(const auto& transition : transition_values)
        {
            checkJsonParams<3>(transition, std::array<std::string, 3>{"from", "to", "input"}, "transitions" , path);
            TMState const* from = states[converter[transition["from"]]];
            TMState const* to   = states[converter[transition["to"  ]]];
            char input = alphabet.at(transition["input"]);
            char dir   = static_cast<std::string> (transition["direction"])[0];
            char write = static_cast<std::string> (transition["write"    ])[0];
            result.find_if_unique({input, from}) = {to, dir, write};
        }
        return result;
    }

    static PATransition parsePATransitions(const json& transition_values, const json& state_values, const std::vector<const State*>& states, const std::map<std::string, char>& alphabet, const std::string& path)
    {
        PATransition result;

        std::map<std::string, uint32_t> converter;
        for(uint32_t i = 0; i < state_values.size(); i++) converter[ state_values[i]["name"] ] = i;

        for(const auto& transition : transition_values)
        {
            checkJsonParams<3>(transition, std::array<std::string, 3>{"from", "to", "input"}, "transitions" , path);
            State const* from  = states[converter[transition["from"]]];
            State const* to    = states[converter[transition["to"  ]]];
            char input = alphabet.at(transition["input"]);
            double probability = 1;
            if(transition.find("probability") != end(transition)) probability = transition["probability"];
            result[{input, from}].emplace_back(probability, to);
        }
        return result;
    }
};
