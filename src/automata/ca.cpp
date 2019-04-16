//============================================================================
// @name        : ca.cpp
// @author      : Thomas Dooms
// @date        : 4/9/19
// @version     : 
// @copyright   : BA1 Informatica - Thomas Dooms - University of Antwerp
// @description : 
//============================================================================

#include "ca.h"


const Cell& CA::operator()(uint32_t x, uint32_t y) const
{
    return *cells.at(y * width + x);
}

const Color& CA::getColor(uint32_t x, uint32_t y) const
{
    return colorConverter.at((*this)(x,y).getState());
}

void CA::update()
{
    std::vector<char> temp(width*height);
    for(uint32_t i = 0; i < cells.size(); i++) temp[i] = charConverter.at(cells[i]->getState());

    for(uint32_t x = 0; x < width; x++)
        for(uint32_t y = 0; y < height; y++)
        {
            std::string input;
            for(const auto& pair : neighbours) input += temp[(y + pair.second) * width + (x + pair.first)];
            (*this)(x,y)(input);
        }
}