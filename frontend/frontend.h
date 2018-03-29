/**
 * @file frontend.h
 * @brief Declaration of the Frontend class.
 */

#pragma once

#include <QObject>

#include "../shared/oop_definitions.h"
#include "../shared/oop_adaptor.h"
#include "../shared/oop_interface.h"

#include "myimage.h"


/**
 * @brief The Frontend class provides the core functionality of the application.
 * It is embedded in the Qt event loop and waits for user input to be processed and incoming D-Bus packets.
 */
class Frontend : public QObject
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", OOP_DBUS_INTERFACE)

public:

    /**
     * @brief The constructor initializes the members and connects signals and slots.
     * @param parent The parent QObject (i.e. the QCoreApplication instance).
     */
    explicit Frontend(QObject *parent = 0);

    /**
     * @brief This function initializes the D-Bus connection and prompts the user for commands.
     */
    void run();

    /**
     * @brief This function sends the pixelrows of the currently loaded image to the backend.
     */
    void startTransmission();

    /**
     * @brief This method load an image from the given path into the MyImage instance.
     * @param file path and filename of the image file (jpg, png, ...).
     * @return true if successful, false otherwise.
     */
    bool loadOriginal(QString file);

    /// Getter for BER mean value.
    double getBerMean() const;

    /// Getter for number of CRC error.
    int getCrcErrors() const;

    /// Getter for Mapping code.
    int getMapping() const;

    /// Getter for ECC repetition factor.
    int getEcc() const;

    /// Getter for SNR value.
    double getSnr() const;

    /// Getter for CRC polynomal.
    QList <bool> getPoly() const;

    /// Setter for CRC polynomal.
    void setPoly(const QList<bool> &poly);

    /// Appends the given value to the CRC polynomal
    void setPolyAppend(bool bcrc);

    /// Setter for ECC repetition factor.
    void setEccLength(int i);

    /// Setter for SNR value.
    void setSnr(double d);

    /// Setter for Mapping code.
    void setMapping(int i);

    /// Setter for sequential value.
    void setSequential(int seq);

private:

    /**
     * @brief This variable indicates the rowcountparameter of the biterrorvaluation
     */
    int counter;

    /**
     * @brief This variable indicates the mean of biterrors in the pixelrows of the transmitted image
     */
    double biterrorsmean;

    /**
     * @brief This variable indicates the numbers of crcerros in the transmitted image
     */
    int crcerrors;

    /**
     * @brief This variable idicates the length of the ecc redundancy
     */
    int ecclength;

    /**
     * @brief This variable indicates the form of mapping (0 = binary, 1 = gray)
     */
    int mapping;

    /**
     * @brief This variable indicates the signal to noise ratio
     */
    double snr;

    /**
     * @brief This list represents the crc-polynom
     */
    QList <bool> poly;

    /**
     * @brief This flag indicates whether the backend is currently running and reachable via D-Bus.
     */
    bool backend_present = false;

    /**
     * @brief This variable indicates whether to application uses a sequential approach to sending messages.
     * sequential = 0     : All pixelrows get send in one for-loop.
     * sequential = x > 0 : The next pixelrow gets send when the predecessor (grade x) arrives.
     */
    int sequential = 5;

    /**
     * @brief This is the original image as read from filesystem.
     */
    MyImage original;

    /**
     * @brief This is the processed image as is leaves the backend.
     */
    MyImage processed;

    /**
     * @brief The D-Bus interface instance.
     * Used to create D-Bus Messages for ipc.
     */
    de::rwth::iss::oop *iface;

signals:

    /**
     * @brief This signal is triggered when the user wants to close the application.
     */
    void quit();

    /**
     * @brief This signal gets triggered when a pixel-row is transmitted to the backend.
     * @param row The index of the transmitted row.
     * @param pixelrow The pixelrow of the image being transmitted.
     */
    void sendPixelRow(int row, QList<bool> pixelrow);

    /**
     * @brief sendPixelRow This signal gets triggered when a pixel-row is transmitted to the frontend.
     * @param row The index of the transmitted row.
     * @param pixelrow The pixelrow of the image being transmitted.
     * @param ber The bit-error-rate of this pixelrow.
     * @param crc The crc flag of this pixelrow.
     */
    void receivePixelRow(int row, QList<bool> pixelrow, double ber, bool crc);

    /**
     * @brief This signal gets triggered to set the crc polynomal.
     * @param poly The coefficients of the crc polynomal.
     */
    void configCRC(QList<bool> poly);

    /**
     * @brief This signal gets triggered to set the repetition rate of the ecc.
     * @param factor The number of times a bits being repeated.
     */
    void configECC(int factor);

    /**
     * @brief This signal gets triggered to set the type of mapping on the channel.
     * @param map The desired mapping.
     */
    void configMapping(int map);

    /**
     * @brief This signal gets triggered to set the signal-to-noise-ratio on the channel.
     * @param snr The desired snr.
     */
    void configSNR(double snr);

    /**
     * @brief This signal gets triggered when the frontend ist starting up or shutting down.
     * @param present true if availible, false otherwise.
     */
    void frontendStatus(bool present);

    /**
     * @brief This signal gets triggered when the backend ist starting up or shutting down.
     * @param present true if availible, false otherwise.
     */
    void backendStatus(bool present);

    /**
     * @brief This signal gets triggered to update the GUI on arrival of a new pixelrow.
     * @param image The updated Image.
     * @param crc_flags The number of crc errors.
     * @param ber The mean of the bit error rate.
     */
    void updateGUI(const MyImage &image, int crc_flags, double ber);

    /**
     * @brief This signal gets triggered when the backend ist starting up or shutting down.
     * @param backend_status true if availible, false otherwise.
     */
    void updateStatus(bool backend_status);

public slots:

    /**
     * @brief This slot handles incoming pixelrows being processed by the backend.
     * @param row The index of the pixelrow.
     * @param pixelrow The pixelrow being transmitted.
     * @param ber The bit-error-rate of this pixelrow.
     * @param crc The crc flag of this pixelrow.
     */
    void receivePixelRowSlot(int row, QList<bool> pixelrow, double ber, bool crc);

    /**
     * @brief This slot handles the status messages of the backend.
     * @param present true if availible, false otherwise.
     */
    void backendStatusSlot(bool present);

};
