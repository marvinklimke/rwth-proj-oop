/**
 * @file frontend/main.cpp
 * @brief Home of the main entry point.
 */

#include <QApplication>

#include "mainwindow.h"

/**
 * @brief The main method.
 * @param argc Number of command line arguments.
 * @param argv Vector of command line arguments.
 * @return true for successful execution, error-code otherwise.
 */
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}


