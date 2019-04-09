//============================================================================
// @name        : 
// @author      : Mano Marichal
// @date        : 
// @version     : 
// @copyright   : BA1 Informatica - Mano Marichal - University of Antwerp
// @description : 
//============================================================================


#include "GUI.h"

//--------------------------COLOR CLASS----------------------------------------
ui::Color::Color(double r, double g, double b) {
    red = r;
    green = g;
    blue = b;
}

ui::Color& ui::Color::operator=(const ui::Color &other) {
    this->red = other.red;
    this->blue = other.blue;
    this->green = other.green;
    return *this;
}

QColor ui::Color::getAsQColor() {
    return QColor(std::round(this->red),
                  std::round(this->green),
                  std::round(this->blue));
}
//--------------------------CELL CLASS----------------------------------------
ui::Cell::Cell(const ui::Color &c) {
    color = c;
}

//--------------------------GRID CLASS----------------------------------------

ui::Grid::Grid(QWidget *parent) : QWidget(parent)
{
    this->setWindowTitle("GameOfLife++");
    this->resize(500, 500);
}

void ui::Grid::init(int gridWidth, int gridHeight, ui::Color color)
{
    assert(gridWidth > 0 && gridHeight > 0);

    height = gridHeight;
    width = gridHeight;

    for (int y=0 ; y<width ; ++y) {

        std::vector<Cell> cellRow;

        for (int x=0 ; x<width ; ++x) {

            cellRow.emplace_back(Cell(color));
        }

        cells.emplace_back(cellRow);
    }

    properlyInitialized = true;
}

void ui::Grid::paintEvent(QPaintEvent *event)
{
    assert(this->checkProperlyInitialized());

    QPainter p(this);

    for (int y=0 ; y<width ; ++y) {

        for (int x=0 ; x<width ; ++x) {

            QRect temp(x*500/width, y*500/height,500/width,500/height);
            p.fillRect(temp, cells[x][y].color.getAsQColor());
        }

    }
}

bool ui::Grid::checkProperlyInitialized()
{
    return properlyInitialized;
}

void ui::Grid::setCellColor(int x, int y, const ui::Color &c) {

    assert(x > 0 && x < width && y > 0 && y < height);

    cells[x][y].color = c;
}

ui::Color ui::Grid::getCellColor(int x, int y) {

    assert(x > 0 && x < width && y > 0 && y < height);

    return cells[x][y].color;
}

