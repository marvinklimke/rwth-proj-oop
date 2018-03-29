/**
 * @file crc.h
 * @brief Declaration of the Crc class.
 */

#pragma once

#include <QList>

#include "ecc.h"

class Backend;

/**
* @brief The Crc class calculates the crc checksum, adds it to the bitstream and checks if the bitstream is correct.
*/
class Crc
{

public:

    /**
    * @brief The constructor sets default values.
    */

    /**
     * @brief The constructor sets default values.
     * @param backendptr Pointer to the parental Backend obejct.
     */
    Crc(Backend* backendptr);

    /// Getter for the CRC flag.
    bool getCrc_flag() const { return crc_flag; }

    /// Getter for the output pixelrow.
    QList<bool> getOutput() const { return output; }

    /**
    * @brief Calculates checksum and adds it to the bitstream which is then stored in the output.
    * @param in The incoming pixelrow.
    */
    void addCrc(const QList<bool> &in);

    /**
    * @brief Checks if the incoming bitstream is correct and alters the crc_flag.
    * @param in The pixelrow with to be crc checked.
    */
    void check(const QList<bool> &in);

    /**
     * @brief Links to the Ecc module.
     * @param eccptr Pointer to the Ecc instance.
     */
    void link(Ecc *eccptr) { this->eccptr=eccptr; }

    /// Setter for the CRC polynomal.
    void setPoly(QList<bool> poly) { this->poly = poly; }

    /**
     * @brief doWork Runs addCrc, sends bitstream to ecc and checks the returning bits.
     * @param in The incoming pixelrow.
     * @return The outgoing pixelrow.
     */
    QList<bool> doWork(const QList<bool> &in);

private:

    /**
     * @brief Pointer to the parental Backend instance.
     */
    Backend *backendptr;

    /**
     * @brief Pointer to the linked Ecc instance.
     */
    Ecc *eccptr;

    /**
     * @brief The incoming pixelrow.
     */
    QList<bool> input;

    /**
     * @brief The outgoing pixelrow.
     */
    QList<bool> output;

    /**
     * @brief The CRC polynomal.
     */
    QList<bool> poly;

    /**
     * @brief The current CRC flag.
     */
    bool crc_flag;

};
