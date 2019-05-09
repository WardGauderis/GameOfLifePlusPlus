//============================================================================
// @name        : color.h
// @author      : Thomas Dooms
// @date        : 4/9/19
// @version     : 
// @copyright   : BA1 Informatica - Thomas Dooms - University of Antwerp
// @description : 
//============================================================================

#ifndef GOL_COLOR_H
#define GOL_COLOR_H

#include <QColor>
#include <cmath>
#include <sstream>

/*
 * Class to represent a color
 */
class Color
{
public:

    /*
     * Constructors
     */
    Color() = default;

    Color(double r, double g, double b) : red(std::min(std::max(r, 0.0), 1.0)), green(std::min(std::max(g, 0.0), 1.0)),
                                          blue(std::min(std::max(b, 0.0), 1.0)) {

    };
    Color(const std::string& hex)
    {
        for(uint32_t i = 0; i < 3; i++)
        {
            std::stringstream s({hex[2*i], hex[2*i+1]});
            uint32_t val;
            s >> std::hex >> val;
            (*this)[i] = double(val)/255.99;
        }
    }

    bool operator==(const Color& rhs) const { return red == rhs.red and green == rhs.green and blue == rhs.blue; }

    bool operator!=(const Color& rhs) const { return red != rhs.red or green != rhs.green or blue != rhs.blue; }


    /*
     * Convert the color to a Qcolor object (loss of precision due to Qcolor using int values)
     */
    operator QColor()
    {
        return {static_cast<int>(this->red * 255.99), static_cast<int>(this->green * 255.99), static_cast<int>(this->blue * 255.99)};
    }

    double& operator[](uint32_t index){ switch (index){ case 0: return red; case 1: return green; case 2: return blue; default: throw std::runtime_error("must be smaller than 2"); }};

private:
    double red, green, blue;
};

#endif //GOL_COLOR_H
