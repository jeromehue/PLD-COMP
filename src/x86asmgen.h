#include <iostream>
#include "ASTNode.h"
#include "symboltable.h"

#pragma once

static Symboltable symbtab;

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

static bool IsConst(ASTNode* n) {
    return (dynamic_cast<Const_n* > (n));
}

static void genBinOp(BinOp_n * node) {
    // Generating ASM for binary operators :
    // +, -, *
    if ( IsConst(node->getLeft()) && IsConst(node->getRight() )) {
        std::cout << "both operands are constant" << std::endl;  
        // Create two temps variables and store operands in it
        std::string temp1 = "!temp1";
        std::string temp2 = "!temp2";

    
        /*
                
        */
    }


}

static void asmgen(ASTNode * n) {
    ASTNode* current  = n;
    if (dynamic_cast<Prog * > (current)) {
        std::cout << "print symbol" << std::endl;
        n->getST()->printSymbols();
    }
    do  {
        current = current->getFirst();
        ASTNode *temp = current;
        do {
            temp = temp->getNext();
            if( dynamic_cast<BinOp_n*>(temp)) {
                std::cout << "We have expr node" << std::endl;
                // Generate assembly from it
                genBinOp(dynamic_cast<BinOp_n*> (temp));
                
            }
        } while(temp->hasNext());
 
    } while (current->hasFirst());
}


