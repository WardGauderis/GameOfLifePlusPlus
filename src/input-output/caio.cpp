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
#include "lawParser.h"

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
        else if (mode == "automatic") return automatic(conf);
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
    srand((int) time(nullptr));
    const int width = conf["General"]["width"].as_int_or_default(20);
    const int height = conf["General"]["height"].as_int_or_default(20);

    const std::string file = conf["General"]["layout"].as_string_or_default("");
    const int amount = conf["States"]["amount"].as_int_or_die();
    if (amount < 1) throw std::runtime_error("Amount of states must be greather than 0");
    const auto layout = parseLayout(file, width, height, amount);

    const std::string inputs = conf["General"]["inputs"].as_string_or_default(
            "(0, -1), (-1, -1), (-1, 0), (-1, 1), (0, 1), (1, 1), (1, 0), (1, -1)");
    const auto neighbours = parseCoordinates(inputs);
    //STATES
    const auto stateData = parseStates(conf);
    //TRANSITIONS
    const auto trans = transition(conf, stateData);

    CA::init(width, height, neighbours, stateData, trans);
    CA::setStart(layout);
}

std::vector<std::string> CAIO::byCharacter(const std::string &str, const char &ch) {
    std::stringstream s(str);
    std::string segment;
    std::vector<std::string> seglist;
    while (std::getline(s, segment, ch)) {
        segment.erase(remove_if(segment.begin(), segment.end(), [](const char &a) { return a == ' ' || a == '\t'; }),
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
    } else if (str == "epsilon") {
        return {"000000"};
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
    throw std::runtime_error("Color " + str + "not recognised");
}

std::vector<std::pair<int, int>> CAIO::parseCoordinates(const std::string &inputs) {
    auto seglist = byCharacter(inputs, ',');
    std::vector<std::pair<int, int>> neighbours;
    for (auto &it : seglist) {
        it.erase(remove_if(it.begin(), it.end(), [](const char &a) { return a == '(' || a == ')'; }),
                 it.end());
    }
    if (seglist.size() % 2 != 0)
        throw std::runtime_error("inputs are not correctly formatted: '(x1, y1), (x2, y2), ...'");
    try {
        for (auto it = seglist.begin(); it < seglist.end(); it = it + 2) {
            neighbours.emplace_back(std::stoi(*(it)), -std::stoi(*(it + 1)));
        }
    } catch (const std::exception &exception) {
        throw std::runtime_error("inputs are not correctly formatted: '(x1, y1), (x2, y2), ...'");
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
    for (int i = 1; i <= amount; ++i) {
        const auto strings = byCharacter(conf["States"]["state" + std::to_string(i)].as_string_or_die(), ',');
        const std::string name = strings[0];
        std::string color = strings[1];
        std::transform(color.begin(), color.end(), color.begin(), tolower);
        const bool epsilon = color == "epsilon";
        const std::string filename = strings[2];
        auto automaton = Parser::parseAutomaton(filename, stateNames);
        stateData.emplace_back(automaton, stateNames[name], name, readColor(color), epsilon);
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
        trans[{true, std::get<0>(state)}] = std::get<0>(*a);

        toFind = transitions[1];
        auto r = std::find_if(stateData.begin(), stateData.end(), [&toFind](auto &a) {
            return std::get<2>(a) == toFind;
        });
        if (r == stateData.end()) throw std::runtime_error("Transition to unknown state" + toFind);
        trans[{false, std::get<0>(state)}] = std::get<0>(*r);
    }
    return trans;
}

std::vector<char>
CAIO::parseLayout(const std::string &fileName, const int width, const int height, const int amount) {
    std::vector<char> layout;
    if (fileName == "random") {
        for (int i = 0; i < width * height; ++i) {
            layout.emplace_back('a' + rand() % amount);
        }
        return layout;
    }
    std::ifstream fin(fileName);
    if (!fin.is_open()) {
        layout.resize(width * height, 'a');
        return layout;
    }
    std::string line;
    for (int i = 0; i < height; ++i) {
        if (!getline(fin, line)) line = "";
        std::string value;
        std::stringstream lineStream(line);
        for (int j = 0; j < width; ++j) {
            if (!getline(lineStream, value, ',')) value = "0";
            try { layout.emplace_back('a' + std::max(std::min(std::stoi(value), amount - 1), 0)); }
            catch (const std::invalid_argument &ex) { layout.push_back('a'); }
        }
    }
    return layout;
}

bool CAIO::exportCA(const std::vector<char> &CA, const int width, const int heigth, const std::string &fileName) {
    try {
        std::ofstream fout(fileName + ".csv");
        if (!fout.is_open()) {
            throw std::runtime_error("File " + fileName + ".csv couldn't be opened");
        }
        for (int i = 0; i < heigth; ++i) {
            for (int j = 0; j < width; ++j) {
                fout << std::to_string(CA[i * heigth + j] - 'a') << (j == width - 1 ? "" : ",");
            }
            fout << '\n';
        }
        fout.close();
        return true;
    } catch (const std::exception &ex) {
        std::cerr << "Error exporting to file " << fileName << ": " << ex.what() << std::endl;
        return false;
    }
}

bool CAIO::automatic(const ini::Configuration &conf) {
    //GENERAL
    srand((int) time(nullptr));
    const int width = conf["General"]["width"].as_int_or_default(20);
    const int height = conf["General"]["height"].as_int_or_default(20);

    const std::string file = conf["General"]["layout"].as_string_or_default("");
    const int amount = conf["States"]["amount"].as_int_or_die();
    if (amount < 1) throw std::runtime_error("Amount of states must be greather than 0");
    const auto layout = parseLayout(file, width, height, amount);

    const std::string inputs = conf["General"]["inputs"].as_string_or_default(
            "(0, -1), (-1, -1), (-1, 0), (-1, 1), (0, 1), (1, 1), (1, 0), (1, -1)");
    const auto neighbours = parseCoordinates(inputs);

    const std::string laws = conf["General"]["layout"].as_string_or_default("");

    LawParser parser;
    bool success = parser.parseLaws(laws);
    if (success) {
//        CA::init(width, height, neighbours, stateData, trans);
//        CA::setStart(layout);
    }
    return success;
}

