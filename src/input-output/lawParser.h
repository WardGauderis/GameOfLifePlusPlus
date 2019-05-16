//============================================================================
// @name        : LawParser.h
// @author      : Ward Gauderis
// @date        : 5/15/19
// @version     : 
// @copyright   : BA1 Informatica - Ward Gauderis - University of Antwerp
// @description : 
//============================================================================
#ifndef GOL_LAWPARSER_H
#define GOL_LAWPARSER_H

#include <map>
#include <vector>
#include "../gui/color.h"
#include "../automata/states.h"

struct Section : public std::vector<std::string> {

};

class LawParser {
    std::map<std::string, Section> sections;

    std::vector<std::tuple<std::string, char, Color>> states;

    void parseStates();

    void parseSections(const std::string &fileName);

    Section getSection(const std::string &name);

    static std::pair<std::string, std::string> splitBrackets(const std::string &line);

    void generateDFAPlusPlus(unsigned int inputs);

    void addStatesRec(unsigned int inputs, StatePlusPlus *previous);

public:
    bool parseLaws(const std::string &fileName, unsigned int inputs);

    static Color readColor(std::string str);
};


#endif //GOL_LAWPARSER_H
