#include <iostream>
#include <fstream>
#include "ASTNode.h"
#include "symboltable.h"

#pragma once

static Symboltable *st;
static std::ofstream output;

void prologue() {
    output << 
        ".global main\n"
        "main:\n"
        "\t# Prologue\n"
        "\tpushq %rbp\n"
        "\tmovq %rsp, %rbp\n"
        "\n"
        "\t# Body\n";
}

void epilogue() {
    output <<
        "\n"
        "\t# Epilogue\n"
        "\tpopq %rbp\n"
        "\tret\n";
}

static void asmprint(ASTNode *n) {
    std::cout << "call to asmprint" << std::endl;
    ASTNode *current = n;
    current->display();
    std::string indent;
    while (current->hasFirst()) {
        current = current->getFirst();
        std::cout << "|" << std::endl;
        //current->displayLinked();
        indent = indent + "|";
        ASTNode *cur = current;
        std::cout << indent << "-";
        cur->display();
        while (cur->hasNext()) {
            cur = cur->getNext();
            std::cout << indent << "-";
            cur->display();
        }
    }
}

bool isConst(ASTNode *n) {
    return (dynamic_cast<Const_n *>(n));
}

bool isIdent(ASTNode *n) {
    return (dynamic_cast<Ident_n *>(n));
}

bool isBinOp(ASTNode *n) {
    return (dynamic_cast<BinOp_n *>(n));
}

static int tempCount;
int getTempCount() {
    return tempCount++;
}

static int genConst(Const_n *node) {
    //Debug
    std::cout << "Current node is const" << std::endl;

    //create temp var
    int a = getTempCount();
    std::string tempname = "!tempvar" + std::to_string(a);
    int adrtemp = st->store(tempname, 0);

    // Fetch const value
    int const1 =
        (dynamic_cast<Const_n *>(node))->getValue();

    // generate asm
    output << "\tmovl\t$" << const1 << "," << adrtemp << "(%rbp)\n";

    // return var adress
    return adrtemp;
}

static int genBinOp(BinOp_n *node) {
    // Generating ASM for binary operators nodes
    // Check if node is a primary expression : const or var
    std::cout << "call to Binop" << std::endl;
    node->display();
    

    int leftadr;
    int rightadr;

    // Opérande gauche
    if (isBinOp(node->getLeft())) {
        // Le noeud de gauche est un opérateur
        // on effectue le calcul
        std::cout << "test" << std::endl;
        leftadr = genBinOp(dynamic_cast<BinOp_n *>(node->getLeft()));
        
    } else if (isConst(node->getLeft())) {
        
        // Le noeud de gauche est une constante
        node->getLeft()->display();
        std::cout << "test2" << std::endl;
        leftadr = genConst(dynamic_cast<Const_n *>(node->getLeft()));
    
    } else if (isIdent(node->getLeft())) {

        // Le noeud de gauche est une variable
        std::string name = (dynamic_cast<Ident_n *>(node->getLeft()))->getName();
        leftadr = st->getAddress(name);
    }

    // Opérande droite
    if (isBinOp(node->getRight())) {

        // Le noeud de gauche est un opérateur
        // on effectue le calcul
        rightadr = genBinOp(dynamic_cast<BinOp_n *>(node->getRight()));

    } else if (isConst(node->getRight())) {
        // Le noeud de gauche est une constante
        node->getLeft()->display();
        std::cout << "test2" << std::endl;
        rightadr = genConst(dynamic_cast<Const_n *>(node->getRight()));

    } else if (isIdent(node->getRight())) {
        // Le noeud de gauche est une variable

        // Ne marche probablement pas
        std::cout << "_test3" << std::endl;
        std::cout << "_test3.1" << std::endl;
        std::string name = (dynamic_cast<Ident_n *>(node->getRight()))->getName();
        std::cout << "_test3.2" << std::endl;
        rightadr = st->getAddress(name);
    }
    std::cout << "########### op in switch :  " << node->getOp()<< std::endl;
    switch (node->getOp()) {
    case '+':
        output << "\tmovl\t" << leftadr << "(%rbp), %edx\n";
        output << "\tmovl\t" << rightadr << "(%rbp), %eax\n";
        output << "\taddl\t%edx, %eax\n";
        output << "\tmovl\t%eax," << rightadr << "(%rbp)\n";
        return rightadr;
    case '-':
        output << "\tmovl\t" << rightadr << "(%rbp), %edx\n";
        output << "\tmovl\t" << leftadr << "(%rbp), %eax\n";
        output << "\tsubl\t%edx, %eax\n";
        output << "\tmovl\t%eax," << rightadr << "(%rbp)\n";
        return rightadr;
    case '*':
        output << "\tmovl    " << leftadr << "(%rbp), %eax\n";
        output << "\tmull\t" << rightadr << "(%rbp)\n";
        output << "\tmovl    %eax" << ", " << rightadr << "(%rbp)\n";
        return rightadr;
    default:
        std::cout << "Error : Unknow operator" << std::endl;
        exit(EXIT_FAILURE);
    }
    std::cout << "Erreur" << std::endl;
    return 0;
}

