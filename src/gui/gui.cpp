//============================================================================
// @name        : 
// @author      : Mano Marichal
// @date        : 
// @version     : 
// @copyright   : BA1 Informatica - Mano Marichal - University of Antwerp
// @description : 
//============================================================================

#include <QtCore/QTime>
#include "gui.h"

//--------------------------WINDOW CLASS----------------------------------------

Window::Window(QWidget *parent) : QWidget(parent)
{
    this->setWindowTitle("GameOfLife++");
}

void Window::init(uint32_t _xCells, uint32_t _yCells, const Color& color)
{
    xCells = _xCells;
    yCells = _yCells;

    cells = std::vector<Color>(xCells*yCells, color);
    properlyInitialized = true;
}

void Window::paintEvent(QPaintEvent* event)
{
    assert(this->checkProperlyInitialized());

    uint32_t celWidth  = std::ceil( double(this->size().width() ) / double(xCells) );
    uint32_t celHeight = std::ceil( double(this->size().height()) / double(yCells) );

    QPainter painter(this);

    for (uint32_t x = 0; x < xCells; ++x)
        for (uint32_t y = 0; y < yCells; ++y)
        {
            QRect temp(x * celWidth, y * celHeight, celWidth, celHeight);
            painter.fillRect(temp, (*this)(x,y));
        }
}


void Window::delay(uint32_t ms)
{
    QTime stopTime = QTime::currentTime().addMSecs(ms);
    while (QTime::currentTime() < stopTime)
    {
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
        system("sleep 0.01");
    }
}


bool Window::checkProperlyInitialized()
{
    return properlyInitialized;
}

Color& Window::operator()(uint32_t x, uint32_t y)
{
    return cells.at(y*xCells + x);
}

const Color& Window::operator()(uint32_t x, uint32_t y) const
{
    return cells.at(y*xCells + x);
}

Window::~Window()
{
    for (auto widget:widgetsToDelete) delete widget;
}


void Window::createButtons()
{

}