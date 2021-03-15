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
        std::cout << "call to visitAxiom" << std::endl;
        // Création de notre premier noeud : prog
        ASTNode* prog  = new Prog(nullptr);
        ASTNode* content = visit(ctx->prog());
        std::cout << "correct return from visitAxiom" << std::endl;
        prog->setFirst(content);
        return prog;
    }
  
    virtual antlrcpp::Any visitProg
      (ifccParser::ProgContext *ctx) override
    {
        //std::cout << "Call to visitProg" << std::endl;
        std::cout << ctx->getText() << std::endl;
        ASTNode* return_node = visit(ctx->retval()); 
        visitChildren(ctx); 
        return return_node;      
      
    }

  
  virtual antlrcpp::Any visitRetval
      (ifccParser::RetvalContext *ctx) override
        {

            //std::cout << "visit retval" << std::endl;
            if(ctx->CONST() != NULL ) {
                int val = stoi(ctx->CONST()->getText());
                // Création d'un noeud de type const
                ASTNode* retNode = new RETURN(NULL, NULL, NULL);
                Expr* expr = new CONST(NULL, NULL, NULL, NULL, 1);
                ASTNode* n = new CONST(NULL, NULL, NULL, NULL, 1);
                retNode->setExpr(expr);
                retNode->setFirst(expr);
                return retNode;
            } else {
              std::cout << "return a variable" << std::endl;
            }
            return 0;
      }
  
  
    virtual antlrcpp::Any visitExpr 
      (ifccParser::ExprContext *ctx) override 
    {
         
    //int left = visit(ctx->left);
    //int right = visit(ctx->right);
   
        //TODO Create the node recursibely

        char op = ctx->op->getText().at(0);
        int ret; 
        switch(op) {
        case '+':
            return ret;
            break;
        case '-': 
            return ret;
            break;
        case '*': 
            return ret;
            break;
        case '/': 
            return ret;
            break;
        default:
            std::cout << "Unknow operator : " << op << std::endl;
            exit(EXIT_FAILURE);
            break;
    }
    return 0;

      
    } 
  /*
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
                std::cout << "new entry in symbol table " 
                    << ctx->ID()->getText() << std::endl; 
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

