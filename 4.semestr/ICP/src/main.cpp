/**
 * 
 * ICP Project
 * 
 * @file main.cpp
 * 
 * @brief Main file of program.
 * 
 * @author Michal Blažek <xblaze38>
 * 
*/

#include "mainwindow.h"

#include <QApplication>

/**
 * @brief The entry point of the program.
 * @param argc The number of command-line arguments.
 * @param argv An array of command-line arguments.
 * @return The exit status of the program.
 */
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowTitle("Roomba 1.0");
    w.show();
    return a.exec();
}
