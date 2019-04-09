//============================================================================
// @name        : 
// @author      : Mano Marichal
// @date        : 
// @version     : 
// @copyright   : BA1 Informatica - Mano Marichal - University of Antwerp
// @description : 
//============================================================================

#include "gui.h"

//--------------------------WINDOW CLASS----------------------------------------

Window::Window(QWidget *parent) : QWidget(parent)
{
    this->setWindowTitle("GameOfLife++");
}

void Window::init(uint32_t _width, uint32_t _height, const Color& color)
{
    width = _width;
    height = _height;

    cells = std::vector<Color>(width*height, color);
    properlyInitialized = true;
}

void Window::paintEvent(QPaintEvent* event)
{
    assert(this->checkProperlyInitialized());

    QPainter p(this);

    for (uint32_t x = 0; x < width; ++x)
        for (uint32_t y = 0; y < height; ++y)
        {
            QRect temp(x*500/width, y*500/height,500/width,500/height);
            p.fillRect(temp, (*this)(x,y));
        }
}

void Window::execute(uint32_t ticks)
{

    for (uint32_t i = 0; i < ticks; ++i)
    {
        int x = rand() % width;
        int y = rand() % height;

        (*this)(x, y) = Color(0,0,255);

        this->repaint();

        system("sleep 0.5");
    }
}

void Window::simulate()
{
    while (true)
    {
        bool ok;
        int ticks = QInputDialog::getInt(this, tr("#ticks to execute the simulate for"), tr("Amount:"), 10, 0, 100000, 2, &ok);
        if(ok) this->execute(ticks);
        else return;
    }
}


bool Window::checkProperlyInitialized()
{
    return properlyInitialized;
}

Color& Window::operator()(uint32_t x, uint32_t y)
{
    return cells.at(y*width + x);
}

const Color& Window::operator()(uint32_t x, uint32_t y) const
{
    return cells.at(y*width + x);
}

Window::~Window()
{
    for (auto widget:widgetsToDelete) delete widget;
    delete this;
}


void Window::createButtons()
{

}