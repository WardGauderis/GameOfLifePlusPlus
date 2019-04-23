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
std::vector<std::pair<uint32_t, uint32_t>> CA::neighbours;

std::deque<std::vector<char>> CA::stack;
std::map<char, Color> CA::converter;

void CA::init(uint32_t width, uint32_t height, const std::vector<std::pair<uint32_t, uint32_t>>& neighbours,
              const std::vector<std::tuple<const Automaton*, const std::string&, const Color&>>& stateData,
              const FSMTransition& transition, const std::vector<std::string>& layout)
{
    CA::width = width;
    CA::height = height;
    CA::neighbours = neighbours;

    std::vector<const Automaton*> states;
    states.reserve(stateData.size());
    for(const auto& data : stateData) states.push_back(std::get<0>(data));

    FSM::init(states, transition);

    CA::cells.reserve(width*height);
    std::map<std::string, const Automaton*> converter;
    for(const auto& data : stateData) converter[std::get<1>(data)] = std::get<0>(data);

    for(uint32_t i = 0; i < width*height; i++) cells.push_back(new FSM{converter.at(layout[i])});
}

void CA::destroy()
{
    for(const auto& cell : cells) delete cell;
}

const Color& CA::getColor(uint32_t x, uint32_t y, uint32_t iteration)
{
    if(iteration == stack.size()) update();
    else if(iteration > stack.size()) throw std::runtime_error("multiple steps at once");

    return converter.at(stack[iteration].at(y * width + x));
}

void CA::update()
{
    for(uint32_t x = 0; x < width; x++)
        for(uint32_t y = 0; y < height; y++)
        {
            std::string input;
            for(const auto& pair : neighbours) input += (*end(stack))[(y + pair.second) * width + (x + pair.first)];
            (*cells.at(y * width + x))(input);
        }

    stack.emplace_back(width*height);
    for(uint32_t i = 0; i < width*height; i++) (*end(stack))[i] = cells[i]->getCurrent();
}