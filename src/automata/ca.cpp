//============================================================================
// @name        : ca.cpp
// @author      : Thomas Dooms
// @date        : 4/9/19
// @version     : 
// @copyright   : BA1 Informatica - Thomas Dooms - University of Antwerp
// @description : 
//============================================================================

#include "ca.h"
#include "fsm.h"


uint32_t CA::width;
uint32_t CA::height;

std::vector<const Cell*> CA::cells;
std::vector<std::pair<int, int>> CA::neighbours;

std::deque<std::vector<char>> CA::stack;
std::map<char, Color> CA::converter;

void CA::init(uint32_t width, uint32_t height, const std::vector<std::pair<int, int>>& neighbours,
              const std::vector<std::tuple<const Automaton*, char, std::string, Color, bool>>& stateData, const FSMTransition& transition)
{
    CA::width = width;
    CA::height = height;
    CA::neighbours = neighbours;

    std::vector<const Automaton*> states;
    states.reserve(stateData.size());
    std::map<const Automaton*, std::pair<char, bool>> temp;

    for(const auto& data : stateData)
    {
        states.push_back(std::get<0>(data));
        converter.emplace(std::get<1>(data), std::get<3>(data));
        temp.emplace(std::get<0>(data), std::pair<char, bool>{std::get<1>(data), std::get<4>(data)});
    }

    FSM::init(temp, transition);

    CA::cells.reserve(width*height);
    stack.emplace_back(width*height);
    for(uint32_t i = 0; i < width*height; i++)
    {
        int a = 1;
        if(i == 1 or i == 22 or i == 41 or i == 40 or i ==42) a = 0;
        stack[0][i] = std::get<1>(stateData[a]);
        cells.push_back(new FSM{std::get<0>(stateData[a])});
    }
}

void CA::destroy()
{
    for(const auto& cell : cells) delete cell;
}

const Color& CA::getColor(uint32_t i, uint32_t iteration)
{
    return converter.at(stack[iteration].at(i));
}

const std::vector<char>& CA::getData(uint32_t iteration)
{
    return stack[iteration];
}

void CA::update()
{
    for(uint32_t x = 0; x < width; x++)
        for(uint32_t y = 0; y < height; y++)
        {
            std::string input;
            for(const auto& pair : neighbours) input += stack.back()[((y + pair.second)%height) * width + ((x + pair.first)%width)];
            (*cells.at(y * width + x))(input);
        }
    stack.emplace_back(width*height);
    for(uint32_t i = 0; i < width*height; i++) stack.back()[i] = cells[i]->getCurrent();
}

