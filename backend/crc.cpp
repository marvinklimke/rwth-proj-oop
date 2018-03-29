/**
 * @file crc.cpp
 * @brief Definition of the Crc class' functions.
 */

#include "crc.h"

Crc::Crc(Backend *backendptr):
    backendptr(backendptr)
{
    input.clear();
    output.clear();
    poly.clear();
    crc_flag = false;
}

void Crc::addCrc(const QList<bool> &in)
{
    input.clear();
    input.append(in);
    output.clear();
    output.append(input);

    for (int i = 0; i < poly.size() - 1; i++) {
        input.push_back(0);
    }
    while(input.size() > poly.size() - 1) {
        if(input[0] != 0) {
            for (int i = 0; i < poly.size(); i++) {
                input[i] = input[i] xor poly[i];
            }
        }

        input.pop_front();
    }

    output.append(input); // adding crc_checksum to bitstream
}

void Crc::check(const QList<bool> &in)
{
    input.clear();
    output.clear();
    input.append(in);
    output.append(in);

    for (int i = 0; i < poly.size() - 1 ; i++) {
        output.pop_back();
    }

    while(input.size() > poly.size() - 1) {
        if(input[0] != 0) {
            for (int i = 0; i < poly.size(); i++) {
                input[i] = input[i] xor poly[i];
            }
            input.pop_front();
        } else {
            input.pop_front();
        }
    }

    for (int i = 0; i < input.size(); i++) {
        if(input[i] != 0) {
            crc_flag = false;
            return;
        }
    }

    crc_flag = true;
    return;
}


QList<bool> Crc::doWork(const QList<bool> &in)
{
    addCrc(in);
    check(eccptr->doWork(this->getOutput()));
    return output;
}
