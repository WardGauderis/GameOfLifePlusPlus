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
CA::Type CA::type = Type::none;


void CA::init(uint32_t width, uint32_t height, const std::vector<std::pair<int, int>>& neighbours,
              const std::vector<std::tuple<const Automaton*, char, std::string, Color, bool>>& stateData, const FSMTransition& transition)
{
    CA::width = width;
    CA::height = height;
    CA::neighbours = neighbours;

    std::map<char, const Automaton*> charToState;

    std::map<const Automaton*, std::pair<char, bool>> states;

    for(const auto& data : stateData)
    {
        charToState.emplace(std::get<1>(data), std::get<0>(data));
        converter.emplace(std::get<1>(data), std::get<3>(data));
        states.emplace(std::get<0>(data), std::pair<char, bool>{std::get<1>(data), std::get<4>(data)});
    }

    FSM::init(states, transition, charToState);
    type = Type::fsm;
}

void CA::init(uint32_t width, uint32_t height, const std::vector<std::pair<int, int>> &neighbours, const std::map<char, Color>& converter)
{
    CA::width = width;
    CA::height = height;
    CA::neighbours = neighbours;
    CA::converter = converter;

    type = Type::dfaPlusPlus;
}

void CA::setStart(const std::vector<char>& start)
{
    // this resets everything, if it hasnt been done already
    stack = {};
    cells = {};

    stack.emplace_back(start);

    CA::cells.reserve(width*height);
    switch(type)
    {
        case Type::fsm:
            for(uint32_t i = 0; i < width*height; i++) cells.emplace_back(new FSM{ start[i] });
            break;
        case Type::dfaPlusPlus:
            for(uint32_t i = 0; i < width*height; i++) cells.emplace_back(new DFAPlusPlus{ start[i] });
            break;
        default:
            throw std::runtime_error("CA not initialized correctly before calling this function\n");
    }

}

void CA::destroy()
{
    for(const auto& cell : cells) delete cell;
}

char CA::getChar(uint32_t i, uint32_t iteration)
{
    return stack[iteration].at(i);
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
    return yVal * width + xVal;
}

const std::map<char, Color>& CA::getColors()
{
    return converter;
}

