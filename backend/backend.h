/**
 * @file backend.h
 * @brief Declaration of the Backend class.
 */

#pragma once

#include <QList>
#include <QObject>

#include "../shared/oop_definitions.h"
#include "../shared/oop_adaptor.h"
#include "../shared/oop_interface.h"

#include "crc.h"
#include "ecc.h"
#include "mod.h"
#include "channel.h"

/**
 * @brief The Backend class stores the configuration parameters, input and calculated output.
 */
class Backend : public QObject
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", OOP_DBUS_INTERFACE)

public:

    /**
     * @brief The constructor initializes backend with some default parameters and links modules.
     */
    Backend(QObject *parent = 0);

    /**
     * @brief The destructor deletes the module-instances and the d-bus interface.
     */
    virtual ~Backend();

    /**
     * @brief Starts the backend routine.
     */
    void doWork();

    /**
     * @brief Calculates the bit-error-rate.
     */
    void calcBer();

    /**
     * @brief Prints the input and output pixelrow and crc flag and ber to cout.
     */
    void print();

private:

    /// The CRC instance.
    Crc *theCrc;

    /// The ECC instance.
    Ecc *theEcc;

    /// The Modulator instance.
    Mod *theMod;

    /// The Channel instance.
    Channel *theChannel;

    /// The pixelrow to be processed.
    QList<bool> input;

    /// The received pixelrow.
    QList<bool> output;

    /// The CRC polynomal.
    QList<bool> crcpoly;

    /// The ECC repetition factor.
    int rep = 1;

    /// The Mapping code.
    /// 0 binary 1 gray.
    bool mod = false;

    /// The current CRC flag.
    bool crc_flag;

    /// The signal-to-noise-ratio.
    double snr = 1.0;

    /// The current bit-error-rate.
    double ber;

    /**
     * @brief iface The D-Bus interface instance.
     * Used to create D-Bus Messages for communication with the frontend.
     */
    de::rwth::iss::oop *iface;

    /**
     * @brief initDBus This funtion initializes the D-Bus connection registers the object.
     * @return true if successful, false otherwise.
     */
    bool initDBus();

signals:

    /**
     * @brief quit This signal is triggered when the user wants to close the application.
     */
    void quit();

    /**
     * @brief sendPixelRow This signal gets triggered when a pixel-row is transmitted to the backend.
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
     * @brief configCRC This signal gets triggered to set the crc polynomal.
     * @param poly The coefficients of the crc polynomal.
     */
    void configCRC(QList<bool> poly);

    /**
     * @brief configECC This signal gets triggered to set the repetition rate of the ecc.
     * @param rep The number of times a bits being repeated.
     */
    void configECC(int rep);

    /**
     * @brief configMapping This signal gets triggered to set the type of mapping on the channel.
     * @param mod The desired mapping.
     */
    void configMapping(int mod);

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

public slots:

    /**
     * @brief sendPixelRowSlot This slots handles incoming pixelrows.
     * @param row The index of the pixelrow being transmitted.
     * @param pixelrow The incoming pixelrow to be processed.
     */
    void sendPixelRowSlot(int row, QList<bool> pixelrow);

    /**
     * @brief configCRCSlot This slot handles the configuration of the crc polynomal.
     * @param poly The crc polynomal.
     */
    void configCRCSlot(QList<bool> poly);

    /**
     * @brief configECCSlot This slot handles the configuration of the ecc repetition factor.
     * @param rep The number of times a bits being repeated.
     */
    void configECCSlot(int rep);

    /**
     * @brief configMappingSlot This slot handles the configuration of the mapping on the channel.
     * @param mod The desired mapping.
     */
    void configMappingSlot(int mod);

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
