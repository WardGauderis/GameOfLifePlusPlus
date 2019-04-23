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

CA *CAGenerator::generate(const std::string &filename) {
    try {
        ini::Configuration conf;
        std::ifstream fin(filename);
        fin >> conf;
        fin.close();
        const std::string mode = conf["General"]["mode"].as_string_or_die();
        if (mode == "manual") {
            return manual(conf);
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

CA *CAGenerator::manual(const ini::Configuration &conf) {
    const int width = conf["General"]["width"].as_int_or_default(200);
    const int height = conf["General"]["height"].as_int_or_default(200);
    const std::string layout = conf["General"]["layout"].as_string_or_default("");
    const std::string inputs = conf["General"]["inputs"].as_string_or_default(
            "(0, -1), (-1, -1), (-1, 0), (-1, 1), (0, 1), (1, 1), (1, 0), (1, -1)");
    auto seglist = byCharacter(inputs, ',');
    std::vector<std::pair<uint32_t, uint32_t>> neighbours;
    for (auto &it : seglist) {
        it.erase(remove_if(it.begin(), it.end(), [](const char &a) { return a == '(' || a == ')'; }),
                 it.end());
    }
    for (auto it = seglist.begin(); it != seglist.end() - 1;) {
        neighbours.emplace_back(static_cast<unsigned int>(std::stoi(*(it++))),
                                static_cast<unsigned int>(std::stoi(*(it))));
    }
    const int amount = conf["States"]["amount"].as_int_or_die();
    std::vector<std::tuple<Automaton *, std::string, Color>> stateData;
    for (int i = 1; i <= amount; ++i) {
        const auto strings = byCharacter(conf["States"]["state" + std::to_string(i)].as_string_or_die(), ',');
        if (strings.size() != 3) {
            std::cerr << "NOT ENOUGH ARGUMENTS!!!\n";
        }
        const std::string name = strings[0];
        const std::string color = strings[1];
        const std::string filename = strings[2];
        stateData.emplace_back(parseAutomaton(filename), name,)
    }
    for (int i = 1; i <= amount; ++i) {
        const auto strings = byCharacter(conf["States"]["state" + std::to_string(i)].as_string_or_die(), ',');
        if (strings.size() != 3) {
            std::cerr << "NOT ENOUGH ARGUMENTS!!!\n";
        }
        const std::string accept = strings[0];
        const std::string reject = strings[1];
    }
    CA::init(width, height, neighbours, stateData,)
    return nullptr;
}

std::vector<std::string> CAGenerator::byCharacter(std::string str, const char &ch) {
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
        return Color(str.substr())
    } else if (str == "epsilon")

    } else if (str == "red") {
        return Color("ff0000");
    } else if (str == "orange") {
        return Color("ffa500");
    } else if (str == "yellow") {
        return Color("ffff00");
    } else if (str == "green") {
        return Color("008000");
    }else if (str == "blue") {
        return Color("0000ff");
    }else if (str == "purple") {
        return Color("800080");
    }else if (str == "brown") {
        return Color("a52a2a");
    }else if (str == "magenta") {
        return Color("ff00ff");
    }else if (str == "tan") {
        return Color("d2b48c");
    }else if (str == "cyan") {
        return Color("00ffff");
    }else if (str == "olive") {
        return Color("808000");
    }else if (str == "maroon") {
        return Color("800000");
    }else if (str == "navy") {
        return Color("000080");
    }else if (str == "aquamarine") {
        return Color("7fffd4");
    }else if (str == "turquoise") {
        return Color("40e0d0");
    }else if (str == "silver") {
        return Color("c0c0c0");
    }else if (str == "lime") {
        return Color("00ff00");
    }else if (str == "teal") {
        return Color("008080");
    }else if (str == "indigo") {
        return Color("4b0082");
    }else if (str == "violet") {
        return Color("ee82ee");
    }else if (str == "pink") {
        return Color("ffc0cb");
    }else if (str == "black") {
        return Color("000000");
    }else if (str == "white") {
        return Color("ffffff");
    }else if (str == "gray" || str == "grey") {
        return Color("808080");
    }
}
