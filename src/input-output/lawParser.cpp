//============================================================================
// @name        : LawParser.cpp
// @author      : Ward Gauderis
// @date        : 5/15/19
// @version     : 
// @copyright   : BA1 Informatica - Ward Gauderis - University of Antwerp
// @description : 
//============================================================================
#include <iostream>
#include "lawParser.h"
#include "../automata/dfaPlusPlus.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <stack>

bool LawParser::parseLaws(const std::string &fileName, const unsigned int inputs) {
    try {
        parseSections(fileName);
        parseStates();
        generateDFAPlusPlus(inputs);
        return true;
    }
    catch (const std::exception &ex) {
        std::cerr << "Error parsing file " << fileName << ": " << ex.what() << std::endl;
        return false;
    }
}

void LawParser::parseStates() {
    Section rules = getSection("states");
    unsigned int i = 0;
    for (const auto &rule: rules) {
        std::pair<std::string, std::string> pair = splitBrackets(rule);
        auto name = pair.first;
        Color color = readColor(pair.second);
        char character = 'a' + i;
        states.emplace_back(name, character, color);
        i++;
    }
}

void LawParser::parseSections(const std::string &fileName) {
    std::ifstream fin(fileName);
    if (!fin.is_open()) {
        throw std::runtime_error("File " + fileName + " not found");
    }
    std::string line;
    std::string sectionName;
    while (std::getline(fin, line)) {
        line.erase(remove_if(line.begin(), line.end(), [](const char &a) { return a == ' ' || a == '\t'; }),
                   line.end());
        if ((line[0] == '/' && line[1] == '/') || line.empty()) {
            continue;
        }
        if (line[0] != '(') {
            sectionName = line;
            sections[sectionName];
            continue;
        }
        if (sectionName.empty()) continue;
        sections[sectionName].push_back(line);
    }
    fin.close();
}

Section LawParser::getSection(const std::string &name) {
    try { return sections.at("states"); }
    catch (const std::exception &ex) {
        throw std::runtime_error("Section " + name + " is not defined");
    }
}

std::pair<std::string, std::string> LawParser::splitBrackets(const std::string &line) {
    std::stack<int> indices;
    std::pair<std::string, std::string> inside;
    bool empty = true;
    bool found = false;
    for (unsigned int i = 0; i < line.size(); ++i) {
        char c = line[i];
        if (c == '(') {
            indices.push(i);
            found = true;
        } else if (c == ')') {
            try {
                if (indices.size() == 1 && empty) {
                    inside.first = line.substr(indices.top() + 1, i - indices.top() - 1);
                    inside.second = line.substr(i + 1);
                    empty = false;
                }
                found = true;
                indices.pop();
            }
            catch (const std::exception &ex) {
                throw std::runtime_error("Amount of brackets is incorrect on line: '" + line + "'");
            }
        }
    }
    if (!indices.empty() || !found)
        throw std::runtime_error("Amount of brackets is incorrect on line: '" + line + "'");
    return inside;
}

void LawParser::generateDFAPlusPlus(const unsigned int inputs) {
    for (const auto &state: states) {
        DFAPlusPlus::alphabet.emplace_back(std::get<1>(state));
    }
    DFAPlusPlus::start = new StatePlusPlus('a' - 1);
    DFAPlusPlus::states.emplace_back(DFAPlusPlus::start);
    addStatesRec(inputs, DFAPlusPlus::start);
    DFAPlusPlus::TFAPlusPlus();
    DFAPlusPlus::print("TESTTEST", states);
}

Color LawParser::readColor(std::string str) {
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

void LawParser::addStatesRec(const unsigned int inputs, StatePlusPlus *previous) {
    if (inputs == 0) {
        for (const auto &state: states) {
            auto next = new StatePlusPlus(std::get<1>(state));
            DFAPlusPlus::states.emplace_back(next);
            DFAPlusPlus::transition[{std::get<1>(state), previous}] = next;
            for (const auto &input: states) {
                DFAPlusPlus::transition[{std::get<1>(input), next}] = next;
            }
        }
        return;
    }
    for (const auto &state: states) {
        auto next = new StatePlusPlus('a' - 1);
        DFAPlusPlus::states.emplace_back(next);
        DFAPlusPlus::transition[{std::get<1>(state), previous}] = next;
        addStatesRec(inputs - 1, next);
    }
}
