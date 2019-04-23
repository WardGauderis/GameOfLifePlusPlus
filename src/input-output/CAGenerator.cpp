//============================================================================
// @name        : CAGenerator.cpp
// @author      : Ward Gauderis
// @date        : 4/23/19
// @version     : 
// @copyright   : BA1 Informatica - Ward Gauderis - University of Antwerp
// @description : 
//============================================================================
#include <fstream>
#include <iostream>
#include <sstream>
#include "CAGenerator.h"
#include "../automata/transitions.h"
#include "parser.h"

void CAGenerator::generate(const std::string &filename) {
    try {
        ini::Configuration conf;
        std::ifstream fin(filename);
        fin >> conf;
        fin.close();
        const std::string mode = conf["General"]["mode"].as_string_or_die();
        if (mode == "manual") {
            manual(conf);
        } else if (mode == "automatic") {
            std::cerr << "Coming soon!!!\n";
        } else {
            std::cerr << "Coming soon!!!\n";
        }
    }
    catch (ini::ParseException &ex) {
        std::cerr << "Error parsing file: " << filename << ": " << ex.what() << std::endl;
    }
}

void CAGenerator::manual(const ini::Configuration &conf) {
    const int width = conf["General"]["width"].as_int_or_default(200);
    const int height = conf["General"]["height"].as_int_or_default(200);
    const std::string layout = conf["General"]["layout"].as_string_or_default("");
    const std::string inputs = conf["General"]["inputs"].as_string_or_default(
            "(0, -1), (-1, -1), (-1, 0), (-1, 1), (0, 1), (1, 1), (1, 0), (1, -1)");
    auto seglist = byCharacter(inputs, ',');
    std::vector<std::pair<int, int>> neighbours;
    for (auto &it : seglist) {
        it.erase(remove_if(it.begin(), it.end(), [](const char &a) { return a == '(' || a == ')'; }),
                 it.end());
    }
    for (auto it = seglist.begin(); it < seglist.end(); it = it + 2) {
        neighbours.emplace_back(std::stoi(*(it)), std::stoi(*(it + 1)));
    }
    const int amount = conf["States"]["amount"].as_int_or_die();
    std::map<std::string, char> stateNames;
    for (int i = 1; i <= amount; ++i) {
        const auto strings = byCharacter(conf["States"]["state" + std::to_string(i)].as_string_or_die(), ',');
        const std::string name = strings[0];
        stateNames[name] = 'a' + i - 1;
    }
    std::vector<std::tuple<const Automaton *, char, std::string, Color>> stateData;
    for (int i = 1; i <= amount; ++i) {
        const auto strings = byCharacter(conf["States"]["state" + std::to_string(i)].as_string_or_die(), ',');
        if (strings.size() != 3) {
            std::cerr << "NOT ENOUGH ARGUMENTS!!!\n";
        }
        const std::string name = strings[0];
        const std::string color = strings[1];
        const std::string filename = strings[2];
        stateData.emplace_back(Parser::parseAutomaton(filename, stateNames), stateNames[name], name, readColor(color));
    }
    FSMTransition trans;
    auto *transition = &trans.getMap();
    for (const auto &state: stateData) {
        const auto transitions = byCharacter(conf["Transitions"][std::get<2>(state)].as_string_or_die(), ',');
        if (transitions.size() != 2) {
            std::cerr << "NOT ENOUGH ARGUMENTS!!!\n";
        }
        const std::string accept = transitions[0];
        const Automaton *a = std::get<0>(*std::find_if(stateData.begin(), stateData.end(), [&accept](
                const std::tuple<const Automaton *, char, std::string, Color> &a) {
            return std::get<2>(a) == accept;
        }));
        (*transition)[{std::get<0>(state), true}] = a;
        const std::string reject = transitions[1];
        const Automaton *r = std::get<0>(*std::find_if(stateData.begin(), stateData.end(), [&reject](
                const std::tuple<const Automaton *, char, std::string, Color> &a) {
            return std::get<2>(a) == reject;
        }));
        (*transition)[{std::get<0>(state), false}] = r;
    }
    CA::init(width, height, neighbours, stateData, trans);
}

std::vector<std::string> CAGenerator::byCharacter(const std::string &str, const char &ch) {
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

Color CAGenerator::readColor(std::string str) {
    Color a(0, 1, 1);
    std::transform(str.begin(), str.end(), str.begin(), tolower);
    if (str[0] == '#') {
        return {str.substr(1)};
    } else if (str == "epsilon") {
        std::cerr << "EPSILOOOOOON"; //TODO
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
    return {"000000"};
}
