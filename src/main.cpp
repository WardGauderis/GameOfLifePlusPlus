//============================================================================
// @name        : main.cpp
// @author      : Thomas Dooms
// @date        : 3/28/19
// @version     : 
// @copyright   : BA1 Informatica - Thomas Dooms - University of Antwerp
// @description : 
//============================================================================
#include <QApplication>
#include "../src/gui/GUI.h"


int main(int argc, char* argv[])
{
    // deze 4 lijnen laten staan
    QApplication a(argc, argv);

    ui::Grid mainwindow;

    mainwindow.init(100,100, ui::Color(255,0,0));

    mainwindow.show();

    return a.exec();
}