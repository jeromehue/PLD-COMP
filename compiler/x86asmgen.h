#include <iostream>
#include "ASTNode.h"

#pragma once

static void asmprint(ASTNode* n) {
    std::cout << "bonjour" << std::endl;
    ASTNode* current  = n;
    while(current->hasNext()) {
        std::cout << "hasnext" << std::endl;
        current = current.getNext();
    }
}
