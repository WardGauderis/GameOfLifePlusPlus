//============================================================================
// @name        : DFA.h
// @author      : Ward Gauderis
// @date        : 5/4/19
// @version     : 
// @copyright   : BA1 Informatica - Ward Gauderis - University of Antwerp
// @description : 
//============================================================================
#ifndef TEST_DFA_H
#define TEST_DFA_H


#include <vector>
#include <set>
#include <map>

struct StatePlusPlus {
    std::string name;
    char type;

    StatePlusPlus(std::string name, char type) : name(std::move(name)), type(type) {}
};

struct DFAPlusPlusTransition {
public:
    const StatePlusPlus *&operator()(char c, const StatePlusPlus *state) {
        return transition[{state, c}];
    }

    const StatePlusPlus *const &operator()(char c, const StatePlusPlus *state) const {
        return transition.at(std::pair<const StatePlusPlus *, char>{state, c});
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
    const StatePlusPlus *empty = nullptr;
};


class DFAPlusPlus {
public:
    static std::vector<char> alphabet;

    static std::vector<StatePlusPlus *> states;

    static DFAPlusPlusTransition transition;

    char operator()(const std::string &word) const;

    char getCurrent() const;

    void TFAPlusPlus();

    DFAPlusPlus(const StatePlusPlus *current);

    void print(const std::string &fileName) const;

    DFAPlusPlus(const std::string &fileName);

private:
    mutable const StatePlusPlus *current;

    static StatePlusPlus *upgradeToMin(DFAPlusPlusTransition &minTransition, std::vector<StatePlusPlus *> &minStates,
                                       std::map<std::set<const StatePlusPlus *>, StatePlusPlus *> &minStatesMap,
                                       const std::set<const StatePlusPlus *> &minState);

};


#endif //TEST_DFA_H
