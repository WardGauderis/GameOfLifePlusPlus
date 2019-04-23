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
}

void Window::init(uint32_t _xCells, uint32_t _yCells, const Color& color)
{
    this->setWindowTitle("GameOfLife++");
    this->setCentralWidget(root);

    root->setLayout(layout);

    xCells = _xCells;
    yCells = _yCells;

    cells = std::vector<Color>(xCells*yCells, color);
    properlyInitialized = true;
}

void Window::paintEvent([[maybe_unused]] QPaintEvent* event)
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
    (*this)(rand()%20, rand()%20) = Color(0, 0, 1);
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

Color& Window::operator[](uint32_t index)
{
    return cells[index];
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

    QPushButton* play = new QPushButton("Play", this);
    QPushButton* pause = new QPushButton("Pause", this);
    QPushButton* skipOne = new QPushButton("Next tick", this);
    QPushButton* goBackOne = new QPushButton("Previous tick", this);

    play->show();
    pause->show();
    skipOne->show();
    goBackOne->show();

    int size = 70;

    play->setFixedHeight(size);
    pause->setFixedHeight(size);
    skipOne->setFixedHeight(size);
    goBackOne->setFixedHeight(size);

    layout-> addWidget(play, 1, 1 ,1, 1);
    layout-> addWidget(pause, 1, 2 ,1, 1);
    layout-> addWidget(skipOne, 1, 3 ,1, 1);
    layout-> addWidget(goBackOne, 1, 0 ,1, 1);
    layout->setRowStretch(0, this->size().height() - size);
    layout->setRowStretch(0, size);

    connect(play, SIGNAL(pressed()), this, SLOT(onPlay()));
    connect(pause, SIGNAL(pressed()), this, SLOT(onPause()));
    connect(skipOne, SIGNAL(pressed()), this, SLOT(onNext()));
    connect(goBackOne, SIGNAL(pressed()), this, SLOT(onPrevious()));


    widgetsToDelete.emplace_back(play);
    widgetsToDelete.emplace_back(pause);
    widgetsToDelete.emplace_back(skipOne);
    widgetsToDelete.emplace_back(goBackOne);
}

void Window::onPlay()
{
    crState = play;
}

void Window::onPause() {
    crState = pause;
}

void Window::onNext() {
    crState = next;
}

void Window::onPrevious() {
    crState = previous;
}

Window::state Window::getState() const {
    return crState;
}


