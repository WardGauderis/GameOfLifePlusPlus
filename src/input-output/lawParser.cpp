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
#include <fstream>
#include <sstream>
#include <algorithm>
#include <stack>

bool LawParser::parseLaws(const std::string &fileName) {
    try {
        parseSections(fileName);
        parseStates();
        return true;
    }
    catch (const std::exception &ex) {
        std::cerr << "Error parsing file " << fileName << ": " << ex.what() << std::endl;
        return false;
    }
}

void LawParser::parseStates() {
    Section states = getSection("states");
    for (const auto &rule: states) {
        std::pair<std::string, std::string> pair = splitBrackets(rule);
        auto name = pair.first;
        auto color = pair.second;
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
    for (unsigned int i = 0; i < line.size(); ++i) {
        char c = line[i];
        if (c == '(') {
            indices.push(i);
        } else if (c == ')') {
            try {
                if (indices.size() == 1 && empty) {
                    inside.first = line.substr(indices.top() + 1, i - indices.top() - 1);
                    inside.second = line.substr(i + 1);
                    empty = false;
                }
                indices.pop();
            }
            catch (const std::exception &ex) {
                throw std::runtime_error("Amount of brackets is incorrect on line: " + line);
            }
        }
    }
    if (!indices.empty())throw std::runtime_error("Amount of brackets is incorrect on line: " + line);
    return inside;
}
