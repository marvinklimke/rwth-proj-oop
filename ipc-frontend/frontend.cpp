/**
 * @file frontend.cpp Definition of the Frontend class' functions.
 */

#include "frontend.h"

#include <unistd.h>

using namespace std;

Frontend::Frontend(QObject *parent) :
    QObject(parent),
    notifier(STDIN_FILENO, QSocketNotifier::Read),
    qin(stdin),
    original(),
    processed()
{
    connect(&notifier, SIGNAL(activated(int)), this, SLOT(readCommand()));

    new OopAdaptor(this);
    QDBusConnection::sessionBus().registerObject("/de/rwth/iss/oop/frontend", this);
    iface = new de::rwth::iss::oop("", "/de/rwth/iss/oop/backend", QDBusConnection::sessionBus(), this);

    connect(iface, SIGNAL(receivePixelRow(int,QList<bool>,double,bool)), this, SLOT(receivePixelRowSlot(int,QList<bool>,double,bool)));
    connect(iface, SIGNAL(backendStatus(bool)), this, SLOT(backendStatusSlot(bool)));
}

void Frontend::run()
{
    emit frontendStatus(true);
    cout << "Frontend started up." << endl;
    cout << PROMPT << flush;
}

void Frontend::readCommand()
{
    QString line = qin.readLine();
    QStringList args = line.split(" ");
    if(line == "quit") {
        cout << "Frontend shutting down." << endl;
        emit frontendStatus(false);
        emit quit();
        return;
    } else if(args[0] == "load") {
        loadImage(args);
    } else if(args[0] == "save") {
        saveImage(args);
    } else if(line == "start") {
        startTransmission();
    } else if(args[0] == "crc") {
        QList<bool> poly;
        for(int i = 1; i < args.length(); i++)
            poly.append((bool)args[i].toInt());
        emit configCRC(poly);
    } else if(args[0] == "ecc") {
        int rep = args[1].toInt();
        emit configECC(rep);
    } else if(args[0] == "map") {
        int mapp = args[1].toInt();
        emit configMapping(mapp);
    } else if(args[0] == "snr") {
        double snr = args[1].toDouble();
        emit configSNR(snr);
    } else if(args[0] == "seq") {
        sequential = args[1].toInt();
        cout << "sequential set to " << sequential << endl;
    } else if(line == "help") {
        cout << "This is the dummy Frontend of the oop-project." << endl << endl
             << "currently availible commands:" << endl
             << "load <path>    : Load image from file specified (has default value)." << endl
             << "save <path>    : Save image to file specified (has default value)." << endl
             << "start          : Start transmitting pixelrows over D-Bus." << endl
             << "crc <poly>     : Send a crc polynomal to the backend." << endl
             << "ecc <val>      : Send a ecc repetition value to the backend." << endl
             << "map <val>      : Send a map value { 0, 1 } to the backend." << endl
             << "snr <val>      : Send a snr value to the backend." << endl
             << "seq <val>      : Config sequential mode (enable for big pictures)." << endl
             << "help           : Display this page." << endl
             << "quit           : Quit the application." << endl << endl;
    } else if(line != "") {
        cout << "Unknown command: " << line.toStdString() << endl
             << "Type help to list all availible commands." << endl;
    }
    cout << PROMPT << flush;
}

void Frontend::loadImage(const QStringList& args)
{
    QString path = "lena.jpg";
    if(args.length() > 1) {
        path = args[1];
    }
    if(original.load(path)) {
        cout << "Successfully loaded image from file: " << path.toStdString() << endl;
    } else {
        cout << "Error loading image from file: " << path.toStdString() << endl;
    }
    processed = MyImage(original.width(), original.height(), original.format());
}

void Frontend::saveImage(const QStringList& args) const
{
    QString path = "lena2.jpg";
    if(args.length() > 1) {
        path = args[1];
    }
    if(processed.save(path)) {
        cout << "Successfully saved image to file: " << path.toStdString() << endl;
    } else {
        cout << "Error saving image to file: " << path.toStdString() << endl;
    }
}

void Frontend::startTransmission()
{
    if(sequential) {
        if(sequential < original.height()) {
            for(int i = 0; i < sequential; i++) {
                emit sendPixelRow(i, original.getPixelRow(i));
            }
        } else {
            cerr << "Image to small for sequential value " << sequential << "." << endl;
        }
    } else {
        QList<bool> pixelrow;
        for(int r = 0; r < original.height(); r++) {
            pixelrow = original.getPixelRow(r);
            emit sendPixelRow(r, pixelrow);
            cout << "Send row " << r << endl;
        }
    }
}

void Frontend::receivePixelRowSlot(int row, QList<bool> pixelrow, double ber, bool crc)
{
    if(sequential) {
        int nextrow = row + sequential;
        if(nextrow < original.height()) {
            emit sendPixelRow(nextrow, original.getPixelRow(nextrow));
        }
    }

    processed.setPixelRow(row, pixelrow);

    cout << "Received row " << row << " with ber = " << ber << " and crc flag = " << boolalpha << crc << endl;
}

void Frontend::backendStatusSlot(bool present)
{
    backend_present = present;
    if(present) {
        cout << endl << "Backend is now present." << endl;
    } else {
        cout << endl << "Backend shutting down." << endl;
    }
    cout << PROMPT << flush;
}
