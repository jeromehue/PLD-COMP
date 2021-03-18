
// Generated from ifcc.g4 by ANTLR 4.7.2

#pragma once


#include "antlr4-runtime.h"
#include "generated/grammar/ifccVisitor.h"
#include "generated/grammar/ifccBaseVisitor.h"
#include "symboltable.h"

/**
 * This class provides an empty implementation of ifccVisitor, which can be
 * extended to create a visitor which only needs to handle a subset of the available methods.
 */
class  Visitor : public ifccBaseVisitor {
public:

  virtual antlrcpp::Any visitAxiom
      (ifccParser::AxiomContext *ctx) override;

  virtual antlrcpp::Any visitProg
      (ifccParser::ProgContext *ctx) override; 

  virtual antlrcpp::Any visitExpr 
      (ifccParser::ExprContext *ctx) override;

  virtual antlrcpp::Any visitNumber
      (ifccParser::NumberContext *ctx) override ;

  virtual antlrcpp::Any visitVar
      (ifccParser::VarContext *ctx) override ;

  virtual antlrcpp::Any visitDeclaration
      (ifccParser::DeclarationContext *ctx) override ;

  virtual antlrcpp::Any visitBlockItem 
      (ifccParser::BlockItemContext *ctx) override ;

  virtual antlrcpp::Any visitInitDeclarator 
      (ifccParser::InitDeclaratorContext *ctx) override ;

  virtual antlrcpp::Any visitAssignmentExpr 
      (ifccParser::AssignmentExprContext *ctx) override ;

  virtual antlrcpp::Any visitEqualityExpression 
      (ifccParser::EqualityExpressionContext *ctx) override ;


//  virtual antlrcpp::Any visitPrimaryExpression
//      (ifccParser::PrimaryExpressionContext *ctx) override;

  virtual antlrcpp::Any visitRelExpr
      (ifccParser::RelExprContext *ctx) override;

  virtual antlrcpp::Any visitRe_number
      (ifccParser::Re_numberContext *ctx) override;

  virtual antlrcpp::Any visitRe_var
      (ifccParser::Re_varContext *ctx) override;

protected: 
    Symboltable symboltable;
};

