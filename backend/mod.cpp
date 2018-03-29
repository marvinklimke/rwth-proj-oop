/**
 * @file mod.cpp
 * @brief Definition of the Mod class' functions.
 */

#include "mod.h"

using namespace std;

Mod::Mod(Backend *backendptr) :
    backendptr(backendptr)
{
    mod = 0;
    odd = false;
    input.clear();
    output.clear();
    cinput.clear();
    coutput.clear();
}

void Mod::modulate(const QList<bool> &in)
{
    input.clear();
    coutput.clear();
    input.append(in);

    if(input.size() % 2 != 0) {
        odd=true;
        input.push_back(0);
    }

    if(mod == false) { // Binary
        for(int i = 0; i < input.size(); i += 2) {
            if(input[i] == 0 && input[i+1] == 0)
                coutput.append(symbol(0));
            if(input[i] == 0 && input[i+1] == 1)
                coutput.append(symbol(1));
            if(input[i] == 1 && input[i+1] == 0)
                coutput.append(symbol(2));
            if(input[i] == 1 && input[i+1] == 1)
                coutput.append(symbol(3));
        }
    } else { // Gray
        for(int i = 0; i < input.size(); i += 2) {
            if(input[i] == 1 && input[i+1] == 1)
                coutput.append(symbol(0));
            if(input[i] == 0 && input[i+1] == 1)
                coutput.append(symbol(1));
            if(input[i] == 0 && input[i+1] == 0)
                coutput.append(symbol(2));
            if(input[i] == 1 && input[i+1] == 0)
                coutput.append(symbol(3));
        }
    }
}

void Mod::demod(const QList<std::complex<double>> &in)
{
    cinput.clear();
    output.clear();
    cinput.append(in);

    for(int i = 0; i < cinput.size(); i += 1) {
        int sgn_re, sgn_im;
        if(real(cinput[i])<0)   sgn_re = -1;
        else                    sgn_re = 1;
        if(imag(cinput[i])<0)   sgn_im = -1;
        else                    sgn_im = 1;

        if(mod == false) { // Binary
            if(sgn_re == 1 && sgn_im == 1)
                output.append({0,0});
            else if(sgn_re == -1 && sgn_im == 1)
                output.append({0,1});
            else if(sgn_re == -1 && sgn_im == -1)
                output.append({1,0});
            else if(sgn_re == 1 && sgn_im == -1)
                output.append({1,1});
        } else { // Gray
            if(sgn_re == 1 && sgn_im == 1)
                output.append({1,1});
            else if(sgn_re == -1 && sgn_im == 1)
                output.append({0,1});
            else if(sgn_re == -1 && sgn_im == -1)
                output.append({0,0});
            else if(sgn_re == 1 && sgn_im == -1)
                output.append({1,0});
        }
    }

    if(odd == true) {
        odd = false;
        output.pop_back();
    }
}

QList<bool> Mod::doWork(const QList<bool> &in)
{
    modulate(in);
    demod(channelptr->doWork(this->getCoutput()));
    return output;
}

inline complex<double> Mod::symbol(const int s) const
{
    switch(s) {
    case 0: return complex<double>(W2INV, W2INV);
    case 1: return complex<double>(-W2INV, W2INV);
    case 2: return complex<double>(-W2INV, -W2INV);
    case 3: return complex<double>(W2INV, -W2INV);
    default: return complex<double>(0,0);
    }
}
