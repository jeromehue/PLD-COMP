#include <iostream>
#include "ASTNode.h"

#pragma once

static void asmprint(ASTNode* n) {
    std::cout << "call to asmprint" << std::endl;
    ASTNode* current  = n;
    current->display();
    while(current->hasFirst()) {
        current = current->getFirst();
        std::cout << "|" << std::endl;
        current->displayLinked();
    }
}
