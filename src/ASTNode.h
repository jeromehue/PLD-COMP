/*************************************************************************
                           ASTNode  -  description
                             -------------------
    début                : 2021-02-27
    copyright            : (C) 2021 par H4244 
*************************************************************************/

#pragma once

#include <iostream>
#include "IR.h"

using namespace std;

// Les différents types de noeuds
enum nodeOp{
    OP_ADD,
    OP_ASSIGN,
    OP_IDENT,
    OP_CONST,
    OP_RETURN
};

class Node {
   
public:
    
    Node(int op, Node* left, Node* right, int arg0, int arg1) {
        this->op        = op;
        this->left      = left;
        this->right     = right;
        this->args[0]   = arg0;
        this->args[1]   = arg1;
    }

    
    void display() {
        switch(op) {
            case OP_ADD:
                std::cout << "OP_ADD" << std::endl;
                break;
            case OP_CONST:
                std::cout << "OP_CONST" << std::endl;
                break;
            case OP_IDENT:
                std::cout << "OP_IDENT" << std::endl;
                break;
            case OP_ASSIGN:
                std::cout << "OP_ASSIGN" << std::endl;
                break;
            case OP_RETURN:
                std::cout << "OP_RETURN" << std::endl;
                break;
            default:
                std::cout << "Unknown Node" << std::endl;
                break;
        }
    }
   
    // The code generation is a recursive walk of the AST of
    // of the function body.
    // Each node of the AST has a methode buildIR(CFG* cfg) 
    std::string  buildIR(CFG* cfg){
        switch(op) {
            case OP_CONST:
                /* Tiré du poly :
                string var = createNewVar();
                cfg->addInstruction(ldconst var const) 
                */

                //std::string var =  create_new_tempvar();
                //cfg->

                break;
            default:
                std::cout << "Erreur lors de la génération de l'IR" 
                << std::endl;
                break;
        }
        return 0;
    }

    // Operator
    int op;

    // Left and right childs
    Node *left, *right;

    // 0, 1 or 2 arguments
    int args[1]; 


};
