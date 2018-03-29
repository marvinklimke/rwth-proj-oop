/**
 * @file backend.cpp
 * @brief Definition of the Backend class' functions.
 */

#include <iostream>

#include "backend.h"

using namespace std;

Backend::Backend(QObject *parent) :
    QObject(parent),
    theCrc(new Crc(this)),
    theEcc(new Ecc(this)),
    theMod(new Mod(this)),
    theChannel(new Channel(this))
{
    theCrc->link(theEcc);
    theEcc->link(theMod);
    theMod->link(theChannel);

    if(!initDBus()) {
        cerr << "Initialization of D-Bus failed!" << endl;
        return;
    }

    emit backendStatus(true);
    cout << "Backend ready." << endl;
}

Backend::~Backend()
{
    delete theCrc;
    delete theEcc;
    delete theMod;
    delete theChannel;
    delete iface;
}

bool Backend::initDBus()
{
    new OopAdaptor(this);
    if(!QDBusConnection::sessionBus().registerObject(OOP_DBUS_PATH_BACKEND, this)) {
        return false;
    }
    iface = new de::rwth::iss::oop("", OOP_DBUS_PATH_FRONTEND, QDBusConnection::sessionBus(), this);

    connect(iface, SIGNAL(sendPixelRow(int,QList<bool>)), this, SLOT(sendPixelRowSlot(int,QList<bool>)));
    connect(iface, SIGNAL(configCRC(QList<bool>)), this, SLOT(configCRCSlot(QList<bool>)));
    connect(iface, SIGNAL(configECC(int)), this, SLOT(configECCSlot(int)));
    connect(iface, SIGNAL(configMapping(int)), this, SLOT(configMappingSlot(int)));
    connect(iface, SIGNAL(configSNR(double)), this, SLOT(configSNRSlot(double)));
    connect(iface, SIGNAL(frontendStatus(bool)), this, SLOT(frontendStatusSlot(bool)));

    return true;
}

void Backend::doWork()
{
    output.clear();
    output.append(theCrc->doWork(input));
    calcBer();
    //print(); //put back in if you want to see output every time
}

void Backend::print()
{
    cout << "INPUT: " << endl;
    for(int i = 0; i < input.size(); i++) {
        cout << input[i];
    }
    cout << endl;
    cout << "RECEIVED OUTPUT: " << endl;
    for(int i = 0; i < output.size(); i++) {
        cout << output[i];
    }
    cout << endl;
    cout << "CRC_FLAG: " << crc_flag << endl;
    cout << "BER: " << ber << endl;
}

void Backend::calcBer()
{
    if(output.empty()) {
        cout << "ERROR: run bits through backend first to receive an output" << endl;
        return;
    }
    double counter = 0;
    for(int i = 0; i < input.size(); i++) {
        if(input[i] != output[i]) counter++;
    }
    ber = counter / input.size();
}

void Backend::sendPixelRowSlot(int row, QList<bool> pixelrow)
{
    input = pixelrow;
    doWork();
    crc_flag = theCrc->getCrc_flag();

    emit receivePixelRow(row, output, ber, crc_flag);
    cout << "Processed row " << row << ", ber =  " << ber << ", crc_flag = " << crc_flag << endl;
}

void Backend::configCRCSlot(QList<bool> poly)
{
    this->crcpoly = poly;
    theCrc->setPoly(poly);
}

void Backend::configECCSlot(int rep)
{
    if(rep % 2 == 0) {
        rep += 1;
        cout << "Repition factor was even, added one so that REP odd:" << rep << endl;
    }
    this->rep = rep;
    theEcc->setRep(rep);
}

void Backend::configMappingSlot(int mod)
{
    this->mod = (bool)mod;
    theMod->setMod((bool)mod);
}

void Backend::configSNRSlot(double snr)
{
    this->snr = snr;
    theChannel->setSnr(this->snr);
}

void Backend::frontendStatusSlot(bool present)
{
    if(present) {
        cout << "Frontend is up. Everything ready." << endl;
        emit backendStatus(true);
    } else {
        cout << "Frontend shutting down. Closing application." << endl;
        emit quit();
    }
}