/**
 * Generate x86 assembly code
 */
static void asmgen(ASTNode *n) {

    tempCount = 0;

    output.open("output.s");
    prologue();
    ASTNode *current = n;
    if (dynamic_cast<Prog *>(current)) {
        std::cout << "print symbol" << std::endl;
        n->getST()->printSymbols();
        st = n->getST();
    }
    do {
        current = current->getFirst();
        ASTNode *temp = current;
        do {
            if (dynamic_cast<BinOp_n *>(temp)) {
                std::cout << "We have expr node" << std::endl;
                // Generate assembly from it
                genBinOp(dynamic_cast<BinOp_n *>(temp));
            } else if (dynamic_cast<Assign_n *>(temp)) {
                std::cout << "We have assign node" << std::endl;
                temp->display();
                std::string name = temp->getName();
                Expr_n *a(temp->getExpr());
                std::cout << "a display : "; a->display(); std::cout << std::endl;
                std::cout << "Assign variable : " << name << std::endl;
                if (dynamic_cast<Const_n *>(a)) {
                    Const_n *id = dynamic_cast<Const_n *>(a); 
                    int value = id->getValue();
                    std::cout << "value is " << value << std::endl;
                    output << "\tmovl\t$" << value << "," << st->getAddress(name) << "(%rbp)\n";
                    
                }
                else if (dynamic_cast<BinOp_n *>(a)) {
                    BinOp_n *b = dynamic_cast<BinOp_n *>(a);
                    b->display();
                    // Generate assembly
                    int adr = genBinOp(b);
                    int var_adr = st->getAddress(name);
                    output << "\tmovl\t" << adr << "(%rbp), %eax\n";
                    output << "\tmovl\t%eax," << var_adr << "(%rbp)\n";
                }
            } else if (dynamic_cast<Return_n *>(temp)) {
                std::cout << "Instruction de retour " << std::endl;
                if (dynamic_cast<Const_n *>(temp->getFirst())) {
                    std::cout << "return a const " << std::endl;
                    Const_n *n = dynamic_cast<Const_n *>(temp->getFirst());
                    int retconst = n->getValue();
                    output << "\tmovl\t$" << retconst << ",%eax\n";
                }
            } else {
                std::cout << "Error, unknow node" << std::endl;
            }
            temp = temp->getNext();
        } while (temp->hasNext());

        // Fin de la boucle, dernier noeud
        if (dynamic_cast<Return_n *>(temp)) {
            if (dynamic_cast<Const_n *>(temp->getFirst())) {
                std::cout << "return a const " << std::endl;
                
                Const_n *n = dynamic_cast<Const_n *>(temp->getFirst());
                output << "\tmovl\t$" << n->getValue() << ",%eax\n";
            
            } else if (dynamic_cast<Ident_n *>(temp->getFirst())) {
                Ident_n *t = dynamic_cast<Ident_n *>(temp->getFirst());
                int retvaradr = st->getAddress(t->getName());
                output << "\tmovl\t" << retvaradr << "(%rbp),%eax\n";
            }
        }
    } while (current->hasFirst());
    epilogue();
    output.close();
}
