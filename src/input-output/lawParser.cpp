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

bool LawParser::parseLaws(const std::string &fileName) {
    try {
        parseSections(fileName);
        parseStates();
        generateDFAPlusPlus();
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
        std::pair<std::string, std::string> pair = splitDoublePoint(rule);
        auto name = splitBrackets(pair.first).first;

        size_t index = pair.second.find(',');
        if (index == std::string::npos) index = pair.second.size();
        std::string c = pair.second.substr(0, index);

        Color color = readColor(c);
        char character = 'a' + i;
        try {
            states.character(name);
            std::cerr << "Redefinition of state '" + name + "' is ignored\n";
        }
        catch (const std::exception &exception) {
            states.emplace_back(name, character, color, character);
            i++;
        }
    }
    for (unsigned int j = 0; j < states.size(); ++j) {
        std::pair<std::string, std::string> pair = splitDoublePoint(rules[j]);
        pair = splitStatement(pair.second, ",");
        std::get<3>(states[j]) = states.character(pair.second);
    }
    for (const auto &section: sections) {
        try {
            states.character(section.first);
        } catch (const std::exception &exception) {
            if (section.first != "states") {
                std::cerr << "The section '" + section.first +
                             "' is not a state defined in the section 'states' and will be ignored\n";
            }
        }
    }
    for (const auto &state: states) {
        DFAPlusPlus::alphabet.emplace_back(std::get<1>(state));
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
    try { return sections.at(name); }
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

void LawParser::generateDFAPlusPlus() {
    std::vector<TempDFA> dfas;
    for (const auto &state: states) {
        dfas.emplace_back(generatePerState(state));
    }
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
    throw std::runtime_error("Color '" + str + "' not recognised");
}

TempDFA LawParser::generatePerState(const std::tuple<std::string, char, Color, char> &state) {
    Section laws = getSection(std::get<0>(state));
    char def = std::get<3>(state);
    std::vector<TempDFA> dfas;
    for (const auto &law: laws) {
        dfas.emplace_back(generateLaw(law, def));
    }
    return TempDFA();
}

TempDFA LawParser::generateLaw(const std::string &law, const char def) {
    auto pair = splitDoublePoint(law);
    std::string condition = splitBrackets(pair.first).first;
    char next = states.character(pair.second);
    Statement *statement = readCondition(condition, next);
    auto test = statement->generateTempDFA(def);
    test.print("TEST", states);
    return statement->generateTempDFA(def);
}

std::pair<std::string, std::string> LawParser::splitDoublePoint(const std::string &line) {
    try {
        return splitStatement(line, ":");
    } catch (const std::exception &exception) {
        throw std::runtime_error("Line '" + line + "' is missing a ':'");
    }
}

Statement *LawParser::readCondition(std::string condition, const char next) {
    if (condition[0] == '(') {
        auto pair = splitBrackets(condition);
        Statement *first = readCondition(pair.first, next);
        Statement *second;
        if (pair.second.substr(0, 2) == "&&") {
            second = readCondition(splitBrackets(pair.second.substr(2)).first, next);
            return new AND(first, second);
        } else if (pair.second.substr(0, 2) == "||") {
            second = readCondition(splitBrackets(pair.second.substr(2)).first, next);
            return new OR(first, second);
        } else {
            throw std::runtime_error(
                    "Unnecessary brackets or incorrect composition of statements in statement '" + condition + "'");
        }
    } else if (condition[0] == '!') {
        Statement *statement = readCondition(splitBrackets(condition).first, next);
        return new NOT(statement);
    } else if (condition[0] == '[') {
        auto pair = splitStatement(condition, "==");
        std::size_t index = pair.first.find(']');
        if (index == std::string::npos)
            throw std::runtime_error("']' is missing in range statement '" + condition + "'");
        auto range = parseRange(pair.first.substr(1, index - 1));
        char state = states.character(pair.second);
        if (pair.first[index + 1] == '&') {
            return new RANGEAND(range.first, range.second, state, next);
        } else if (pair.first[index + 1] == '|') {
            return new RANGEOR(range.first, range.second, state, next);
        } else {
            throw std::runtime_error(
                    "A type declaration ('&' or '|') of the position range was forgotten in statement '" + condition +
                    "'");
        }
    } else if (isdigit(condition[0])) {
        auto pair = splitStatement(condition, "==");
        unsigned int position = parseInt(pair.first);
        char state = states.character(pair.second);
        return new POSITION(position, state, next);
    } else {
        auto pair = splitStatement(condition, "==");
        char state = states.character(pair.first);
        if (pair.second[0] == '[') {
            std::size_t index = pair.second.find(']');
            if (index == std::string::npos)
                throw std::runtime_error("']' is missing in range statement '" + condition + "'");
            auto range = parseRange(pair.second.substr(1, index - 1));
            return new RANGEAMOUNT(range.first, range.second, state, next);
        } else {
            unsigned int amount = parseInt(pair.second);
            return new AMOUNT(amount, state, next);
        }
    }
}

std::pair<unsigned int, unsigned int> LawParser::parseRange(const std::string &range) {
    size_t index = range.find('-');
    if (index == std::string::npos) throw std::runtime_error("'-' is missing in range '" + range + "'");
    unsigned int begin;
    unsigned int end;
    try {
        begin = parseInt(range.substr(0, index));
        end = parseInt(range.substr(index + 1));
    } catch (const std::exception &exception) {
        throw std::runtime_error("Operand is not an unsigned integer in range '" + range + "'");
    }
    if (begin >= end) {
        throw std::runtime_error("First operand must be smaller than second operand in range '" + range + "'");
    }
    return {begin, end};
}

std::pair<std::string, std::string>
LawParser::splitStatement(const std::string &statement, const std::string &splitter) {
    size_t index = statement.find(splitter);
    if (index == std::string::npos)
        throw std::runtime_error("'" + splitter + "' is missing in statement '" + statement + "'");
    return {statement.substr(0, index), statement.substr(index + splitter.size())};
}

unsigned int LawParser::parseInt(const std::string &str) {
    std::string::const_iterator it1 = str.begin();
    while (it1 != str.end() && std::isdigit(*it1)) ++it1;
    if (!(!str.empty() && it1 == str.end())) {
        throw std::runtime_error("'" + str + "' should be an unsigned int, but isn't");
    }
    try {
        return std::stoi(str);
    } catch (const std::exception &ex) {
        throw std::runtime_error("'" + str + "' should be an unsigned int, but isn't");
    }
}
