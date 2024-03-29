//============================================================================
// @name        :
// @author      : Mano Marichal
// @date        :
// @version     :
// @copyright   : BA1 Informatica - Mano Marichal - University of Antwerp
// @description :
//============================================================================


#ifndef GOL_GUI_H
#define GOL_GUI_H

#include <QApplication>
#include <QRect>
#include <QPushButton>
#include <QPainter>
#include <QMainWindow>
#include <QInputDialog>
#include <QToolBar>
#include <QLayout>
#include <QHBoxLayout>
#include <QSpacerItem>
#include <QSlider>
#include <QMessageBox>
#include <QLabel>
#include <QMouseEvent>
#include <qpixmap.h>
#include <QIcon>


#include <random>
#include <iostream>
#include <assert.h>
#include <vector>
#include <cmath>

#include "color.h"
#include "../input-output/caio.h"
#include "../automata/ca.h"

class UIGrid: public QWidget
{
Q_OBJECT

public:
    explicit UIGrid(QWidget* parent = nullptr);
    void paintEvent(QPaintEvent* event) override;

    void setXCells(uint32_t xCells);
    void setYCells(uint32_t yCells);

    std::vector<char> cells;
    std::map<char, Color> converter;

    bool& getRepaint();

    bool initialized;
    bool canPaint = false;
    bool canChange = true;

    void mousePressEvent(QMouseEvent *event);

    uint32_t xCells;
    uint32_t yCells;
};

class Window: public QMainWindow
{

Q_OBJECT

public:
    enum state {play, pause, next, previous, quit, playback};

    explicit Window(QWidget* parent = nullptr);
    ~Window() override;

    void initCA(uint32_t _xCells, uint32_t _yCells, const std::map<char, Color> &caMap);

    char& operator[](uint32_t index) const;

    void delay(uint32_t ms);
    void processEverything();

    void showPlayButton();
    state getState();

    void closeEvent(QCloseEvent *event);

    int getSliderValue() const;
    const std::vector<char>& getStart() const;

    std::string askString(std::string example);
    double askDouble(double min, double max, double step, double example);

    bool isInitialized() const;

    const std::string &getFilename() const;

    void createIniButtons();
    void createEditButtons();

    void setInitialized(bool initialized);

    const std::string &getLayoutFilename() const;

    void paintEvent([[maybe_unused]] QPaintEvent *event);

    uint32_t getTicksPassed() const;
    void setTicksPassed(uint32_t ticksPassed);

    std::string defaultFileName;
private:

    const int size = 35;

    bool initialized = false;

    std::string filename;

    uint32_t xCells;
    uint32_t yCells;

    state crState = pause;

    QWidget *root = new QWidget(this);
    QGridLayout *layout = new QGridLayout;
    UIGrid *raster = new UIGrid(root);

    QPushButton* playBtn;
    QPushButton* pauseBtn;
    QPushButton* playBackBtn;

    QLabel* labelTicksPassed;
    uint32_t ticksPassed = 0;

    std::vector<QWidget*> widgetsToDelete;
    std::vector<QWidget*> iniWidgets;
    std::vector<QWidget*> editWidgets;

    int sliderValue = 1;
    QSlider* fancySlider;

    static QTime prev;

private slots:
    void onPlay();
    void onPause();
    void onNext();
    void onPrevious();
    void setSliderValue(int val);
    void onLoadIniFile();
    void onStartSimulation();
    void onLoadLayout();
    void onPlayBack();
    void onExportLayout();
};



#endif //GOL_GUI_H
