/**
 * @file mod.h
 * @brief Declaration of the Mod class.
 */

#pragma once

#include <cmath>
#include <complex>

#include <QList>

#include "channel.h"

class Backend;

/**
* @brief The Mod class converts the bitstream to complex symbols and vice versa.
*/
class Mod
{

public:

    /**
    * @brief Mod The constructor sets default values.
    * @param backendptr Pointer to parental Backend instance.
    */
    Mod(Backend *backendptr);

    /// Getter for output stream of complex symbols.
    QList<std::complex<double>> getCoutput() const { return coutput; }

    /// Getter for output pixelrow.
    QList<bool> getOutput() const { return output; }

    /**
     * @brief This method creates the stream of complex symbols from a pixelrow.
     * @param in The pixelrow to be converted.
     */
    void modulate(const QList<bool> &in);

    /**
     * @brief This method reverts the modulation and creates a pixelrow.
     * @param in The stream of complex symbols to be used.
     */
    void demod(const QList<std::complex<double> > &in);

    /**
     * @brief Links to the Channel instance.
     * @param channelptr Pointer to Channel instance.
     */
    void link(Channel *channelptr) { this->channelptr = channelptr; }

    /// Setter for Mapping code.
    void setMod(bool mod) { this->mod = mod; }

    /**
    * @brief doWork Runs modulate and demod.
    * @param in The incoming pixelrow.
    * @return The outgoing pixelrow.
    */
    QList<bool> doWork(const QList<bool> &in);

private:

    /**
     * @brief Pointer to parental Backend instance.
     */
    Backend *backendptr;

    /**
     * @brief Pointer to Channel instance.
     */
    Channel *channelptr;

    /**
     * @brief Mapping code.
     */
    bool mod;

    /**
     * @brief The incoming pixelrow.
     */
    QList<bool> input;

    /**
     * @brief The outgoing pixelrow.
     */
    QList<bool> output;

    /**
     * @brief The incoming stream of complex symbols.
     */
    QList<std::complex<double>> cinput;

    /**
     * @brief The outgoing stream of complex symbols.
     */
    QList<std::complex<double>> coutput;

    /**
     * @brief Indicates, whether the length of the pixelrow was odd.
     * for qpsk even # of bits needed.
     */
    bool odd;

    /**
     * @brief This method provides the complex symbols.
     * @param s Index of the complex symbol.
     * @return The complex symbol.
     */
    inline std::complex<double> symbol(const int s) const;

    /**
     * @brief Constant 1 over sqrt(2).
     */
    const double W2INV = 1 / sqrt(2);

};
