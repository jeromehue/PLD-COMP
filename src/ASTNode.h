/*************************************************************************
                           ASTNode  -  description
                             -------------------
    début                : 2021-02-27
    copyright            : (C) 2021 par H4244 
*************************************************************************/

#pragma once

#include <iostream>

using namespace std;

// Les différents types de noeuds
enum nodeType {

    N_CONST,
    N_IDENT
};

class Expr_n;

class ASTNode {
public:
    ASTNode(ASTNode *first, ASTNode *next) : next(next), first(first) { }
    virtual void display() { std::cout << "Node " << std::endl; }
    virtual void setExpr(Expr_n *expr) {}
    virtual Expr_n *getExpr() { return NULL; }
    void setFirst(ASTNode *n) {
        this->first = n;
    }
    void setNext(ASTNode *n) {
        this->next = n;
    }
    bool hasNext() {
        return (this->next != NULL);
    }
    void displayLinked() {
        std::cout << "->";
        this->display();
        ASTNode *n = this;
        while (n->hasNext()) {
            std::cout << "->";
            n = n->next;
            n->display();
        }
    }
    void setEndNext(ASTNode *last) {
        ASTNode *n = this;
        while (n->hasNext()) {
            n = n->next;
        }
        n->setNext(last);
    }
    bool hasFirst() {
        return (this->first != NULL);
    }
    ASTNode *getNext() {
        return this->next;
    }
    ASTNode *getFirst() {
        return this->first;
    }
    virtual void setST(Symboltable *st) { }
    virtual Symboltable *getST() { return NULL; }
    virtual std::string getName() { return NULL; }
protected:
    ASTNode *next;
    ASTNode *first;
};

class Prog : public ASTNode {
public:
    Prog(ASTNode *first) : ASTNode(first, NULL) { }
    void setST(Symboltable *st) {
        this->symbolTable = st;
    }
    virtual Symboltable *getST() {
        std::cout << "get ST" << std::endl;
        return this->symbolTable;
    }
protected:
    Symboltable *symbolTable;
};

class Expr_n : public ASTNode {
public:
    Expr_n(ASTNode *first, ASTNode *next,
           ASTNode *left, ASTNode *right) : ASTNode(first, next), left(left), right(right) { }
    virtual void display() { std::cout << "Expr Node " << std::endl; }
protected:
    ASTNode *left;
    ASTNode *right;
};

// Opérateur binaire : +, -, *, (/)
class BinOp_n : public Expr_n {
public:
    BinOp_n(ASTNode *first, ASTNode *next,
            ASTNode *left, ASTNode *right, char op) : Expr_n(first, next, left, right), op(op) {
        std::cout << "Creating node binop " << op << std::endl;
    }
    ASTNode *getLeft() {
        return left;
    }
    ASTNode *getRight() {
        return right;
    }
    char getOp() {
        return op;
    }
    void display() {
        std::cout << "Bin op : " << op << std::endl;
    }
protected:
    char op;
};

class Const_n : public Expr_n {
public:
    Const_n(ASTNode *first, ASTNode *next,
            ASTNode *left, ASTNode *right,
            int value) : Expr_n(first, next, left, right), value(value) {
        std::cout << "Creating const node "
                  << value << std::endl;
    }
    void display() {
        std::cout << "CONST(" << value << ")" << std::endl;
    }
    int getValue() {
        return this->value;
    }
protected:
    int value;
};

class Return_n : public ASTNode {
public:
    Return_n(ASTNode *first, ASTNode *next,
             Expr_n *retExpr) : ASTNode(retExpr, next), retExpr(retExpr) { }
    void display() {
        std::cout << "RETURN Node" << std::endl;
    }
    void setExpr(Expr_n *expr) {
        this->retExpr = expr;
    }
protected:
    Expr_n *retExpr;
};

class Ident_n : public Expr_n {
public:
    Ident_n(ASTNode *first, ASTNode *next, std::string name) : Expr_n(first, next, NULL, NULL), name(name) {
        std::cout << "Creating node ident" << std::endl;
    }
    void display() {
        std::cout << "Ident(" << name << ")" << std::endl;
    }
    std::string getName() {
        return name;
    }
protected:
    std::string name;
};

// Assign node
// lvalue : ident
// rvalue : expression
class Assign_n : public ASTNode {
public:
    Assign_n(ASTNode *first, ASTNode *next, Expr_n *rvalue, Ident_n *lvalue)
        : ASTNode(first, next), rvalue(rvalue), lvalue(lvalue) { }
    void display() {
        std::cout << "ASSIGN : " << lvalue->getName() << std::endl;
    }
    Expr_n *getExpr() {
        return this->rvalue;
    }
    Ident_n *getVar() {
        return this->lvalue;
    }
    std::string getName() {
        return lvalue->getName();
    }
private:
    Expr_n *rvalue;
    Ident_n *lvalue;
};
