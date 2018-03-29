/**
 * @file main.cpp This file contains the main function.
 */

#include <QCoreApplication>

#include "backend.h"

/**
 * @brief main The main entry point of the ipc-backend application.
 * @param argc Number of command line arguments.
 * @param argv Vector of command line arguments.
 * @return 0 for successful execution.
 */
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    Backend backend(&a);
    backend.run();
    QObject::connect(&backend, SIGNAL(quit()), &a, SLOT(quit()));

    return a.exec();
}
