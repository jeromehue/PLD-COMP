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
        if(ctx->declaration()) {
        ASTNode* declaration_node = visit(ctx->declaration());
        std::cout << "display of declaration" << std::endl;
        declaration_node->displayLinked();
        declaration_node->setEndNext(return_node);   
        std::cout << "end of visit prog" << std::endl;
        return declaration_node;      
        }
        else {
            return return_node;
        }
    }

  
  virtual antlrcpp::Any visitRetval
      (ifccParser::RetvalContext *ctx) override
        {

            //std::cout << "visit retval" << std::endl;
            if(ctx->CONST() != NULL ) {
                int val = stoi(ctx->CONST()->getText());
                // Création d'un noeud de type const
                ASTNode* retNode = new Return_n(NULL, NULL, NULL);
                Expr_n* expr = new Const_n(NULL, NULL, NULL, NULL, 1);
                ASTNode* n = new Const_n(NULL, NULL, NULL, NULL, 1);
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
        ASTNode* n = visit(ctx->initDeclaratorList());
        std::cout << "returned from visit declarator" << std::endl;
        if (n != nullptr) 
            n->display();
        return n;
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
       /* 
        std::cout << "List of declarators" << std::endl;
        auto v = ctx->initDeclarator()->getText();
        std::cout << "variable : " << v<< std::endl;
        std::cout << "temp to be initialized" << std::endl;
        
        // recursively parse trough declarations
        if (ctx->initDeclaratorList() != NULL )  {
            ASTNode* n = visit(ctx->initDeclaratorList());
            if(n != nullptr) {
                //std::cout << "add node to temp one" << std::endl;
            }
        }
        
        ASTNode * node = visit(ctx->initDeclarator());
        if (node == nullptr) {
                std::cout << "déclaration sans affectation" << std::endl;
        } else {
            std::cout<<"retour d'une affectation " << std::endl;
        }

        return node;
*/
        auto v = ctx->initDeclarator();
        //std::cout << typeid(v).name() << std::endl;
        std::cout << v.size() << std::endl;
        ASTNode* n = nullptr;
        for (int i = 0; i< v.size(); ++i){
            ASTNode * node = visit(ctx->initDeclarator(i));
            if (node == NULL) {
                std::cout << "déclaration sans affectation" << std::endl;
            } else {
                std::cout<<"retour d'une affectation " << std::endl;
                if (n== NULL) {
                    // Première affectation
                    n = node;
                } else {
                    // nième affectation
                    n->setEndNext(node);
                }
            }

        }
        return n;
    }

    virtual antlrcpp::Any visitInitDeclarator 
      (ifccParser::InitDeclaratorContext *ctx) override 
    {
        std::cout << "new entry in symbol table " << std::endl; 
        if(ctx->arithExpr() != NULL){
            std::cout << "assignement" << std::endl;
            //ASTNode* rvalue = visit(ctx->arithExpr());
            std::cout <<"left value = "<<ctx->ID()->getText()<<std::endl;
           
            // For testing purpose 
            Ident_n* lvalue = new Ident_n(NULL, NULL, "a");
            Expr_n* rvalue = new Const_n(NULL, NULL, NULL, NULL, 5);
            ASTNode* assign = new Assign_n(NULL, NULL, rvalue, lvalue);
            return assign;
        } else {
            std::cout << "Declaration without assignment" << std::endl;
            // Nothing to do here except creating a new entry
            // to symbol table
            std::cout  << ctx->ID()->getText() << std::endl; 
            ASTNode* a = NULL;
            return a;
        }
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

