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
    int rightadr;


    // Opérande gauche
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

    // Opérande droite
    if ( IsBinOp(node->getRight())   ) {
        // Le noeud de gauche est un opérateur
        // on effectue le calcul
        std::cout << "test" << std::endl;
        rightadr = genBinOp(dynamic_cast<BinOp_n*> (node->getRight()));
    } else if (IsConst(node->getRight()) ) {
        // Le noeud de gauche est une constante
        node->getLeft()->display();
        std::cout << "test2" << std::endl;
        rightadr = genConst(dynamic_cast<Const_n * > (node->getRight()));
        //genBinOp(dynamic_cast<BinOp_n * > (node->getLeft()));
    } else if (IsIdent(node->getRight()) ) {
        // Le noeud de gauche est une variable
        
        // Ne marche probablement pas
        std::cout << "test3" << std::endl;
        rightadr = genBinOp(dynamic_cast<BinOp_n*> (node->getRight()));
    }
       


        switch(node->getOp()){
            case '+':
                output << "\taddl\t" << leftadr << "(%rbp)," 
            << rightadr <<"(%rbp)\n" ;
               return rightadr; 
                break;
            case '-':
                break;
            case '*':
                break;
            default:
                std::cout << "Error : Unknow operator" << std::endl;
                exit(EXIT_FAILURE);
                break;
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
            //temp = temp->getNext();
            
            if( dynamic_cast<Assign_n *>(temp)) {
                
                std::cout << "debug" << std::endl;
                temp->display();
                Ident_n* i = (dynamic_cast<Assign_n * >(temp))->getVar();
                std::string name = i->getName();
                Expr_n* e  = dynamic_cast<Expr_n * >(temp->getExpr());
                if(dynamic_cast<Const_n*> (e)) {
                    std::cout << "assign a const" << std::endl;
                    Const_n* c = dynamic_cast<Const_n * >(e);
                    int value = c->getValue();
                    std::cout << "const : " << value << std::endl;
                    output << "\tmovl\t$" << value << "," 
                        << st->getAddress(name) << "%(rbp)\n";
                }

            
            } else  if( dynamic_cast<BinOp_n*>(temp)) {
                std::cout << "We have expr node" << std::endl;
                // Generate assembly from it
                genBinOp(dynamic_cast<BinOp_n*> (temp));                
            } else if ( dynamic_cast<Assign_n*>(temp)) {

                std::cout << "We have assign node" << std::endl;
                temp->display();
                Expr_n* a(temp->getExpr()); 
                a->display();
                if (dynamic_cast<BinOp_n* > (a)){
                    std::cout << "bonjour"<< std::endl;
                    BinOp_n* b = dynamic_cast<BinOp_n*> (a);
                    b->display();
                    // Generate assembly
                    int adr = genBinOp(b);
                    int var_adr = st->getAddress("b");
                    output << "\tmovl\t"<< adr << "%(rbp),"
                        << var_adr << "%(rbp)\n" ;
                }
            } else if (dynamic_cast<Return_n* >(temp)) {
                std::cout << "Instruction de retour " << std::endl;
                if(dynamic_cast<Const_n*> (temp->getFirst())) 
                {   std::cout << "return a const "  << std::endl;
                    Const_n* n  = dynamic_cast<Const_n*>(temp->getFirst());
                    int retconst = n->getValue();
                    output << "\tmovl\t$"  << retconst << ",%eax\n"; 
                }
            }
            temp = temp->getNext();
        } while(temp->hasNext());
 
    } while (current->hasFirst());
    epilogue();
   output.close();
}

