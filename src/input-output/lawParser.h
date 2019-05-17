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
#include "../automata/dfaPlusPlus.h"
#include "statement.h"


struct Section : public std::vector<std::string> {

};

class LawParser {
    std::map<std::string, Section> sections;

    StateMap states;

    void parseStates();

    void parseSections(const std::string &fileName);

    Section getSection(const std::string &name);

    static std::pair<std::string, std::string> splitBrackets(const std::string &line);

    static std::pair<std::string, std::string> splitDoublePoint(const std::string &line);

    void generateDFAPlusPlus();

    TempDFA generatePerState(const std::tuple<std::string, char, Color, char> &state);

    TempDFA generateLaw(const std::string &law);

    void readCondition(std::string condition);

    static std::pair<unsigned int, unsigned int> parseRange(const std::string &range);

    static std::pair<std::string, std::string>
    splitStatement(const std::string &statement, const std::string &splitter);

    static unsigned int parseInt(const std::string &str);

public:
    bool parseLaws(const std::string &fileName);

    static Color readColor(std::string str);
};


#endif //GOL_LAWPARSER_H
