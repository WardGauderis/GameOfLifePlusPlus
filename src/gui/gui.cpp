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
    if (!canPaint) return;
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
            painter.fillRect(std::floor(xPos), std::floor(yPos), std::ceil(celWidth), std::ceil(celHeight), converter.at(cells.at(y*xCells + x)));
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


void UIGrid::mousePressEvent(QMouseEvent *event)
{
    if (event->button() != Qt::LeftButton or !canChange) return;

    double celWidth  = double(this->size().width() ) / double(xCells);
    double celHeight = double(this->size().height()) / double(yCells);

    int x = std::floor(event->pos().x()/celWidth);
    int y = std::floor(event->pos().y()/celHeight);

    uint32_t index = y*xCells + x;
    const auto iter = ++converter.find(cells.at(index));
    if(iter == end(converter)) cells.at(y * xCells + x) = begin(converter)->first;
    else cells.at(y * xCells + x) = iter->first;

    this->repaint();
}

//--------------------------WINDOW CLASS----------------------------------------

Window::Window(QWidget *parent) : QMainWindow(parent)
{
}


//const std::map<char, Color> &colormap
void Window::initCA(uint32_t _xCells, uint32_t _yCells, const std::map<char, Color>& converter)
{
    this->setMinimumSize(750, 750);

    xCells = _xCells;
    yCells = _yCells;

    layout->addWidget(raster, 0, 0, 1, 10);
    raster->cells = CA::getData(0);
    raster->converter = converter;

    raster->setXCells(xCells);
    raster->setYCells(yCells);

    raster->initialized = true;
    raster->canPaint = true;
}

void Window::paintEvent([[maybe_unused]] QPaintEvent *event)
{
    if (!(raster->canChange)) labelTicksPassed->setText(("Ticks passed:\n" + std::to_string(ticksPassed)).c_str());
}

void Window::processEverything()
{
    QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
    QTime current = QTime::currentTime();
    if(prev.addMSecs(20) < current)
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
        [[maybe_unused]] int res = system("sleep 0.01");
    }
}

char& Window::operator[](uint32_t index) const
{
    return raster->cells.at(index);
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

    raster->canChange = true;
}

void Window::showPlayButton()
{
    raster->canChange = false;

    for (auto &widget:editWidgets)
    {
        widget->hide();
    }

//    //ICONS
//
//    QPixmap icons("./icons/mediaplayer.png");
//    icons.save("hallo");
//    icons = icons.scaledToHeight(size);
//
//    int w = icons.size().width()/3;
//    int h = icons.size().height()/3;
//
//    QRect playRect(w, 0, w, h);
//    QRect pauseRect(w, 2*h, w, h);
//    QRect nextRect(w, h, w, h);
//    QRect prevRect(w, h, w, h);
//
//    QIcon playIcon(icons.copy(playRect));
//    QIcon pauseIcon(icons.copy(pauseRect));
//    QIcon nextIcon(icons.copy(prevRect));
//    QIcon prevIcon(icons.copy(nextRect));
//
//    playBtn->setIcon(playIcon);
//    pauseBtn->setIcon(pauseIcon);
//    skipOne->setIcon(nextIcon);
//    goBackOne->setIcon(prevIcon);
//
//    playBtn->setIconSize(QSize(this->width()/10, size));
//    pauseBtn->setIconSize(QSize(this->width()/10, size));
//    skipOne->setIconSize(QSize(this->width()/10, size));
//    goBackOne->setIconSize(QSize(this->width()/10, size));

    //BUTTONS
    playBtn = new QPushButton("Play", this);
    pauseBtn = new QPushButton("Pause", this);
    QPushButton* skipOne = new QPushButton("Next tick", this);
    QPushButton* goBackOne = new QPushButton("Previous tick", this);

//    playBtn = new QPushButton(this);
//    pauseBtn = new QPushButton(this);
//    QPushButton* skipOne = new QPushButton(this);
//    QPushButton* goBackOne = new QPushButton(this);

    playBtn->show();
    pauseBtn->show();
    skipOne->show();
    goBackOne->show();

    playBtn->setFixedHeight(size);
    pauseBtn->setFixedHeight(size);
    skipOne->setFixedHeight(size);
    goBackOne->setFixedHeight(size);

    layout-> addWidget(playBtn, 1, 5 ,1, 1);
    layout-> addWidget(pauseBtn, 1, 4 ,1, 1);
    layout-> addWidget(skipOne, 1, 6 ,1, 1);
    layout-> addWidget(goBackOne, 1, 2 ,1, 1);


    connect(playBtn, SIGNAL(pressed()), this, SLOT(onPlay()));
    connect(pauseBtn, SIGNAL(pressed()), this, SLOT(onPause()));
    connect(skipOne, SIGNAL(pressed()), this, SLOT(onNext()));
    connect(goBackOne, SIGNAL(pressed()), this, SLOT(onPrevious()));


    playBackBtn = new QPushButton("Play Back", this);
    playBackBtn->setFixedHeight(size);
    playBackBtn->show();
    layout-> addWidget(playBackBtn, 1, 3 ,1, 1);
    connect(playBackBtn, SIGNAL(pressed()), this, SLOT(onPlayBack()));

    fancySlider = new QSlider(Qt::Orientation::Horizontal, this);
    fancySlider->setSingleStep(1);
    fancySlider->setTickInterval(10);
    fancySlider->setMaximum(110);
    fancySlider->setMinimum(0);
    connect(fancySlider, SIGNAL(valueChanged(int)), this, SLOT(setSliderValue(int)));
    layout->addWidget(fancySlider,1, 0, 1, 2);

    labelTicksPassed = new QLabel(this);
    labelTicksPassed->setText(("Ticks passed:\n" + std::to_string(ticksPassed)).c_str());
    labelTicksPassed->show();
    layout-> addWidget(labelTicksPassed, 1, 8 ,1, 1);

    QPushButton * exportButton = new QPushButton("Export current\nstate as layout", this);
    exportButton->setFixedHeight(size);
    exportButton->show();
    layout-> addWidget(exportButton, 1, 7 ,1, 1);
    connect(exportButton, SIGNAL(pressed()), this, SLOT(onExportLayout()));
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

// ------------------------------------- SLOTS, GETTERS AND SETTERS ----------------------------------------------
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

void Window::onPlayBack()
{
    crState = playback;
}

Window::state Window::getState()
{
    state temp = crState;
    if(crState == next or crState == previous) crState = pause;
    return temp;
}

void Window::closeEvent([[maybe_unused]] QCloseEvent *event)
{
    exit(0);
}

void Window::setSliderValue(int val)
{
    fancySlider->setToolTip(QString(std::to_string(sliderValue).c_str()));
    sliderValue = val;
}

int Window::getSliderValue() const
{
    return sliderValue;
}
const std::vector<char>& Window::getStart() const
{
    return raster->cells;
}

void Window::onLoadIniFile()
{
    filename = askString(defaultFileName);
    if (!filename.empty()) initialized = true;
}

void Window::onLoadLayout()
{
    std::string fileName = askString(CAIO::layout);
    raster->cells = CAIO::parseLayout(fileName, raster->xCells, raster->yCells, raster->converter.size());
}
void Window::onExportLayout()
{
    std::string fileName = askString(CAIO::layout);
    if (fileName.empty()) return;
    CAIO::exportCA(raster->cells, raster->xCells, raster->yCells, fileName);
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

uint32_t Window::getTicksPassed() const {
    return ticksPassed;
}

void Window::setTicksPassed(uint32_t ticksPassed) {
    Window::ticksPassed = ticksPassed;
}
