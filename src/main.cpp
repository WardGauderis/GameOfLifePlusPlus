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


// dit mag weg als ik dit ben vergeten
#include "input-output/parser.h"


int main(int argc, char* argv[])
{
    TM test = Parser::parseTM("../input/tm.json");


    // deze 4 lijnen laten staan
    QApplication a(argc, argv);

    ui::Grid mainwindow;

    mainwindow.init(20,20, ui::Color(255,0,0));

    mainwindow.show();

    mainwindow.simulation();

    return a.exec();
}