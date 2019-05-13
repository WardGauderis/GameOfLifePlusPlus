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

QTime Window::prev = QTime::currentTime();

UIGrid::UIGrid(QWidget *parent) : QWidget(parent)
{

}

void UIGrid::paintEvent([[maybe_unused]] QPaintEvent *event)
{
    if (!initialized) return;
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
            painter.fillRect(std::floor(xPos), std::floor(yPos), std::ceil(celWidth),  std::ceil(celHeight),cells.at(y*xCells + x));
            yPos += celHeight;
        }
        xPos += celWidth;
    }
    shouldRepaint = false;
}

void UIGrid::setXCells(uint32_t xCells) {
    UIGrid::xCells = xCells;
}

void UIGrid::setYCells(uint32_t yCells) {
    UIGrid::yCells = yCells;
}

bool& UIGrid::getRepaint() { return shouldRepaint; }

/*
void UIGrid::mousePressEvent(QMouseEvent *event)
{

    if (event->button() == Qt::LeftButton)
    {
        double celWidth  = double(this->size().width() ) / double(xCells);
        double celHeight = double(this->size().height()) / double(yCells);

        int x = std::floor(event->pos().x()/celWidth);
        int y = std::floor(event->pos().y()/celHeight);
    }
}
*/
//--------------------------WINDOW CLASS----------------------------------------

Window::Window(QWidget *parent) : QMainWindow(parent)
{
}


//const std::map<char, Color> &colormap
void Window::initCA(uint32_t _xCells, uint32_t _yCells)
{
    this->setMinimumSize(750, 750);

    xCells = _xCells;
    yCells = _yCells;

    
    layout->addWidget(raster, 0, 0, 1, 10);

    raster->cells = std::vector<Color>(xCells*yCells, Color(0, 0 ,0));

    raster->setXCells(xCells);
    raster->setYCells(yCells);
    raster->initialized = true;
}

void Window::processEverything()
{
    QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
    QTime current = QTime::currentTime();
    if(prev.addMSecs(20) < current and raster->getRepaint())
    {
        prev = current;
        repaint();
    }
}

void Window::delay(uint32_t ms)
{
    processEverything();
    QTime stopTime = QTime::currentTime().addMSecs(ms);
    while (QTime::currentTime() < stopTime)
    {
        processEverything();
    }
}

const Color& Window::getColor(uint32_t x, uint32_t y) const
{
    return raster->cells.at(y*xCells + x);
}

void Window::setColor(uint32_t x, uint32_t y, const Color& color)
{
    raster->getRepaint() = true;
    raster->cells.at(y*xCells + x) = color;
}

void Window::setColor(uint32_t i, const Color& color)
{
    raster->getRepaint() = true;
    raster->cells.at(i) = color;
}



Window::~Window()
{
    for (auto widget:widgetsToDelete) delete widget;
    delete this;
}


void Window::createIniButtons()
{

    this->setWindowTitle("GameOfLife++");
    this->setCentralWidget(root);
    root->setLayout(layout);
    root->show();

    QLabel* hallo = new QLabel(this);
    hallo->setText("Welcome To GameOfLife++");
    hallo->show();
    layout-> addWidget(hallo, 0, 0 ,1, 1);
    iniWidgets.emplace_back(hallo);

    //configuratiebestand
    QPushButton * chooseFile = new QPushButton("choose .ini \nfile", this);
    chooseFile->setFixedHeight(size);
    chooseFile->show();
    layout-> addWidget(chooseFile, 3, 0 ,1, 1);
    iniWidgets.emplace_back(chooseFile);
    connect(chooseFile, SIGNAL(pressed()), this, SLOT(onLoadIniFile()));

}

