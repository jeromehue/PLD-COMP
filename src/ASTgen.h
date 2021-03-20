#pragma once

#include "antlr4-runtime.h"
#include "ifccVisitor.h"
#include "ifccBaseVisitor.h"

#include "symboltable.h"
#include "ASTNode.h"

#include <vector>

class ASTgen : public ifccBaseVisitor {
public:
    virtual antlrcpp::Any visitAxiom(ifccParser::AxiomContext *ctx) override {
        std::cout << "Call to visitAxiom()" << std::endl;
        // Création de notre premier noeud : prog
        ASTNode *prog = new Prog(nullptr);
        prog->setST(new Symboltable());
        symboltable = (prog->getST());
        ASTNode *content = visit(ctx->prog());
        std::cout << "correct return from visitAxiom" << std::endl;
        prog->setFirst(content);
        return prog;
    }

    virtual antlrcpp::Any visitProg(ifccParser::ProgContext *ctx) override {
        std::cout << "Call to visitProg()" << std::endl;
        std::cout << "Programme \n"
                  << ctx->getText() << std::endl;
        ASTNode *return_node = visit(ctx->primaryExpression());
        ASTNode *abc =
            (visit(ctx->primaryExpression()));
        abc->display();
        if (dynamic_cast<Expr_n *>(abc)) {
            std::cout << "oui" << std::endl;
            return_node = new Return_n(NULL, NULL,
                                       dynamic_cast<Expr_n *>(abc));
        }
        if (ctx->declaration()) {
            ASTNode *declaration_node = visit(ctx->declaration());
            std::cout << "Display of declaration" << std::endl;
            declaration_node->displayLinked();
            for (int i = 0; i < ctx->statement().size(); ++i) {
                std::cout << "ANDL1" << std::endl;
                auto st = visit(ctx->statement(i));
                std::cout << typeid(st).name() << std::endl;
                //visit(ctx->statement(i));
                std::cout << "ANDL2" << std::endl;
                declaration_node->setEndNext(st);
            }
            declaration_node->setEndNext(return_node);
            std::cout << "End of visitProg()" << std::endl;
            return declaration_node;
        } else {
            return return_node;
        }
    }

    virtual antlrcpp::Any visitStatement(ifccParser::StatementContext *ctx) override {

        std::cout << "call to visitStatemet" << std::endl;
        int nbAssign = ctx->assignmentExpr().size();
        // Create the first node and
        ASTNode *first = new ASTNode(NULL, NULL);
        for (int i = 0; i < nbAssign; ++i) {
            ASTNode *a = visit(ctx->assignmentExpr(0));
            std::cout << "Retrieved node" << std::endl;
            first->setEndNext(a);
        }
        ASTNode *final_node = first->getNext();
        return final_node;
    }
    virtual antlrcpp::Any visitExpr(ifccParser::ExprContext *ctx) override {
        std::cout << "Call to visitExpr" << std::endl;
        char op = ctx->op->getText().at(0);
        ASTNode *left = visit(ctx->left);
        ASTNode *right = visit(ctx->right);
        ASTNode *n = new BinOp_n(NULL, NULL, left, right, op);
        //TODO Create the node recursibely
        return n;
    }

    virtual antlrcpp::Any visitNumber(ifccParser::NumberContext *ctx) override {
        std::cout << "Call to visitNumber" << std::endl;
        ASTNode *n = new Const_n(NULL, NULL, NULL, NULL,
                                 stoi(ctx->CONST()->getText()));
        return n;
    }

    virtual antlrcpp::Any visitVar(ifccParser::VarContext *ctx) override {
        std::cout << "Call to visitVar" << std::endl;
        ASTNode *n = new Ident_n(NULL, NULL, ctx->ID()->getText());
        return n;
    }

    virtual antlrcpp::Any visitDeclaration(ifccParser::DeclarationContext *ctx) override {
        std::cout << "Call to visitDeclaration()" << std::endl;
        std::cout << "Type of declarator: "
                  << ctx->TYPE()->getText() << std::endl;
        ASTNode *n = visit(ctx->initDeclaratorList());
        if (n != nullptr)
            n->display();
        return n;
    }

    virtual antlrcpp::Any visitBlockItem(ifccParser::BlockItemContext *ctx) override {
        std::cout << "Call to visitBlockItem()" << std::endl;
        visitChildren(ctx);
        return 0;
    }

    virtual antlrcpp::Any visitInitDeclaratorList(ifccParser::InitDeclaratorListContext *ctx) override {
        std::cout << "Call to visitInitDeclaratorList()" << std::endl;
        // On récupère un vector avec tous les éléments déclarés
        auto v = ctx->initDeclarator();
        // Et on affiche sa taille (i.e. le nombre d'éléments déclarés)
        std::cout << "Nombre de déclarations " << v.size() << std::endl;
        ASTNode *n = nullptr;
        for (int i = 0; i < v.size(); ++i) {
            ASTNode *node = visit(ctx->initDeclarator(i));
            if (node == NULL) {
                // ...
            } else {
                // std::cout << "Déclaration avec affectation" << std::endl;
                if (n == NULL) {
                    // Première affectation
                    n = node;
                } else {
                    // n-ième affectation
                    n->setEndNext(node);
                }
            }
        }
        return n;
    }

    virtual antlrcpp::Any visitInitDeclarator(ifccParser::InitDeclaratorContext *ctx) override {

        std::cout << "Nouvelle entrée dans la table des symboles"
                  << std::endl;
        std::string var_name = ctx->ID()->getText();
        symboltable->store(var_name, 0);
        symboltable->printSymbols();

        if (ctx->arithExpr() != NULL) {
            std::cout << "Affectation :"
                      << " left value = " << ctx->ID()->getText() << std::endl;
            std::string id = ctx->ID()->getText();
            ASTNode *e = visit(ctx->arithExpr());
            e->display();
            int val;
            if (dynamic_cast<Const_n *>(e)) {
                val = (dynamic_cast<Const_n *>(e))->getValue();
            } else {
                val = 0;
            }
            // For testing purpose
            Ident_n *lvalue = new Ident_n(NULL, NULL, id);
            Expr_n *rvalue = new Const_n(NULL, NULL, NULL, NULL, val);
            ASTNode *assign = new Assign_n(NULL, NULL, rvalue, lvalue);
            return assign;
        } else {
            std::cout << "Declaration sans affectation ";
            // Nothing to do here except creating a new entry
            // to symbol table
            std::cout << ctx->ID()->getText() << std::endl;
            ASTNode *a = NULL;
            return a;
        }
    }

    virtual antlrcpp::Any visitAssignArithExpr(ifccParser::AssignArithExprContext *ctx) override {

        std::cout << "Call to visitAssignmentExpr" << std::endl;
        std::cout << "Variable : " << ctx->ID()->getText() << std::endl;
        Ident_n *lvalue = new Ident_n(NULL, NULL, ctx->ID()->getText());
        std::cout << "????" << std::endl;
        ASTNode *a = visit(ctx->arithExpr());
        a->display();
        std::cout << typeid(a).name() << std::endl;
        std::cout << "ANLD" << std::endl;
        ASTNode *test = new Assign_n(NULL, NULL, dynamic_cast<Expr_n *>(a), lvalue);
        return test;
    }

protected:
    Symboltable *symboltable;
};
