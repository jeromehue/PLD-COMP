#include <string>
#include <iostream>

#ifndef VERBOSE_H
#define VERBOSE_H

using namespace std;

extern bool Verbose;

static int verbose(string chaine)
{
        if (!Verbose) return 1;
        cout << chaine << endl;
        return 0;
}

static void setVerbose(bool setting)
{
        Verbose = setting;
}

#endif
