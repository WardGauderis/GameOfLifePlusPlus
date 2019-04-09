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
    Color(double r, double g, double b) : red(r), green(g), blue(b) {};

    /*
     * Convert the color to a Qcolor object (loss of precision due to Qcolor using int values)
     */
    operator QColor()
    {
        return {(int)std::round(this->red), (int)std::round(this->green), (int)std::round(this->blue)};
    }

private:
    double red, green, blue;
};

#endif //GOL_COLOR_H
