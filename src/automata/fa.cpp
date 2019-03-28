//============================================================================
// @name        : FA.cpp
// @author      : Thomas Dooms
// @date        : 3/16/19
// @version     : 
// @copyright   : BA1 Informatica - Thomas Dooms - University of Antwerp
// @description : 
//============================================================================

#include "fa.h"
#include "json/json.h"
#include <algorithm>
#include <iostream>
#include <queue>


FA::FA(const std::vector<char>& alphabet, const std::vector<State*>& states, const Transition& transition, const std::string& type) : alphabet(alphabet), states(states), transition(transition), type(type)
{
    start = *std::find_if(begin(states), end(states), [](const auto& state){ return state->starting; });
}
FA::~FA()
{
    for(State* state : states) delete state;
}

bool FA::operator()(const std::string& word) const
{
    std::set<const State*> current = {start};
    if(type == "ENFA") ecloseCurrent(current);
    for(const char c : word)
    {
        std::set<const State*> newStates;
        for(const State* state : current)
        {
            const auto temp = transition(c, state);
            newStates.insert(begin(temp), end(temp));
        }
        current.clear();
        current = newStates;
        if(type == "ENFA") ecloseCurrent(current);
    }
    return std::any_of(begin(current), end(current), [](const auto& state){ return state->accepting; });
}

void FA::ecloseCurrent(std::set<const State*>& current) const
{
    if(type != "ENFA") return;
    char epsilon = alphabet.back();
    for(const State* state : current)
    {
        const auto temp = transition(epsilon, state);
        current.insert(begin(temp), end(temp));
    }
}

//============================================================================//

void FA::dot(const std::string& path) const
{
    std::ofstream file(path);

    file << "digraph G {\n";
    file << "NULL -> " << start->name << ";\n";
    file << "NULL [style=invis];\n";

    for(const State* state: states)
    {
        for(const char c : alphabet)
        {
            for(const auto& newState : transition(c, state)) file << state->name << " -> " << newState->name << "[label=\"" << c << "\"];\n";
        }
    }
    for(const State* state : states)
    {
        if(state->accepting) file << state->name << "[peripheries=2];\n";
    }

    file << "}" << std::endl; // Flushing is very important.
    std::string pngPath = path.substr(0, path.size() - 3) + "png";
    system(("dot -Tpng " + path + " -o " + pngPath).c_str());
}

void FA::json(const std::string& name) const
{
    nlohmann::json json;
    json["type"] = type;
    if(type == "ENFA")
    {
        for(uint32_t i = 0; i < alphabet.size() - 1; i++) json["alphabet"].push_back(std::string(1, alphabet[i]));
        json["eps"] = std::string(1, alphabet.back());
    }
    else for(const char c : alphabet) json["alphabet"].push_back(std::string(1, c));

    for(uint32_t i = 0; i < states.size(); i++)
    {
        json["states"][i]["name"     ] = states[i]->name;
        json["states"][i]["accepting"] = states[i]->accepting;
        json["states"][i]["starting" ] = states[i]->starting;
    }
    uint32_t iter = 0;
    for(const State* state : states)
        for(const char c : alphabet)
        {
            const std::vector<const State*> nextVector = transition(c, state);
            for(const State* next : nextVector)
            {
                json["transitions"][iter]["from" ] = state->name;
                json["transitions"][iter]["to"   ] = next ->name;
                json["transitions"][iter]["input-output"] = std::string(1,c);
                iter++;
            }
        }

    std::ofstream file("output/" + name + ".json");
    file << json;
}

//============================================================================

FA FA::SSC(const FA& fa)
{
    std::map<std::set<const State*>, const State*> existing;
    std::queue<std::set<const State*>> queue;
    uint32_t index = 0;

    std::vector<State*> newStates;
    Transition newTransition;
    std::set<const State*> startStates = {fa.start};
    fa.ecloseCurrent(startStates);

    State* newState = new State{makeName(startStates), true, fa.start->accepting, 0};
    existing.insert({startStates, newState});
    newStates.push_back(newState);
    queue.push(startStates);

    while(!queue.empty())
    {
        for(const char c : fa.alphabet)
        {
            if(fa.type == "ENFA" and c == fa.alphabet.back()) continue;

            std::set<const State*> nextStates;
            for(const State* state : queue.front())
                for(const State* next : fa.transition(c, state))
                    nextStates.insert(next);

            fa.ecloseCurrent(nextStates);
            const auto& iter = existing.find(nextStates);
            if(iter != end(existing))
            {
                newTransition(c, newStates[index]).push_back(iter->second);
                continue;
            }

            bool accepting = *std::find_if(begin(nextStates), end(nextStates), [](const auto& state){ return state->accepting; });
            State* newState = new State{makeName(nextStates), false, accepting, static_cast<uint32_t>(newStates.size())};
            newStates.push_back(newState);

            newTransition(c, newStates[index]).push_back(newState);
            existing.insert({nextStates, newState});
            queue.push(nextStates);
        }
        index++;
        queue.pop();
    }

    return {fa.alphabet, newStates, newTransition, "DFA"};
}

std::string FA::makeName(const std::set<const State*>& states)
{
    if(states.empty()) return "\"{}\"";
    std::string result = "\"{";
    for(const State* state : states) result += state->name + ',';
    result.back() = '}';
    return result + '\"';
}

//============================================================================//

