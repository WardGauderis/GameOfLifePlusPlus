//============================================================================
// @name        : main.cpp
// @author      : Thomas Dooms
// @date        : 3/28/19
// @version     :
// @copyright   : BA1 Informatica - Thomas Dooms - University of Antwerp
// @description :
//============================================================================
#include <QApplication>
#include "system/simulation.h"

int main(int argc, char *argv[]) {
    QApplication application(argc, argv);
    Simulation simulation(argc, argv);

    simulation.simulate();

    return QApplication::exec();
}