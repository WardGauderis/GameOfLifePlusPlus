//============================================================================
// @name        : ca.cpp
// @author      : Thomas Dooms
// @date        : 4/9/19
// @version     : 
// @copyright   : BA1 Informatica - Thomas Dooms - University of Antwerp
// @description : 
//============================================================================

#include <iostream>
#include "ca.h"
#include "fsm.h"


uint32_t CA::width;
uint32_t CA::height;

std::vector<const Cell*> CA::cells;
std::vector<std::pair<int, int>> CA::neighbours;

std::deque<std::vector<char>> CA::stack;
std::map<char, Color> CA::converter;

void CA::init(uint32_t width, uint32_t height, const std::vector<std::pair<int, int>>& neighbours,
              const std::vector<std::tuple<const Automaton*, char, std::string, Color, bool>>& stateData, const FSMTransition& transition, const std::vector<char> start)
{
    CA::width = width;
    CA::height = height;
    CA::neighbours = neighbours;

    std::map<const Automaton*, std::pair<char, bool>> states;
    std::map<char, const Automaton*> charToState;

    for(const auto& data : stateData)
    {
        charToState.emplace(std::get<1>(data), std::get<0>(data));
        converter.emplace(std::get<1>(data), std::get<3>(data));
        states.emplace(std::get<0>(data), std::pair<char, bool>{std::get<1>(data), std::get<4>(data)});
    }

    FSM::init(states, transition);

    CA::cells.reserve(width*height);
    stack.emplace_back(start);

    for(uint32_t i = 0; i < width*height; i++) cells.emplace_back(new FSM{charToState.at(stack.back()[i])});
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
            std::string input(neighbours.size(), '0');

            for(uint32_t i = 0; i < neighbours.size(); i++)
                input[i] = stack.back()[getIndex(neighbours[i], x, y)];

            (*cells.at(y * width + x))(input);
        }
    stack.emplace_back(width*height);
    for(uint32_t i = 0; i < width*height; i++) stack.back()[i] = cells[i]->getCurrent();
}

uint32_t CA::getIndex(std::pair<int, int> offset, uint32_t x, uint32_t y)
{
    uint32_t xVal = (int(width ) + (offset.first  + int(x)) % int(width )) % int(width );
    uint32_t yVal = (int(height) + (offset.second + int(y)) % int(height)) % int(height);
    getColors();
    return yVal * width + xVal;
}

std::vector<Color> CA::getColors()
{
    std::vector<Color> colors;
    colors.resize(converter.size());
    for(const auto& elem : converter) colors.push_back(elem.second);
    return colors;
}

