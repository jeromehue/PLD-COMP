
// Generated from ifcc.g4 by ANTLR 4.7.2

#pragma once


#include "antlr4-runtime.h"
#include "antlr4-generated/ifccVisitor.h"
#include "symboltable.h"

/**
 * This class provides an empty implementation of ifccVisitor, which can be
 * extended to create a visitor which only needs to handle a subset of the available methods.
 */
class  Visitor : public ifccVisitor {
public:

  virtual antlrcpp::Any visitAxiom(ifccParser::AxiomContext *ctx) override;

  virtual antlrcpp::Any visitProg(ifccParser::ProgContext *ctx) override; 

  virtual antlrcpp::Any visitAffectation
      (ifccParser::AffectationContext *ctx) override;
protected: 
    Symboltable symboltable;
};

