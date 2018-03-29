/**
 * @file backend.cpp Definition of the Backend class' functions.
 */

#include "backend.h"

#include <unistd.h>

using namespace std;

Backend::Backend(QObject *parent) :
    QObject(parent),
    notifier(STDIN_FILENO, QSocketNotifier::Read),
    qin(stdin)
{
    connect(&notifier, SIGNAL(activated(int)), this, SLOT(readCommand()));

    new OopAdaptor(this);
    QDBusConnection::sessionBus().registerObject("/de/rwth/iss/oop/backend", this);
    iface = new de::rwth::iss::oop("", "/de/rwth/iss/oop/frontend", QDBusConnection::sessionBus(), this);

    connect(iface, SIGNAL(sendPixelRow(int,QList<bool>)), this, SLOT(sendPixelRowSlot(int,QList<bool>)));
    connect(iface, SIGNAL(configCRC(QList<bool>)), this, SLOT(configCRCSlot(QList<bool>)));
    connect(iface, SIGNAL(configECC(int)), this, SLOT(configECCSlot(int)));
    connect(iface, SIGNAL(configMapping(int)), this, SLOT(configMappingSlot(int)));
    connect(iface, SIGNAL(configSNR(double)), this, SLOT(configSNRSlot(double)));
    connect(iface, SIGNAL(frontendStatus(bool)), this, SLOT(frontendStatusSlot(bool)));
}

void Backend::readCommand()
{
    QString line = qin.readLine();
    if(line == "quit") {
        cout << "Backend shutting down." << endl;
        emit backendStatus(false);
        emit quit();
        return;
    } else if(line == "help") {
        cout << "This is the dummy Backend of the oop-project." << endl << endl
             << "currently availible commands:" << endl
             << "help   : Display this page." << endl
             << "quit   : Quit the application." << endl;
    } else if(line != ""){
        cout << "Unknown command: " << line.toStdString() << endl
             << "Type help to list all availible commands." << endl;
    }
    cout << PROMPT << flush;
}

void Backend::run()
{
    emit backendStatus(true);
    cout << "Backend started up." << endl;
    cout << PROMPT << flush;
}

void Backend::sendPixelRowSlot(int row, QList<bool> pixelrow)
{
    for(int i = 0; i < pixelrow.length(); i += 2) {
        pixelrow[i] = !pixelrow[i];
    }
    emit receivePixelRow(row, pixelrow, -1.0f, true);
    cout << "Processed row " << row << endl;
}

void Backend::configCRCSlot(QList<bool> poly)
{
    cout << endl << "Received config CRC poly:" << endl;
    for(int i = 0; i < poly.length(); i++) {
        cout << poly[i] << " ";
    }
    cout << endl << PROMPT << flush;
}

void Backend::configECCSlot(int factor)
{
    cout << endl << "Received config ECC factor " << factor << endl
         << PROMPT << flush;
}

void Backend::configMappingSlot(int map)
{
    cout << endl << "Received config Mapping code " << map << endl
         << PROMPT << flush;
}

void Backend::configSNRSlot(double snr)
{
    cout << endl << "Received config SNR value " << snr << endl
         << PROMPT << flush;
}

void Backend::frontendStatusSlot(bool present)
{
    frontend_present = present;
    if(present) {
        cout << endl << "Frontend is now present." << endl;
    } else {
        cout << endl << "Frontend shutting down." << endl;
    }
    cout << PROMPT << flush;
}
