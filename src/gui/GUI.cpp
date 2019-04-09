//============================================================================
// @name        : 
// @author      : Mano Marichal
// @date        : 
// @version     : 
// @copyright   : BA1 Informatica - Mano Marichal - University of Antwerp
// @description : 
//============================================================================


#include "GUI.h"

ui::Color::Color(double r, double g, double b) {
    red = r;
    green = g;
    blue = b;
}

ui::Grid::Grid(QWidget *parent, int gridWidth, int gridHeight) : QWidget(parent)
{
    assert(gridWidth > 0 && gridHeight > 0);

    height = gridHeight;
    width = gridHeight;

    properlyInitialized = true;
}

void ui::Grid::paintEvent(QPaintEvent *event)
{
    assert(this->properlyInitialized);

    QPainter p(this);
    QRect r(10,10,10,10);
    p.drawRect(r);
}

bool ui::Grid::checkProperlyInitialized()
{
    return properlyInitialized;
}

int ui::Grid::run()
{

}
