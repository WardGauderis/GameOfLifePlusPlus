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


#include <random>
#include <iostream>
#include <assert.h>
#include <vector>
#include <cmath>

#include "color.h"

class UIGrid: public QWidget
{
Q_OBJECT

public:
    explicit UIGrid(QWidget* parent = nullptr);
    void paintEvent(QPaintEvent* event) override;

    void setXCells(uint32_t xCells);
    void setYCells(uint32_t yCells);

    std::vector<Color> cells;

    bool& getRepaint();

private:
    uint32_t xCells;
    uint32_t yCells;

    bool shouldRepaint = false;
};

class Window: public QMainWindow
{

Q_OBJECT

public:
    enum state {play, pause, next, previous, quit};

    explicit Window(QWidget* parent = nullptr);
    ~Window() override;

    void initCA(uint32_t _xCells, uint32_t _yCells);

    const Color& getColor(uint32_t x, uint32_t y) const;
    void setColor(uint32_t x, uint32_t y, const Color& color);
    void setColor(uint32_t i, const Color& color);

    void delay(uint32_t ms);
    void processEverything();

    void showPlayButton();
    state getState();

    void closeEvent(QCloseEvent *event);

    int getSliderValue() const;

    std::string askString(std::string example);
    double askDouble(double min, double max, double step, double example);

    bool isInitialized() const;

    const std::string &getFilename() const;

    void createIniButtons();
    void createEditButtons();

    void setInitialized(bool initialized);

    const std::string &getLayoutFilename() const;

protected:

private:

    const int size = 35;

    bool initialized = false;

    std::string filename;
    std::string layoutFilename;

    uint32_t xCells;
    uint32_t yCells;

    state crState = pause;

    QWidget *root = new QWidget(this);
    QGridLayout *layout = new QGridLayout;
    UIGrid *raster = new UIGrid(root);
    QPushButton* playBtn;
    QPushButton* pauseBtn;

    std::vector<QWidget*> widgetsToDelete;
    std::vector<QWidget*> iniWidgets;
    std::vector<QWidget*> editWidgets;

    int sliderValue = 1;
    QSlider* fancySlider;

    static QTime prev;
    bool shouldRepaint = false;

private slots:
    void onPlay();
    void onPause();
    void onNext();
    void onPrevious();
    void setSliderValue(int val);
    void onLoadIniFile();
    void onStartSimulation();
    void onLoadLayout();
};



#endif //GOL_GUI_H
