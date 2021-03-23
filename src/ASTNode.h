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
    OP_CONST
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
        }
    }
   
    // The code generation is a recursive walk of the AST of
    // of the function body.
    // Each node of the AST has a methode buildIR(CFG* cfg) 
    void buildIR(CFG* cfg);

    // Operator
    int op;

    // Left and right childs
    Node *left, *right;

    // 0, 1 or 2 arguments
    int args[1]; 


};
