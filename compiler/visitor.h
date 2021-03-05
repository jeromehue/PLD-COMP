
// Generated from ifcc.g4 by ANTLR 4.7.2

#pragma once


#include "antlr4-runtime.h"
#include "antlr4-generated/ifccVisitor.h"


/**
 * This class provides an empty implementation of ifccVisitor, which can be
 * extended to create a visitor which only needs to handle a subset of the available methods.
 */
class  Visitor : public ifccVisitor {
public:

  virtual antlrcpp::Any visitAxiom(ifccParser::AxiomContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitProg(ifccParser::ProgContext *ctx) override {
    val_ptr = -4;
     int retval = stoi(ctx->CONST()->getText()); //gérer erreurs
     std::cout<<".global main\n"
           " main: \n"
           "	#prologue\n"
           "	pushq %rbp\n"
           "	movq %rsp, %rbp\n"
           "\n"
           "	#body\n";
    visitChildren(ctx);
     std::cout <<   
            "	movl	$"<<retval<<", %eax\n"
            "\n"
            "	#epilogue\n"
            "   popq %rbp\n"
            " 	ret\n";

     return 0;
  }

  virtual antlrcpp::Any visitAffectation(ifccParser::AffectationContext *ctx) override {
      std::cout << "        movl    $" << ctx->CONST()->getText() << 
          ", " << val_ptr <<  "(%rbp)\n";
      val_ptr = val_ptr -4;
    return visitChildren(ctx);
  }

protected: 
  int val_ptr;
};

