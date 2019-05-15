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

struct Section : public std::vector<std::string> {

};

class LawParser {
    std::map<std::string, Section> sections;

    void parseStates();

    void parseSections(const std::string &fileName);

    Section getSection(const std::string &name);

public:
    bool parseLaws(const std::string &fileName);

    std::string getInBrackets(const std::string &line);
};


#endif //GOL_LAWPARSER_H
