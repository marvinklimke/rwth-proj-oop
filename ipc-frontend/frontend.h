/**
 * @file frontend.h Declaration of the Frontend class.
 */

#pragma once

#include <QObject>
#include <QSocketNotifier>
#include <QTextStream>

#include <iostream>

#include "frontend_adaptor.h"
#include "frontend_interface.h"
#include "myimage.h"

/**
 * @brief The Frontend class provides the core functionality of the application.
 * It is embedded in the Qt event loop and waits for user input to be processed and incoming D-Bus packets.
 */
class Frontend : public QObject
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "de.rwth.iss.oop")

public:

    /**
     * @brief Frontend The constructor initializes the members and connects signals and slots.
     * @param parent The parent QObject (i.e. the QCoreApplication instance).
     */
    explicit Frontend(QObject *parent = 0);

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
     * @brief PROMT This literal is the command prompt for user input.
     */
    const std::string PROMPT = "frontend >> ";

    /**
     * @brief backend_present This flag indicates whether the backend is currently running and reachable via D-Bus.
     */
    bool backend_present = false;

    /**
     * @brief sequential This variable indicates whether to application uses a sequential approach to sending messages.
     * sequential = 0     : All pixelrows get send in one for-loop.
     * sequential = x > 0 : The next pixelrow gets send when the predecessor (grade x) arrives.
     */
    int sequential = 0;

    /**
     * @brief original This is the original image as read from filesystem.
     */
    MyImage original;

    /**
     * @brief processed This is the processed image as is leaves the backend.
     */
    MyImage processed;

    /**
     * @brief iface The D-Bus interface instance.
     * Used to create D-Bus Messages for ipc.
     */
    de::rwth::iss::oop *iface;

    /**
     * @brief loadImage Helper function to load an image from filesystem.
     * @param args The command entered by the user.
     */
    void loadImage(const QStringList& args);

    /**
     * @brief saveImage Helper function to save an image to the filesystem.
     * @param args The command entered by the user.
     */
    void saveImage(const QStringList& args) const;

    /**
     * @brief startTransmission This function sends all pixel rows of the currently loaded image to the backend.
     */
    void startTransmission();

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
     * @brief receivePixelRowSlot This slot handles incoming pixelrows being processed by the backend.
     * @param pixelrow The pixelrow being transmitted.
     * @param row The index of the pixelrow.
     */
    void receivePixelRowSlot(int row, QList<bool> pixelrow, double ber, bool crc);

    /**
     * @brief backendStatusSlot This slot handles the status messages of the backend.
     * @param present true if availible, false otherwise.
     */
    void backendStatusSlot(bool present);

};
