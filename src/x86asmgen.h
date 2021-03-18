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


static void asmgen(ASTNode * n) {
    ASTNode* current  = n;
    do  {
        current = current->getFirst();
        ASTNode *temp = current;
        do {
            temp = temp->getNext();
            if( dynamic_cast<BinOp_n*>(temp)) {
                std::cout << "We have expr node" << std::endl;
            }
        } while(temp->hasNext());
 
    } while (current->hasFirst());
}
