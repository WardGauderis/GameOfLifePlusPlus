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


#include <random>
#include <iostream>
#include <assert.h>
#include <vector>
#include <cmath>
#include "color.h"

class Window: public QWidget
{

    Q_OBJECT

public:
    /*
    * Constructor, gridwidth and height indicate the amount of cells that the grid is supposed to have
    */
    explicit Window(QWidget* parent = nullptr);
    /*
    * Destructor
    */
    ~Window() override;

    /*
    * Used to initialize the grid
    */
    void init(uint32_t _xCells, uint32_t _yCells, const Color& color);

    /*
    * Used to check if the grid is properly initialized and can be used
    */
    bool checkProperlyInitialized();

    const Color& operator()(uint32_t x, uint32_t y) const;
    Color& operator()(uint32_t x, uint32_t y);

    static void delay(uint32_t ms);

protected:
    /*
    * The paint event for our grid
    */
    void paintEvent(QPaintEvent* event) override;

private:
    /*
    * Creates helper buttons
    */
    void createButtons();


    uint32_t xCells;
    uint32_t yCells;

    bool properlyInitialized = false;

    std::vector<QWidget*> widgetsToDelete;
    std::vector<Color> cells;
};



#endif //GOL_GUI_H
