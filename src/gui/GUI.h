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
#include <QLabel>
#include <QPainter>
#include <QMainWindow>

#include <assert.h>
#include <vector>

namespace ui {

    /*
     * Struct to represent a color
     */
    struct Color {
        double red, green, blue;
        Color(double r, double g, double b);
    };

    /*
     * Struct to represent cells in our application
     */
    struct Cell {
        ui::Color color;
    };

    typedef std::vector<std::vector<Cell>> gridOfCells;

    class Grid: public QWidget {

        Q_OBJECT

    private:

        int width;
        int height;

        bool properlyInitialized=false;

        QWidget *mainWidget = nullptr;

    public:
        /*
         * Constructor, gridwidth and height indicate the amount of cells that the grid is supposed to have
         */
        explicit Grid(QWidget *parent = 0, int gridWidth=100, int gridHeight=100);
        /*
         * Destructor
         */
        ~Grid()= default;
        /*
         * Used to check if the grid is properly initialized and can be used
         */
        bool checkProperlyInitialized();
        /*
         * Runs the current Application
         */
        int run();

    protected:
        /*
         * The paint event for our grid
         */
        void paintEvent(QPaintEvent *event);


    };

}


#endif //GOL_GUI_H