void Window::createEditButtons()
{
    for (auto &widget:iniWidgets)
    {
        widget->hide();
    }

    QPushButton * startSim = new QPushButton("Start the simulation!", this);
    startSim->setFixedHeight(size);
    startSim->show();
    layout-> addWidget(startSim, 1, 0 ,1, 5);
    iniWidgets.emplace_back(startSim);
    connect(startSim, SIGNAL(pressed()), this, SLOT(onStartSimulation()));
    editWidgets.emplace_back(startSim);

    QPushButton * loadLayout = new QPushButton("Load layout", this);
    loadLayout->setFixedHeight(size);
    loadLayout->show();
    layout-> addWidget(loadLayout, 1, 5 ,1, 5);
    iniWidgets.emplace_back(loadLayout);
    connect(loadLayout, SIGNAL(pressed()), this, SLOT(onLoadLayout()));
    editWidgets.emplace_back(loadLayout);
}

void Window::showPlayButton()
{
    for (auto &widget:editWidgets)
    {
        widget->hide();
    }

    playBtn = new QPushButton("Play", this);
    pauseBtn = new QPushButton("Pause", this);
    QPushButton* skipOne = new QPushButton("Next tick", this);
    QPushButton* goBackOne = new QPushButton("Previous tick", this);

    playBtn->show();
    pauseBtn->hide();
    skipOne->show();
    goBackOne->show();

    playBtn->setFixedHeight(size);
    pauseBtn->setFixedHeight(size);
    skipOne->setFixedHeight(size);
    goBackOne->setFixedHeight(size);

    layout-> addWidget(playBtn, 1, 4 ,1, 1);
    layout-> addWidget(pauseBtn, 1, 4 ,1, 1);
    layout-> addWidget(skipOne, 1, 5 ,1, 1);
    layout-> addWidget(goBackOne, 1, 3 ,1, 1);


    connect(playBtn, SIGNAL(pressed()), this, SLOT(onPlay()));
    connect(pauseBtn, SIGNAL(pressed()), this, SLOT(onPause()));
    connect(skipOne, SIGNAL(pressed()), this, SLOT(onNext()));
    connect(goBackOne, SIGNAL(pressed()), this, SLOT(onPrevious()));


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
    playBtn->hide();
    pauseBtn->show();
    crState = play;
}

void Window::onPause()
{
    playBtn->show();
    pauseBtn->hide();
    crState = pause;
}

void Window::onNext()
{
    playBtn->show();
    pauseBtn->hide();
    crState = next;
}

void Window::onPrevious()
{
    playBtn->show();
    pauseBtn->hide();
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

    if (sliderValue > 100 and val > sliderValue)
    {
        QMessageBox msgBox;
        msgBox.setText("ARE YOU SURE YOU WANT TO GO MAXIMUM POWER??");
        msgBox.setStandardButtons(QMessageBox::Ok);
        [[maybe_unused]] int ret =  msgBox.exec();
    }
    sliderValue = val;
}

int Window::getSliderValue() const
{
    return sliderValue;
}

std::string Window::askString(std::string example)
{
    bool ok;
    QString text = QInputDialog::getText(this, tr("enter your filename"),
                                         tr("filename"), QLineEdit::Normal,
                                         QString(example.c_str()), &ok);

    if (ok && !text.isEmpty())
        return text.toStdString();
    else return "";
}

double Window::askDouble(double min, double max, double step, double example)
{
    bool ok;
    double val = QInputDialog::getDouble(this, tr(""),
                                       tr("New value:"), example, min, max, step, &ok);
    if (ok)
    {
        return val;
    }
    else
    {
        return -1;
    }
}

void Window::onLoadIniFile()
{
    filename = askString("./input/thomas_test2/test.ini");
    if (!filename.empty()) initialized = true;
}

void Window::onLoadLayout()
{
    layoutFilename = askString("./input/thomas_test2/test.csv");
}
void Window::onStartSimulation()
{
    initialized = true;
}
bool Window::isInitialized() const {
    return initialized;
}

const std::string &Window::getFilename() const {
    return filename;
}

void Window::setInitialized(bool initialized) {
    Window::initialized = initialized;
}

const std::string &Window::getLayoutFilename() const {
    return layoutFilename;
}
