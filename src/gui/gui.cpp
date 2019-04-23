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

Window::Window(QWidget *parent) : QMainWindow(parent)
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

    double celWidth  = double(this->size().width() ) / double(xCells);
    double celHeight = double(this->size().height()) / double(yCells);

    double xPos = 0;
    double yPos = 0;

    QPainter painter(this);

    for (uint32_t x = 0; x < xCells; ++x)
    {
        yPos = 0;
        for (uint32_t y = 0; y < yCells; ++y)
        {
            QRect temp(std::floor(xPos), std::floor(yPos), std::floor(xPos + celWidth), std::floor(yPos + celHeight));
            painter.fillRect(temp, (*this)(x,y));
            yPos += celHeight;
        }
        xPos += celWidth;
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

void Window::update()
{
    (*this)(rand()%20, rand()%20) = Color(0, 0, 255);
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
    delete this;
}


void Window::createButtons()
{

}

void Window::showPlayButton()
{

    QWidget *menubar = new QWidget(this);

    QHBoxLayout *cute = new QHBoxLayout;

    QPushButton* play = new QPushButton("play", this);
    widgetsToDelete.emplace_back(play);

    QPushButton* pause = new QPushButton("pause", this);
    widgetsToDelete.emplace_back(play);

    cute->addWidget(play);
    cute->addWidget(pause);

    menubar->setLayout(cute);
    menubar->show();

    connect(play, SIGNAL(pressed()), this, SLOT(onPlay()));
}

void Window::onPlay()
{
    playBtnClicked = true;
}

bool Window::isPlayBtnClicked() const {
    return playBtnClicked;
}
