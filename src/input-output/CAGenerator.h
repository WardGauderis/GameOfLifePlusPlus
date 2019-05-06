//============================================================================
// @name        : CAGenerator.h
// @author      : Ward Gauderis
// @date        : 4/23/19
// @version     : 1.0
// @copyright   : BA1 Informatica - Ward Gauderis - University of Antwerp
// @description : Initializes the CA from an ini file
//============================================================================
#ifndef GOL_CAGENERATOR_H
#define GOL_CAGENERATOR_H

#include "../automata/ca.h"
#include "../input-output/ini/ini.h"

class CAGenerator {
public:
    //True if success, else false and cerr message
    static bool generate(const std::string &filename);

private:
    static FSMTransition transition(const ini::Configuration &conf,
                                    const std::vector<std::tuple<const Automaton *, char, std::string, Color, bool>> &stateData);

    static std::vector<std::tuple<const Automaton *, char, std::string, Color, bool>> states(const ini::Configuration &conf);

    static std::vector<std::pair<int, int>> coordinates(const std::string &inputs);

    static void manual(const ini::Configuration &conf);

    static std::vector<std::string> byCharacter(const std::string &str, const char &ch);

    static Color readColor(std::string str);
};


#endif //GOL_CAGENERATOR_H
