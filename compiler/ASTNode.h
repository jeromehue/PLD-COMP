/*************************************************************************
                           ASTNode  -  description
                             -------------------
    début                : 2021-02-27
    copyright            : (C) 2021 par Jerome Hue et Charly Poirier
*************************************************************************/


#pragma once

#include<iostream>

// Les différents types de noeuds


class ASTNode {

    public:
    ASTNode(ASTNode* first, ASTNode* next) :
        next(next), first(first) {};

    protected:
        ASTNode* next;
        ASTNode* first;
};

class Prog : public  ASTNode {
    public:
    Prog(ASTNode* first) : ASTNode(first, NULL) {};
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

class IDENT : public Expr {
    
    public:
        IDENT(ASTNode* first, ASTNode* next, ASTNode* left,ASTNode* right, int value) : 
            Expr(first, next, left, right) ,value(value) {};
    
    protected:
        int value;

};




