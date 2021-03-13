#pragma once

#include "antlr4-runtime.h"
#include "antlr4-generated/ifccVisitor.h"
#include "antlr4-generated/ifccBaseVisitor.h"


#include "symboltable.h"
#include "ASTNode.h"
#include "ifccBaseVisitor.h"

#include <vector>

class  ASTgen : public ifccBaseVisitor {
public:

    virtual antlrcpp::Any visitAxiom
      (ifccParser::AxiomContext *ctx) override 
    {
        std::cout << "Bonjour" << std::endl;
        // Création de notre premier noeud : prog
        ASTNode* prog  = new Prog(nullptr);
        
        visitChildren(ctx);
        return prog;
    }
  
    virtual antlrcpp::Any visitProg
      (ifccParser::ProgContext *ctx) override
    {
        std::cout << "Call to visitProg" << std::endl;
        // Création de la définition de la fonction main
        visitChildren(ctx); 
        return 0;      
      
    }

  
  virtual antlrcpp::Any visitRetval
      (ifccParser::RetvalContext *ctx) override
      {
          std::cout << "visit retval" << std::endl;
          if(ctx->CONST() != NULL ) {
              std::cout << "return a const" << std::endl;

          } else {
              std::cout << "return a variable" << std::endl;
          }
          
          return 0;
      }
  
  /*
  virtual antlrcpp::Any visitExpr 
      (ifccParser::ExprContext *ctx) override;

  virtual antlrcpp::Any visitNumber
      (ifccParser::NumberContext *ctx) override ;

  virtual antlrcpp::Any visitVar
      (ifccParser::VarContext *ctx) override ;
*/
    virtual antlrcpp::Any visitDeclaration
      (ifccParser::DeclarationContext *ctx) override 
    {
        std::cout << "visitDeclaration" << std::endl;      
        std::cout << "Type : " << ctx->TYPE()->getText() << std::endl;
        visitChildren(ctx);
        return 0;
    }
    
    virtual antlrcpp::Any visitBlockItem 
      (ifccParser::BlockItemContext *ctx) override 
    {
          std::cout << "visitBlockItem" << std::endl;
          visitChildren(ctx);
          return 0;
    }

    virtual antlrcpp::Any visitInitDeclaratorList
        (ifccParser::InitDeclaratorListContext * ctx) override 
    {
        std::cout << "List of declarators" << std::endl;
        auto v = ctx->initDeclarator()->getText();
        std::cout << "variable : " << v<< std::endl;
        visitChildren(ctx);
        return 0;
    }

    virtual antlrcpp::Any visitInitDeclarator 
      (ifccParser::InitDeclaratorContext *ctx) override 
      {
          std::cout << "Init declarator" << std::endl;
          if(ctx->arithExpr() != NULL){
              std::cout << "assignement" << std::endl;
          } else {
              std::cout << "simple declaration" << std::endl;
          }
          return 0;
      }

  /*
  virtual antlrcpp::Any visitAssignmentExpr 
      (ifccParser::AssignmentExprContext *ctx) override ;

  virtual antlrcpp::Any visitEqualityExpression 
      (ifccParser::EqualityExpressionContext *ctx) override ;
    

  virtual antlrcpp::Any visitRelExpr
      (ifccParser::RelExprContext *ctx) override;
*/
    virtual antlrcpp::Any visitRe_number
      (ifccParser::Re_numberContext *ctx) override
    {
        std::cout << "return a number" << std::endl;
        return 0;
    }
  
  /*
  virtual antlrcpp::Any visitRe_var
      (ifccParser::Re_varContext *ctx) override;

protected: 
    Symboltable symboltable;*/
};

