//============================================================================
// @name        : CAIO.cpp
// @author      : Ward Gauderis
// @date        : 4/23/19
// @version     : 
// @copyright   : BA1 Informatica - Ward Gauderis - University of Antwerp
// @description : 
//============================================================================
#include <fstream>
#include <iostream>
#include <sstream>
#include "caio.h"
#include "../automata/transitions.h"
#include "parser.h"

bool CAIO::generate(const std::string &fileName) {
    try {
        ini::Configuration conf;
        std::ifstream fin(fileName);
        if (!fin.is_open()) {
            throw std::runtime_error("File " + fileName + " not found");
        }
        fin >> conf;
        fin.close();

        std::string mode = conf["General"]["mode"].as_string_or_die();
        std::transform(mode.begin(), mode.end(), mode.begin(), tolower);
        if (mode == "manual") manual(conf);
        else if (mode == "automatic")std::cerr << "Coming soon!!!\n";
        else throw std::runtime_error("Mode not recognised");
        return true;
    }
    catch (const std::exception &ex) {
        std::cerr << "Error parsing file " << fileName << ": " << ex.what() << std::endl;
        return false;
    }
}

void CAIO::manual(const ini::Configuration &conf) {
    //GENERAL
    const int width = conf["General"]["width"].as_int_or_default(20);
    const int height = conf["General"]["height"].as_int_or_default(20);

    const std::string file = conf["General"]["layout"].as_string_or_default("");
    const int amount = conf["States"]["amount"].as_int_or_die();
    if (amount < 1) throw std::runtime_error("Amount of states must be greather than 0");
    const auto layout = parseLayout(file, width, height, amount);

    const auto neighbours = parseCoordinates(conf["General"]["inputs"].as_string_or_default(
            "(0, -1), (-1, -1), (-1, 0), (-1, 1), (0, 1), (1, 1), (1, 0), (1, -1)"));
    //STATES
    const auto stateData = parseStates(conf);
    //TRANSITIONS
    const auto trans = transition(conf, stateData);

    CA::init(width, height, neighbours, stateData, trans, layout);
}

std::vector<std::string> CAIO::byCharacter(const std::string &str, const char &ch) {
    std::stringstream s(str);
    std::string segment;
    std::vector<std::string> seglist;
    while (std::getline(s, segment, ',')) {
        segment.erase(remove_if(segment.begin(), segment.end(), [&ch](const char &a) { return a == ' ' || a == ch; }),
                      segment.end());
        seglist.push_back(segment);
    }
    return seglist;
}

Color CAIO::readColor(std::string str) {
    if (str[0] == '#') {
        return {str.substr(1)};
    } else if (str == "random") {
        return {double(rand()) / RAND_MAX, double(rand()) / RAND_MAX, double(rand()) / RAND_MAX};
    } else if (str == "red") {
        return {"ff0000"};
    } else if (str == "orange") {
        return {"ffa500"};
    } else if (str == "yellow") {
        return {"ffff00"};
    } else if (str == "green") {
        return {"008000"};
    } else if (str == "blue") {
        return {"0000ff"};
    } else if (str == "purple") {
        return {"800080"};
    } else if (str == "brown") {
        return {"a52a2a"};
    } else if (str == "magenta") {
        return {"ff00ff"};
    } else if (str == "tan") {
        return {"d2b48c"};
    } else if (str == "cyan") {
        return {"00ffff"};
    } else if (str == "olive") {
        return {"808000"};
    } else if (str == "maroon") {
        return {"800000"};
    } else if (str == "navy") {
        return {"000080"};
    } else if (str == "aquamarine") {
        return {"7fffd4"};
    } else if (str == "turquoise") {
        return {"40e0d0"};
    } else if (str == "silver") {
        return {"c0c0c0"};
    } else if (str == "lime") {
        return {"00ff00"};
    } else if (str == "teal") {
        return {"008080"};
    } else if (str == "indigo") {
        return {"4b0082"};
    } else if (str == "violet") {
        return {"ee82ee"};
    } else if (str == "pink") {
        return {"ffc0cb"};
    } else if (str == "black") {
        return {"000000"};
    } else if (str == "white") {
        return {"ffffff"};
    } else if (str == "gray" || str == "grey") {
        return {"808080"};
    }
    throw std::runtime_error("Color" + str + "not recognised");
}

