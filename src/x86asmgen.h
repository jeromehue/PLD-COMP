#include <iostream>
#include <fstream>
#include "ASTNode.h"
#include "symboltable.h"

#pragma once

static Symboltable* st;
static std::ofstream output;

void prologue() {
    output <<
        ".global main\n"
        "main:\n"
        "	# Prologue\n"
        "	pushq %rbp\n"
        "	movq %rsp, %rbp\n"
        "\n"
        "	# Body\n";
}

void epilogue() {
    output <<
        "\n"
        "	# Epilogue\n"
        "   popq %rbp\n"
        " 	ret\n";
}


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

bool IsConst(ASTNode* n) {
    return (dynamic_cast<Const_n* > (n));
}

bool IsIdent(ASTNode* n) {
    return (dynamic_cast<Ident_n* > (n));
}



bool IsBinOp(ASTNode* n) {
    return (dynamic_cast<BinOp_n* > (n));
}


static int tempCount;
int getTempCount() {
    return tempCount++;
}


static int genConst(Const_n* node) {
 
        //Debug
        std::cout << "Current node is const" << std::endl;

        //create temp var
        int a = getTempCount();
        std::string tempname = "!tempvar" + std::to_string(a);
        int adrtemp = st->store(tempname, 0);    

        // Fetch const value
        int const1 = 
            (dynamic_cast<Const_n* > (node))->getValue();

        // generate asm
        std::cout   << "\tmovl\t$" << const1 << ","
                    << adrtemp << "(%rbp)\n";

        // return var adress
        return adrtemp;


}

static int genBinOp(BinOp_n * node) {
    // Generating ASM for binary operators nodes

    // Check if node is a primary expression : const or var
    std::cout << "call to Binop" << std::endl;
    node->display();
   

    int leftadr;

    if ( IsBinOp(node->getLeft())   ) {
        // Le noeud de gauche est un opérateur
        // on effectue le calcul
        std::cout << "test" << std::endl;
        leftadr = genBinOp(dynamic_cast<BinOp_n*> (node->getLeft()));
    } else if (IsConst(node->getLeft()) ) {
        // Le noeud de gauche est une constante
        node->getLeft()->display();
        std::cout << "test2" << std::endl;
        leftadr = genConst(dynamic_cast<Const_n * > (node->getLeft()));
        //genBinOp(dynamic_cast<BinOp_n * > (node->getLeft()));
    } else if (IsIdent(node->getLeft()) ) {
        // Le noeud de gauche est une variable
        
        // Ne marche probablement pas
        std::cout << "test3" << std::endl;
        leftadr = genBinOp(dynamic_cast<BinOp_n*> (node->getLeft()));
    }
    if ( IsConst(node->getLeft()) && IsConst(node->getRight() )) {
        std::cout << "both operands are constant" << std::endl;  
        // Create two temps variables and store operands in it
        std::string temp1 = "!temp1";
        std::string temp2 = "!temp2";

        int adrtemp1 = st->store(temp1, 0);    
        int adrtemp2 = st->store(temp2, 0);    
        int const1 = 
            (dynamic_cast<Const_n* > (node->getLeft()))->getValue();
        int const2 = 
            (dynamic_cast<Const_n* > (node->getRight()))->getValue();
       
        std::cout << "\tmovl\t$" << const1 << ","<< adrtemp1 << "(%rbp)\n";
        std::cout << "\tmovl\t$" << const2 << ","<< adrtemp2 << "(%rbp)\n";


        switch(node->getOp()){
            case '+':
                std::cout << "\taddl\t" << adrtemp1 << "(%rbp)," 
            << adrtemp2 <<"(%rbp)\n" ;
               return adrtemp2; 
                break;
            case '-':
                break;
            case '*':
                break;
            default:
                std::cout << "Error : Unknow operator" << std::endl;
                exit(EXIT_FAILURE);
                break;
        }
        /*
                
        */
    }
    std::cout << "Erreur" << std::endl;
    return 0;
}

/**
 * Generate x86 assembly code
 */
static void asmgen(ASTNode * n) {

    tempCount = 0;    
    
    output.open("output.s");
    prologue();
    ASTNode* current  = n;
    if (dynamic_cast<Prog * > (current)) {
        std::cout << "print symbol" << std::endl;
        n->getST()->printSymbols();
        st = n->getST();
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
    epilogue();
   output.close();
}

