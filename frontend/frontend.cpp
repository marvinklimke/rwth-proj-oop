/**
 * @file frontend.cpp
 * @brief Definition of the Frontend class' functions.
 */

#include <iostream>

#include "frontend.h"

using namespace std;

Frontend::Frontend(QObject *parent) :
    QObject(parent),
    original(),
    processed()
{
    new OopAdaptor(this);
    QDBusConnection::sessionBus().registerObject(OOP_DBUS_PATH_FRONTEND, this);
    iface = new de::rwth::iss::oop("", OOP_DBUS_PATH_BACKEND, QDBusConnection::sessionBus(), this);

    connect(iface, SIGNAL(receivePixelRow(int,QList<bool>,double,bool)), this, SLOT(receivePixelRowSlot(int,QList<bool>,double,bool)));
    connect(iface, SIGNAL(backendStatus(bool)), this, SLOT(backendStatusSlot(bool)));
}

void Frontend::run()
{
    emit frontendStatus(true);
}

void Frontend::startTransmission()
{
    processed = MyImage(original.width(), original.height(), original.format());
    processed.fill(QColor(0, 0, 0, 0));

    crcerrors = 0;
    biterrorsmean = 0;
    counter = 0;

    emit configCRC(poly);
    emit configECC(ecclength);
    emit configMapping(mapping);
    emit configSNR(snr);

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

    counter++;
    biterrorsmean = (((double)counter - 1) / (double)counter) * biterrorsmean + (ber / (double)counter);

    if(!crc) {
        crcerrors++;
    }

    emit updateGUI(processed, crcerrors, biterrorsmean);

    cout << "Received row " << row << " with ber = " << ber << " and crc flag = " << boolalpha << crc << endl;
}

void Frontend::backendStatusSlot(bool present)
{
    backend_present = present;
    emit updateStatus(backend_present);
}

bool Frontend::loadOriginal(QString file)
{
    return original.load(file);
}

double Frontend::getBerMean() const
{
    return biterrorsmean;
}

int Frontend::getCrcErrors() const
{
    return crcerrors;
}

int Frontend::getEcc() const
{
    return ecclength;
}

int Frontend::getMapping() const
{
    return mapping;
}

double Frontend::getSnr() const
{
    return snr;
}

QList<bool> Frontend::getPoly() const
{
    return poly;
}

void Frontend::setEccLength(int i)
{
    ecclength=i;
}

void Frontend::setSnr(double d)
{
    snr=d;
}

void Frontend::setMapping(int i)
{
    mapping=i;
}

void Frontend::setSequential(int seq)
{
    sequential = seq;
}

void Frontend::setPolyAppend(bool bcrc)
{
    poly.push_back(bcrc);
}

void Frontend::setPoly(const QList<bool> &poly)
{
    this->poly = poly;
}
