/**
 * @file channel.cpp
 * @brief Definition of the Channel class' functions.
 */

#include <chrono>
#include <iostream>

#include "channel.h"

using namespace std;

Channel::Channel(Backend *backendptr) :
    backendptr(backendptr)
{
    cinput.clear();
    coutput.clear();
    generator.seed(time(NULL));
}

void Channel::add_noise(const QList<std::complex<double> > &in)
{
    cinput.clear();
    coutput.clear();
    cinput.append(in);

    normal_distribution<double> distribution(0.0, 1.0/snr);

    double re = 0.0, im = 0.0;
    for(int i = 0; i < cinput.size(); i++) {
        re = real(cinput[i])+distribution(generator);
        im = imag(cinput[i])+distribution(generator);
        coutput.append(complex<double>(re,im));
    }
}

void Channel::print(const QList<std::complex<double> > &in) const
{
    for(int i = 0; i < in.size(); i++) {
        cout << in[i];
    }
    cout << endl;
}

QList<std::complex<double>> Channel::doWork(const QList<std::complex<double>> &in)
{
    add_noise(in);
    return coutput;
}
