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
UIGrid::UIGrid(QWidget *parent) : QWidget(parent)
{

}

void UIGrid::paintEvent([[maybe_unused]] QPaintEvent *event)
{

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

void UIGrid::setXCells(uint32_t xCells) {
    UIGrid::xCells = xCells;
}

void UIGrid::setYCells(uint32_t yCells) {
    UIGrid::yCells = yCells;
}

Color& UIGrid::operator()(uint32_t x, uint32_t y)
{
    return cells .at(y*xCells + x);
}
//--------------------------WINDOW CLASS----------------------------------------

Window::Window(QWidget *parent) : QMainWindow(parent)
{
}

void Window::init(uint32_t _xCells, uint32_t _yCells, const Color& color)
{
    this->setWindowTitle("GameOfLife++");
    this->setCentralWidget(root);
    this->setMinimumSize(750, 750);

    root->setLayout(layout);

    xCells = _xCells;
    yCells = _yCells;

    layout->addWidget(raster, 0, 0, 1, 10);

    raster->cells = std::vector<Color>(xCells*yCells, color);
    raster->setXCells(xCells);
    raster->setYCells(yCells);

    properlyInitialized = true;
}

void Window::delay(uint32_t ms)
{
    QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
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
    return raster->cells .at(y*xCells + x);
}

const Color& Window::operator()(uint32_t x, uint32_t y) const
{
    return raster->cells .at(y*xCells + x);
}

Color& Window::operator[](uint32_t index)
{
    return raster->cells [index];
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

    int size = 35;

    play->setFixedHeight(size);
    pause->setFixedHeight(size);
    skipOne->setFixedHeight(size);
    goBackOne->setFixedHeight(size);

    layout-> addWidget(play, 1, 4 ,1, 1);
    layout-> addWidget(pause, 1, 5 ,1, 1);
    layout-> addWidget(skipOne, 1, 6 ,1, 1);
    layout-> addWidget(goBackOne, 1, 3 ,1, 1);


    connect(play, SIGNAL(pressed()), this, SLOT(onPlay()));
    connect(pause, SIGNAL(pressed()), this, SLOT(onPause()));
    connect(skipOne, SIGNAL(pressed()), this, SLOT(onNext()));
    connect(goBackOne, SIGNAL(pressed()), this, SLOT(onPrevious()));


    widgetsToDelete.emplace_back(play);
    widgetsToDelete.emplace_back(pause);
    widgetsToDelete.emplace_back(skipOne);
    widgetsToDelete.emplace_back(goBackOne);

    // slider
    fancySlider = new QSlider(Qt::Orientation::Horizontal, this);
    fancySlider->setSingleStep(1);
    fancySlider->setTickInterval(10);
    fancySlider->setMaximum(110);
    fancySlider->setMinimum(0);
    connect(fancySlider, SIGNAL(valueChanged(int)), this, SLOT(setSliderValue(int)));
    layout->addWidget(fancySlider,1, 0, 1, 3);
}

void Window::onPlay()
{
    crState = play;
}

void Window::onPause()
{
    crState = pause;
}

void Window::onNext()
{
    crState = next;
}

void Window::onPrevious()
{
    crState = previous;
}

Window::state Window::getState()
{
    state temp = crState;
    if(crState == next or crState == previous) crState = pause;
    return temp;
}

void Window::closeEvent([[maybe_unused]] QCloseEvent *event)
{
    crState = quit;
}

void Window::setSliderValue(int val)
{
    fancySlider->setToolTip(QString(std::to_string(sliderValue).c_str()));
    sliderValue = val;
    if (sliderValue > 100 and val < sliderValue)
    {
        QMessageBox msgBox;
        msgBox.setText("ARE YOU SURE YOU WANT TO GO MAXIMUM POWER??");
        msgBox.setStandardButtons(QMessageBox::Ok);
        [[maybe_unused]] int ret =  msgBox.exec();
    }
}

int Window::getSliderValue() const
{
    return sliderValue;
}


