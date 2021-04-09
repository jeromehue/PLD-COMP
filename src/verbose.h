#include <string>
#include <iostream>

#pragma once

extern  bool Verbose;
 
static int verbose(std::string chaine) 
{
        if (!Verbose) 
                return 1;
        std::cout << chaine << std::endl;
        return 0;
}

static void setVerbose(bool setting) 
{
        Verbose = setting;
}
