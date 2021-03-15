/*************************************************************************
                           ASTNode  -  description
                             -------------------
    début                : 2021-02-27
    copyright            : (C) 2021 par Jerome Hue et Charly Poirier
*************************************************************************/


#pragma once

#include<iostream>

// Les différents types de noeuds

using namespace std;

class ASTNode {

public:
    ASTNode(ASTNode* first, ASTNode* next) :
        next(next), first(first) {};

    void display() { std::cout << "Node " << std::endl; }
    
    void setFirst(ASTNode * n) {
        this->first = n;
    }

    bool hasNext() {
        return(this->next != NULL);
    }

    bool hasFirst() {
        return(this->first != NULL);
    }

    ASTNode* getNext() {
        return this->next;
    }

    ASTNode* getFirst() {
        return this->first;
    }


    protected:
        ASTNode* next;
        ASTNode* first;
};

class Prog : public  ASTNode {
public:
    Prog(ASTNode* first) : ASTNode(first, NULL) {};

protected:
};

class Expr : public ASTNode {
    public:
        Expr(ASTNode* first,   ASTNode* next,
                ASTNode* left,ASTNode* right) : 
            ASTNode(first, next), left(left), right(right) {};

    protected:
        ASTNode* left;
        ASTNode* right;

};

// Opérateur binaire : +, -, *, (/)
class BinOp : public Expr {

    public:
        BinOp(ASTNode* first,   ASTNode* next,
                ASTNode* left,ASTNode* right, char op) : 
            Expr(first, next, left, right),op(op) {};
    
    protected:
        char op;
};

class CONST : public Expr {
    
    public:
        CONST(ASTNode* first, ASTNode* next, ASTNode* left,ASTNode* right, int value) : 
            Expr(first, next, left, right) ,value(value) {};
        void display() {std::cout << "CONST(" << value << ")" << std::endl;}

    int getValue() {return this->value;}
    protected:
        int value;

};




