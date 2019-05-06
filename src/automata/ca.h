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

    static void init(uint32_t width, uint32_t height, const std::vector<std::pair<int, int>>& neighbours,
            const std::vector<std::tuple<const Automaton*, char, std::string, Color, bool>>& stateData, const FSMTransition& transition, const std::vector<char> start);

    static void destroy();

    static void update();

    static const Color& getColor(uint32_t i, uint32_t iteration);
    static const std::vector<char>& getData(uint32_t iteration);

    static uint32_t getWidth() { return width ; }
    static uint32_t getHeight(){ return height; }
    static uint32_t getSize(){ return stack.size(); }

private:
    static uint32_t width;
    static uint32_t height;

    static std::vector<const Cell*> cells;
    static std::vector<std::pair<int, int>> neighbours;

    static std::deque<std::vector<char>> stack;
    static std::map<char, Color> converter;
};


#endif //GOL_CA_H
