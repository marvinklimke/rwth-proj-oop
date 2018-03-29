/**
 * @file ecc.cpp
 * @brief Definition of the Ecc class' functions.
 */

#include "ecc.h"

Ecc::Ecc(Backend *backendptr) :
    backendptr(backendptr)
{
    input.clear();
    output.clear();
    rep = 1;
}

void Ecc::addEcc(const QList<bool> &in)
{
    input.clear();
    output.clear();
    input.append(in);

    for(int i = 0; i < input.size(); i++) {
        for(int k = 0; k < rep; k++) {
            output.append(input[i]);
        }
    }
}

void Ecc::remEcc(const QList<bool> &in)
{
    input.clear();
    input.append(in);
    output.clear();

    for(int i = 0; i < input.size(); i += rep) {
        int c0 = 0, c1 = 0;
        for(int j = 0; j < rep; j++) {
            if(input[i+j]==0)   c0++;
            else                c1++;
        }
        if(c1 > c0) output.append(1);
        else        output.append(0);
    }
}

QList<bool> Ecc::doWork(const QList<bool> &in)
{
    addEcc(in);
    remEcc(modptr->doWork(this->getOutput()));
    return output;
}