FA FA::product(const FA& a, const FA& b, bool isAnd)
{
    assert(a.alphabet == b.alphabet);
    std::string type = "DFA";
    if(a.type == "NFA"  or b.type == "NFA" ) type = "NFA" ;
    if(a.type == "ENFA" or b.type == "ENFA") type = "ENFA";

    const auto asize = static_cast<uint32_t>(a.states.size());
    const auto bsize = static_cast<uint32_t>(b.states.size());
    std::vector<State*> newStates(asize * bsize);
    Transition newTransition;

    for(auto& newState : newStates) newState = new State;

    for(uint32_t i = 0; i < a.states.size(); i++)
        for(uint32_t j = 0; j < b.states.size(); j++)
        {
            auto const state = newStates[i*bsize + j];
            state->name      = "\"(" + a.states[i]->name + ',' +  b.states[j]->name + ")\"";
            state->starting  = a.states[i]->starting  and b.states[j]->starting ;
            if(isAnd) state->accepting = a.states[i]->accepting and b.states[j]->accepting;
            else      state->accepting = a.states[i]->accepting or  b.states[j]->accepting;
            state->index     = i*asize + j;

            for(const char c : a.alphabet)
            {
                const uint32_t startIndex = a.states[i]->index*bsize + b.states[j]->index;
                const std::vector<const State*> astates = a.transition(c, a.states[i]);
                const std::vector<const State*> bstates = b.transition(c, b.states[j]);

                for(const State* astate : astates)
                    for(const State* bstate : bstates)
                    {
                        const uint32_t endIndex = astate->index*bsize + bstate->index;
                        newTransition(c, newStates[startIndex]).push_back(newStates[endIndex]);
                    }

            }
        }
    return {a.alphabet, newStates, newTransition, type};
}

FA FA::minimize(const FA& fa, const std::string& path)
{
    HtmlExporter exporter(path);
    exporter.start();

    const auto size = static_cast<uint32_t>(fa.states.size());
    auto table = createTable(fa, exporter);

    std::vector<State*> newStates;
    std::map<const State*, const State*> converter;
    std::vector<bool> skip(size, false);

    for(uint32_t i = 0; i < size; i++)
    {
        if(skip[i]) continue;
        newStates.push_back(new State(*fa.states[i]));
        converter[fa.states[i]] = newStates.back();
        std::set<const State*> addedStates = {fa.states[i]};

        for(uint32_t j = 0; j < i; j++)
            if(!table[i][j])
            {
                addedStates.insert(fa.states[j]);
                if(fa.states[j]->starting) newStates.back()->starting = true;
                converter[fa.states[j]] = newStates.back();
                skip[j] = true;
            }
        newStates.back()->name = makeName(addedStates);
    }

    Transition newTransition;
    for(auto& element : fa.transition.getMap())
    {
        auto& temp = newTransition(element.first.second, converter[element.first.first]);
        if(temp.empty()) temp.push_back(converter[element.second[0]]);
    }

    exporter.finish();
    return {fa.alphabet, newStates, newTransition, "DFA"};
}

void FA::compare(const FA& a, const FA& b, const std::string& path)
{
    HtmlExporter exporter(path);
    exporter.start();

    std::vector<State*> newStates = a.states;
    uint32_t startA = a.start->index;
    uint32_t startB = b.start->index + (uint32_t)a.states.size();
    Transition newTransition;

    newTransition.getMap() = a.transition.getMap();
    newTransition.getMap().insert(begin(b.transition.getMap()), end(b.transition.getMap()));
    newStates.insert(end(newStates), begin(b.states), end(b.states));
    for(uint32_t i = 0; i < newStates.size(); i++) newStates[i]->index = i;

    FA c = {a.alphabet, newStates, newTransition, "ENFA"};
    auto table = createTable(c, exporter);

    if(table[startB][startA] == 0) exporter.write("<li> They are equivalent </li>\n");
    else exporter.write("<li> They are not equivalent </li>\n");
    exporter.finish();

    c.states.clear();  // we need to clear the states otherwise pointer will be doubly deleted
}

std::vector<std::vector<bool>> FA::createTable(const FA& fa, HtmlExporter& exporter)
{
    const auto size = static_cast<uint32_t>(fa.states.size());
    std::vector<std::vector<bool>> table(size);
    for(auto& row : table) row = std::vector<bool>(size, false);

    for(const auto& state : fa.states)
    {
        if(!state->accepting) continue;
        const uint32_t i = state->index;
        for(uint32_t j = 0; j < size; j++)
        {
            table[i][j] = !table[i][j];
            table[j][i] = !table[j][i];
        }
    }

    bool done = false;
    while(!done)
    {
        exporter.writeTable(table, fa.states);
        done = fillTable(table, fa);
    }
    return table;
}

void FA::printTable(const std::vector<std::vector<bool>>& table, const FA& fa)
{
    for(uint32_t i = 1; i < fa.states.size(); i++)
    {
        std::cout << fa.states[i]->name << " | " ;
        for(uint32_t j = 0; j < i; j++)
        {
            if(table[i][j]) std::cout << "x | ";
            else std::cout << "  | ";
        }
        std::cout << '\n';
    }
    std::cout << "    ";
    for(uint32_t i = 0; i < fa.states.size()-1; i++) std::cout << fa.states[i]->name << "   ";
    std::cout << "\n\n";
}


bool FA::fillTable(std::vector<std::vector<bool>>& table, const FA& fa)
{
    bool done = true;
    for(uint32_t i = 0; i < fa.states.size(); i++)
        for(uint32_t j = 0; j < i; j++)
        {
            if(table[i][j]) continue;
            for(const char c : fa.alphabet)
            {
                const auto i1 = fa.transition(c, fa.states[i])[0]->index;
                const auto j1 = fa.transition(c, fa.states[j])[0]->index;
                if(table[i1][j1])
                {
                    done = false;
                    table[i][j] = true;
                    break;
                }
            }
        }
    return done;
}

