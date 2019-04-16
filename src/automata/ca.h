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

class CA {
public:
    CA(const uint32_t width, const uint32_t height) : width(width), height(height) {
        neighbours = {{-1, -1},
                      {0,  -1},
                      {1,  -1},
                      {1,  0},
                      {1,  1},
                      {0,  1},
                      {-1, 1},
                      {-1, 0}};
    };

    ~CA() = default;

    void update();

    const Cell &operator()(uint32_t x, uint32_t y) const;

    const Color &getColor(uint32_t x, uint32_t y) const;

private:
    std::vector<Cell *> cells;
    std::vector<std::pair<uint32_t, uint32_t>> neighbours;

    std::map<std::string, char> charConverter;
    std::map<std::string, Color> colorConverter;

    uint32_t width;
    uint32_t height;
};


#endif //GOL_CA_H
