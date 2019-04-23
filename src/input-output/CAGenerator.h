//============================================================================
// @name        : CAGenerator.h
// @author      : Ward Gauderis
// @date        : 4/23/19
// @version     : 
// @copyright   : BA1 Informatica - Ward Gauderis - University of Antwerp
// @description : 
//============================================================================
#ifndef GOL_CAGENERATOR_H
#define GOL_CAGENERATOR_H

#include "../automata/ca.h"
#include "../input-output/ini/ini.h"

class CAGenerator {
public:
    CA *generate(const std::string &filename);

private:
    CA *manual(const ini::Configuration &conf);

    std::vector<std::string> byCharacter(std::string str, const char& ch);

    Color readColor(std::string str);
};


#endif //GOL_CAGENERATOR_H
