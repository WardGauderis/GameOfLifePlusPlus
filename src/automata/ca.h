//============================================================================
// @name        : ca.h
// @author      : Thomas Dooms
// @date        : 4/9/19
// @version     : 
// @copyright   : BA1 Informatica - Thomas Dooms - University of Antwerp
// @description : 
//============================================================================


#ifndef GOL_CA_H
#define GOL_CA_H

#include "cell.h"
#include <vector>
#include <string>
#include "../gui/color.h"
#include <map>
#include <deque>

struct Automaton;
struct FSMTransition;

class CA
{
public:

    static void init(uint32_t width, uint32_t height, const std::vector<std::pair<uint32_t, uint32_t>>& neighbours,
            const std::vector<std::tuple<const Automaton*, std::string, Color>>& stateData, const FSMTransition& transition);

    static void destroy();

    static void update();

    static const Color& getColor(uint32_t i, uint32_t iteration);

    static const std::vector<char>& getData(uint32_t iteration);

private:
    static uint32_t width;
    static uint32_t height;

    static std::vector<const Cell*> cells;
    static std::vector<std::pair<uint32_t, uint32_t>> neighbours;

    static std::deque<std::vector<char>> stack;
    static std::map<char, Color> converter;
};


#endif //GOL_CA_H
