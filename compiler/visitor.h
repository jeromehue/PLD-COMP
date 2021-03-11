
// Generated from ifcc.g4 by ANTLR 4.7.2

#pragma once


#include "antlr4-runtime.h"
#include "antlr4-generated/ifccVisitor.h"
#include "antlr4-generated/ifccBaseVisitor.h"
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

  virtual antlrcpp::Any visitAffectation
      (ifccParser::AffectationContext *ctx) override;

  virtual antlrcpp::Any visitRetval
      (ifccParser::RetvalContext *ctx) override;

  virtual antlrcpp::Any visitExpr 
      (ifccParser::ExprContext *ctx) override;

  virtual antlrcpp::Any visitNumber
      (ifccParser::NumberContext *ctx) override ;
  
  virtual antlrcpp::Any visitExpr_affectation
      (ifccParser::Expr_affectationContext *ctx) override ;
  
  virtual antlrcpp::Any visitVar
      (ifccParser::VarContext *ctx) override ;

  virtual antlrcpp::Any visitDeclaration
      (ifccParser::DeclarationContext *ctx) override ;
protected: 
    Symboltable symboltable;
};

