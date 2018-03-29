/**
 * @file channel.h
 * @brief Declaration of the Channel class.
 */

#pragma once

#include <complex>
#include <random>

#include <QList>

class Backend;

/**
* @brief The Channel class simulates the noise and adds it to the complex symbols.
*/
class Channel
{

public:

    /**
     * @brief The constructor initializes the random number generator with a time-based seed.
     * @param backendptr Pointer to the parental Backend instance.
     */
    Channel(Backend *backendptr);

    /**
     * @brief This method adds noise to a stream of complex symbols.
     * @param in The incoming stream of complex symbols.
     * @return The outgoing stream of complex symbols.
     */
    QList<std::complex<double>> doWork(const QList<std::complex<double>> &in);

    /// Getter for complex output value.
    QList<std::complex<double>> getCoutput() const { return coutput; }

    /// Setter for signal-to-noise-ratio.
    void setSnr(double snr) { this->snr = snr; }

    /**
     * @brief This method adds noise to a stream of complex symbols and saves it in the internal variable.
     * @param in The incoming stream of complex symbols.
     */
    void add_noise(const QList<std::complex<double>> &in);

    /**
     * @brief This method prints the given stream of complex symbols to cout.
     * @param in The stream of complex symbols to be printed.
     */
    void print(const QList<std::complex<double>> &in) const;

private:

    /**
     * @brief Pointer to parental Backend instance.
     */
    Backend *backendptr;

    /**
     * @brief The current signal-to-noise-ratio.
     */
    double snr;

    /**
     * @brief The random number generator.
     */
    std::default_random_engine generator;

    /**
     * @brief The incoming stream of complex symbols.
     */
    QList<std::complex<double>> cinput;

    /**
     * @brief The outgoing stream of complex symbols.
     */
    QList<std::complex<double>> coutput;

};
