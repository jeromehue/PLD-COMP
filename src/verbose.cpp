#include "verbose.h"
#include <iostream>
#include <string>

bool Verbose = false;

void setVerbose(bool setting) {
    Verbose = setting;
}

int verbose(std::string chaine) {
    if (!Verbose) return 1;
    std::cout << chaine << std::endl;
    return 0;
}
