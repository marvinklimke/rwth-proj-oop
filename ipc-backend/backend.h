/**
 * @file backend.h Declaration of the Backend class.
 */

#pragma once

#include <QObject>
#include <QSocketNotifier>
#include <QTextStream>

#include <iostream>

#include "backend_adaptor.h"
#include "backend_interface.h"

/**
 * @brief The Backend class provides the core functionality of the application.
 * It is embedded in the Qt event loop and waits for incoming D-Bus packets and user-input.
 */
class Backend : public QObject
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "de.rwth.iss.oop")

public:

    /**
     * @brief Backend The constructor initializes the members and connects signals and slots.
     * @param parent The parent QObject (i.e. the QCoreApplication instance).
     */
    explicit Backend(QObject *parent = 0);

    /**
     * @brief run This function initializes the D-Bus connection and prompts the user for commands.
     */
    void run();

private:

    /**
     * @brief notifier This SocketNotifier is used to listen on stdin and call a slot, when the user inputs some data.
     */
    QSocketNotifier notifier;

    /**
     * @brief qin This TextStream is used to read from stdin.
     */
    QTextStream qin;

    /**
     * @brief PROMPT This literal is the command prompt for user input.
     */
    const std::string PROMPT = "backend >> ";

    /**
     * @brief frontend_present This flag indicates whether the frontend is currently running and reachable via D-Bus.
     */
    bool frontend_present = false;

    /**
     * @brief iface The D-Bus interface instance.
     * Used to create D-Bus Messages for ipc.
     */
    de::rwth::iss::oop *iface;

signals:

    /**
     * @brief quit This signal is triggered when the user wants to close the application.
     */
    void quit();

    /**
     * @brief sendPixelRow This signal gets triggered when a pixel-row is transmitted to the backend.
     * @param pixelrow The pixelrow of the image being transmitted.
     * @param row The index of the transmitted row.
     */
    void sendPixelRow(int row, QList<bool> pixelrow);

    /**
     * @brief sendPixelRow This signal gets triggered when a pixel-row is transmitted to the frontend.
     * @param pixelrow The pixelrow of the image being transmitted.
     * @param row The index of the transmitted row.
     */
    void receivePixelRow(int row, QList<bool> pixelrow, double ber, bool crc);

    /**
     * @brief configCRC This signal gets triggered to set the crc polynomal.
     * @param poly The coefficients of the crc polynomal.
     */
    void configCRC(QList<bool> poly);

    /**
     * @brief configECC This signal gets triggered to set the repetition rate of the ecc.
     * @param factor The number of times a bits being repeated.
     */
    void configECC(int factor);

    /**
     * @brief configMapping This signal gets triggered to set the type of mapping on the channel.
     * @param map The desired mapping.
     */
    void configMapping(int map);

    /**
     * @brief configSNR This signal gets triggered to set the signal-to-noise-ratio on the channel.
     * @param snr The desired snr.
     */
    void configSNR(double snr);

    /**
     * @brief frontendStatus This signal gets triggered when the frontend ist starting up or shutting down.
     * @param present true if availible, false otherwise.
     */
    void frontendStatus(bool present);

    /**
     * @brief backendStatus This signal gets triggered when the backend ist starting up or shutting down.
     * @param present true if availible, false otherwise.
     */
    void backendStatus(bool present);

private slots:

    /**
     * @brief readCommand This slot gets called when the user entered a line of data in the command line.
     */
    void readCommand();

public slots:

    /**
     * @brief sendPixelRowSlot This slots handles incoming pixelrows.
     * @param pixelrow The incoming pixelrow to be processed.
     * @param row The index of the pixelrow being transmitted.
     */
    void sendPixelRowSlot(int row, QList<bool> pixelrow);

    /**
     * @brief configCRCSlot This slot handles the configuration of the crc polynomal.
     * @param poly The crc polynomal.
     */
    void configCRCSlot(QList<bool> poly);

    /**
     * @brief configECCSlot This slot handles the configuration of the ecc repetition factor.
     * @param factor The number of times a bits being repeated.
     */
    void configECCSlot(int factor);

    /**
     * @brief configMappingSlot This slot handles the configuration of the mapping on the channel.
     * @param map The desired mapping.
     */
    void configMappingSlot(int map);

    /**
     * @brief configSNRSlot This slot handles the configuration of the signal-to-noise-ratio on the channel.
     * @param snr The desired snr.
     */
    void configSNRSlot(double snr);

    /**
     * @brief frontendStatusSlot This slot handles the status messages of the frontend.
     * @param present true if availible, false otherwise.
     */
    void frontendStatusSlot(bool present);

};
