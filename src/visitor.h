// Generatedgfrom ifcc.g4 by ANTLR 4.7.2

#pragma once

#include "antlr4-runtime.h"
#include "ifccVisitor.h"
#include "ifccBaseVisitor.h"
#include "ASTNode.h"
#include "symboltable.h"
#include <vector>

/**
 * This class provides an empty implementation of ifccVisitor, which can be
 * extended to create a visitor which only needs to handle a subset of the available methods.
 */
class Visitor : public ifccBaseVisitor {
public:

virtual antlrcpp::Any visitAxiom(ifccParser::AxiomContext *ctx) override;

virtual antlrcpp::Any visitProg(ifccParser::ProgContext *ctx) override;

virtual antlrcpp::Any visitReturnInstr(ifccParser::ReturnInstrContext *ctx) override;

virtual antlrcpp::Any visitVar(ifccParser::VarContext *ctx) override;

virtual antlrcpp::Any visitNumber(ifccParser::NumberContext *ctx) override;

virtual antlrcpp::Any visitStatement(ifccParser::StatementContext *ctx) override;

virtual antlrcpp::Any visitInitDeclaratorList(ifccParser::InitDeclaratorListContext *ctx) override;

virtual antlrcpp::Any visitDeclaration(ifccParser::DeclarationContext *ctx) override;

virtual antlrcpp::Any visitAssignArithExpr(ifccParser::AssignArithExprContext *ctx) override;

virtual antlrcpp::Any visitInitDeclarator(ifccParser::InitDeclaratorContext *ctx) override;

virtual antlrcpp::Any visitExpr(ifccParser::ExprContext *ctx) override;

virtual antlrcpp::Any visitPrExpr(ifccParser::PrExprContext *ctx) override;


//helper function 


// Getters
std::vector<Node*> getNodes() {
        return this->ast_nodes;
}

Symboltable *getST() {
    return &symboltable;
}

protected:
    std::vector<Node* > ast_nodes;
    Symboltable symboltable;
};
