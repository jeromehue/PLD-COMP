/*************************************************************************
                           ASTNode  -  description
                             -------------------
    début                : 2021-02-27
    copyright            : (C) 2021 par H4244 
*************************************************************************/

#pragma once

#include <iostream>
#include "IR.h"
#include "symboltable.h"

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
                std::cout << "OP_ASSIGN | " << std::endl;
                std::cout << "\tleft : "; 
                left->display();
                std::cout << "\tright : ";
                right->display();
                std::cout << std::endl;
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

        // Debug print
        std::cout << "Generating IR for : "; 
        this->display(); std::cout << endl; 

        // Avoid jumps that bypasses var init.
        std::string sright, sleft;
        std::string var3;   
        std::vector<std::string> retvector;
        switch(op) {
            
           // case OP_CONST:
                /* Tiré du poly :
                string var = createNewVar();
                cfg->addInstruction(ldconst var const) 
                */

             //   break;
            case OP_CONST:
                var3 = cfg->create_new_tempvar(INT);
                retvector.push_back(var3);
                cfg->current_bb->add_IRInstr(IRInstr::ldconst, INT, retvector
                        );
                return var3;
                break;
            case OP_IDENT:
                std::cout << "Generating IR for var : " <<
                cfg->symbols->getName(args[0])
                << std::endl;
                return cfg->symbols->getName(args[0]);
            case OP_ASSIGN:
                sleft  = left->buildIR(cfg);
                sright = right->buildIR(cfg);
                retvector.push_back(sleft);
                retvector.push_back(sright);
                cfg->current_bb->add_IRInstr(
                    IRInstr::wmem, 
                    INT,  
                    retvector
                );
                return sright;
                break;
            case OP_RETURN:
                break;
            default:
                std::cout 
                << "Erreur lors de la génération de l'IR" 
                << std::endl;
                std::cout 
                << "Fonctionnalité non implémentée" 
                << std::endl;
                exit(EXIT_FAILURE);
                break;
        }
        return "";
    }

    // Operator
    int op;

    // Left and right childs
    Node *left, *right;

    // 0, 1 or 2 arguments
    int args[1]; 


};
