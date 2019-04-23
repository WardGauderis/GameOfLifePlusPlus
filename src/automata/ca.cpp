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


uint32_t CA::width = 0;
uint32_t CA::height = 0;
std::vector<const Cell*> CA::cells = {};
std::vector<std::pair<uint32_t, uint32_t>> CA::neighbours = {};

void CA::init(uint32_t width, uint32_t height, const std::vector<std::pair<uint32_t, uint32_t>>& neighbours,
              const std::vector<std::tuple<const Automaton*, const std::string&, const Color&>>& stateData,
              const FSMTransition& transition, const std::vector<std::string>& starts)
{
    CA::width = width;
    CA::height = height;
    CA::neighbours = neighbours;

    FSM::init(stateData, transition);

    CA::cells.reserve(width*height);
    std::map<std::string, const Automaton*> converter;
    for(const auto& data : stateData) converter[std::get<1>(data)] = std::get<0>(data);

    for(uint32_t i = 0; i < width*height; i++) cells.push_back(new FSM{converter.at(starts[i])});
}

void CA::destroy()
{
    for(const auto& cell : cells) delete cell;
}

const Color& CA::getColor(uint32_t x, uint32_t y)
{
    return std::get<2>(cells.at(y * width + x)->getData());
}

void CA::update()
{
    std::vector<char> temp(width*height);
    for(uint32_t i = 0; i < cells.size(); i++) temp[i] = std::get<0>(cells[i]->getData());

    for(uint32_t x = 0; x < width; x++)
        for(uint32_t y = 0; y < height; y++)
        {
            std::string input;
            for(const auto& pair : neighbours) input += temp[(y + pair.second) * width + (x + pair.first)];
            (*cells.at(y * width + x))(input);
        }
}