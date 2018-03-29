/**
 * @file backend/main.cpp
 * @brief Home of the main entry point.
 */

#include <QCoreApplication>

#include "backend.h"

/**
 * @brief The main method.
 * @param argc Number of command line arguments.
 * @param argv Vector of command line arguments.
 * @return 0 for successful execution, error-code otherwise.
 */
int main(int argc, char *argv[])
{
    Backend *backend = new Backend();
    QCoreApplication a(argc, argv);
    QObject::connect(backend, SIGNAL(quit()), &a, SLOT(quit()));

    return a.exec();
}