std::vector<std::pair<int, int>> CAIO::parseCoordinates(const std::string &inputs) {
    auto seglist = byCharacter(inputs, ',');
    std::vector<std::pair<int, int>> neighbours;
    for (auto &it : seglist) {
        it.erase(remove_if(it.begin(), it.end(), [](const char &a) { return a == '(' || a == ')'; }),
                 it.end());
    }
    for (auto it = seglist.begin(); it < seglist.end(); it = it + 2) {
        neighbours.emplace_back(std::stoi(*(it)), std::stoi(*(it + 1)));
    }
    return neighbours;
}

std::vector<std::tuple<const Automaton *, char, std::string, Color, bool>>
CAIO::parseStates(const ini::Configuration &conf) {
    const int amount = conf["States"]["amount"].as_int_or_die();

    std::map<std::string, char> stateNames;
    for (int i = 1; i <= amount; ++i) {
        const auto strings = byCharacter(conf["States"]["state" + std::to_string(i)].as_string_or_die(), ',');
        if (strings.size() != 3)
            throw std::runtime_error("state" + std::to_string(i) + " needs 3 comma separated fields");

        const std::string name = strings[0];
        stateNames[name] = 'a' + i - 1;
    }

    std::vector<std::tuple<const Automaton *, char, std::string, Color, bool>> stateData;
    srand((int) time(nullptr));
    for (int i = 1; i <= amount; ++i) {
        const auto strings = byCharacter(conf["States"]["state" + std::to_string(i)].as_string_or_die(), ',');
        const std::string name = strings[0];
        std::string color = strings[1];
        std::transform(color.begin(), color.end(), color.begin(), tolower);
        const bool epsilon = color == "epsilon";
        const std::string filename = strings[2];
        stateData.emplace_back(Parser::parseAutomaton(filename, stateNames), stateNames[name], name, readColor(color),
                               epsilon);
    }
    return stateData;
}

FSMTransition CAIO::transition(const ini::Configuration &conf,
                               const std::vector<std::tuple<const Automaton *, char, std::string, Color, bool>> &stateData) {
    FSMTransition trans;
    for (const auto &state: stateData) {
        const auto transitions = byCharacter(conf["Transitions"][std::get<2>(state)].as_string_or_die(), ',');
        if (transitions.size() != 2)
            throw std::runtime_error(std::get<2>(state) + "-transition needs 2 comma separated fields");

        std::string toFind = transitions[0];
        auto a = std::find_if(stateData.begin(), stateData.end(), [&toFind](auto &a) {
            return std::get<2>(a) == toFind;
        });
        if (a == stateData.end()) throw std::runtime_error("Transition to unknown state" + toFind);
        trans(true, std::get<0>(state)) = std::get<0>(*a);

        toFind = transitions[1];
        auto r = std::find_if(stateData.begin(), stateData.end(), [&toFind](auto &a) {
            return std::get<2>(a) == toFind;
        });
        if (r == stateData.end()) throw std::runtime_error("Transition to unknown state" + toFind);
        trans(false, std::get<0>(state)) = std::get<0>(*r);
    }
    return trans;
}

std::vector<char>
CAIO::parseLayout(const std::string &fileName, const int width, const int height, const int amount) {
    std::ifstream fin(fileName);
    if (!fin.is_open()) {
        std::vector<char> layout;
        layout.resize(width * height, 'a');
        return layout;
    }
    std::vector<char> layout;
    std::string line;
    int h = 0;
    while (getline(fin, line)) {
        std::stringstream linestream(line);
        std::string value;
        int w = 0;
        while (getline(linestream, value, ',')) {
            layout.emplace_back('a' + std::max(std::min(std::stoi(value), amount - 1), 0));
            w++;
        }
        if (w != width) throw std::runtime_error("Layout width doesn't match CA width");
        h++;
    }
    if (h != height) throw std::runtime_error("Layout height doesn't match CA height");
    return layout;
}