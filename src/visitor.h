// Generatedgfrom ifcc.g4 by ANTLR 4.7.2

#ifndef VISITOR_H
#define VISITOR_H

#include <vector>

#include "antlr4-runtime.h"
#include "ifccVisitor.h"
#include "ifccBaseVisitor.h"
#include "node.h"
#include "symb.h"

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
        
        virtual antlrcpp::Any visitChar(ifccParser::CharContext *ctx) override;

        virtual antlrcpp::Any visitStatement(ifccParser::StatementContext *ctx) override;

        virtual antlrcpp::Any visitInitDeclaratorList(ifccParser::InitDeclaratorListContext *ctx) override;

        virtual antlrcpp::Any visitDeclaration(ifccParser::DeclarationContext *ctx) override;

        virtual antlrcpp::Any visitAssignArithExpr(ifccParser::AssignArithExprContext *ctx) override;

        virtual antlrcpp::Any visitInitDeclarator(ifccParser::InitDeclaratorContext *ctx) override;

        virtual antlrcpp::Any visitExpr(ifccParser::ExprContext *ctx) override;

        virtual antlrcpp::Any visitPrExpr(ifccParser::PrExprContext *ctx) override;

        virtual antlrcpp::Any visitRelExpr(ifccParser::RelExprContext *ctx) override;

        virtual antlrcpp::Any visitAssignRelExpr(ifccParser::AssignRelExprContext *ctx) override;

        virtual antlrcpp::Any visitAssignFunction(ifccParser::AssignFunctionContext *ctx) override;

        virtual antlrcpp::Any visitIf(ifccParser::IfContext *ctx) override;

        virtual antlrcpp::Any visitIfElse(ifccParser::IfElseContext *ctx) override;

        virtual antlrcpp::Any visitFunction(ifccParser::FunctionContext* ctx) override;

        virtual antlrcpp::Any visitParameterlist(ifccParser::ParameterlistContext* ctx) override;

        virtual antlrcpp::Any visitBlockStatement(ifccParser::BlockStatementContext* ctx) override;

        // Getters
        std::vector<Node*> getNodes()
        {
                return this->ast_nodes;
        }

        Symboltable *getglobalSymb()
        {
                return globalSymb;
        }

        std::vector<Function*> getFunctions()
        {
                return functions;
        }

protected:
        std::vector<Function*> functions;
        Function* curfct;
        std::vector<Node*> ast_nodes;
        Symboltable * globalSymb;
        Symboltable * localSymb;
};

#endif
