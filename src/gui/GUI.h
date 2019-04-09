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

#include <random>
#include <unistd.h>
#include <iostream>
#include <assert.h>
#include <vector>
#include <cmath>

namespace ui {

    /*
     * Class to represent a color
     */
    class Color {

    public:
        double red, green, blue;
        /*
         * Constructors
         */
        Color()= default;
        Color(double r, double g, double b);
        /*
         * Convert the color to a Qcolor object (loss of precision due to Qcolor using int values)
         */
        QColor getAsQColor();
        /*
         * Operators
         */
        ui::Color& operator=(const Color &other);
        /*
         * Destructor
         */
        ~Color()= default;
    };

    /*
     * Class to represent cells in our application
     */
    class Cell {

    public:
        ui::Color color;
        /*
         * Constructors
         */
        explicit Cell(const ui::Color &c);
        /*
         * Destructor
         */
        ~Cell()= default;
    };

    typedef std::vector<std::vector<Cell>> gridOfCells;

    class Grid: public QWidget {

        Q_OBJECT

    private:

        int width;
        int height;

        bool properlyInitialized=false;

        std::vector<QWidget*> widgetsToDelete;

        gridOfCells cells;

    public:
        /*
         * Constructor, gridwidth and height indicate the amount of cells that the grid is supposed to have
         */
        explicit Grid(QWidget *parent = 0);
        /*
         * Destructor
         */
        ~Grid();
        /*
         * Used to initialize the grid
         */
        void init(int gridWidth, int gridHeight, ui::Color color);
        /*
         * Used to check if the grid is properly initialized and can be used
         */
        bool checkProperlyInitialized();
        /*
         * Function to set the color of a cell
         */
        void setCellColor(int x, int y, const ui::Color &c);
        /*
         * Function to get the color of a cell
         */
        ui::Color getCellColor(int x, int y);
        /*
         * Play event
         */
        void onPlay();


    protected:
        /*
         * The paint event for our grid
         */
        void paintEvent(QPaintEvent *event);


    };

}


#endif //GOL_GUI_H
