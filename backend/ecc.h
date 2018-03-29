/**
 * @file ecc.h
 * @brief Declaration of the Ecc class.
 */

#pragma once

#include <QList>

#include "mod.h"

class Backend;

/**
* @brief The Ecc class adds/removes error correction code to/from the bitstream
*/
class Ecc
{

public:

    /**
    * @brief The constructor sets default values.
    * @param backendptr Pointer to the parental Backend instance.
    */
    Ecc(Backend *backendptr);

    /// Getter for the output pixelrow.
    QList<bool> getOutput() const { return output; }

    /// Getter for the ECC repetition factor.
    bool getRep() const { return rep; }

    /// Setter for ECC repetition factor.
    void setRep(int rep) { this->rep = rep; }

    /**
     * @brief Links to the Mod module.
     * @param modptr Pointer to the Mod instance.
     */
    void link(Mod *modptr) { this->modptr = modptr; }

    /**
     * @brief Applies the repetition factor and stores the result internally.
     * @param in The incoming pixelrow.
     */
    void addEcc(const QList<bool> &in);

    /**
     * @brief Reverts the repetition factor.
     * @param in The pixelrow to be reverted.
     */
    void remEcc(const QList<bool> &in);

    /**
    * @brief doWork Runs addEcc and remEcc.
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
     * @brief Pointer to the Mod instance.
     */
    Mod *modptr;

    /**
     * @brief The incoming pixelrow.
     */
    QList<bool> input;

    /**
     * @brief The outgoing pixelrow.
     */
    QList<bool> output;

    /**
     * @brief The current repetition factor.
     */
    int rep;

};
