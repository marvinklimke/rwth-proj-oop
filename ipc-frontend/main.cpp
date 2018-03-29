/**
 * @file main.cpp This file contains the main function.
 */

#include <QCoreApplication>

#include "frontend.h"

/**
 * @brief main The main entry point of the ipc-frontend application.
 * @param argc Number of command line arguments.
 * @param argv Vector of command line arguments.
 * @return 0 for successful execution.
 */
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    Frontend frontend(&a);
    frontend.run();
    QObject::connect(&frontend, SIGNAL(quit()), &a, SLOT(quit()));

    return a.exec();
}
