#include "visitor.h"
#include "decl.h"
#include <vector>
#include <string>

antlrcpp::Any Visitor::visitAxiom(ifccParser::AxiomContext *ctx) 
 {return visitChildren(ctx);}

antlrcpp::Any Visitor::visitProg(ifccParser::ProgContext *ctx) {
    
    freeall_registers();
    cgprologue();    
    visitChildren(ctx);
    int retval = 12;
    std::string retstr;
   /* 
    if (ctx->primaryExpression()->CONST() != nullptr) {
        // Return an int
        retval = visi(ctx->primaryExpression);
        retstr = "$" + std::to_string(retval);
    } else {
        // return a var
        int retval =symboltable.getAddress(ctx->primaryExpression()->ID()->getText());
        retstr = std::to_string(retval) + "(%rbp)";
    }*/
    int a  = visit(ctx->primaryExpression());

      
    cgreturnreg(a);
    std::cout << "\n" << std::endl;
    cgepilogue();
        
    return 0;

 }

/*
antlrcpp::Any Visitor::visitAssignmentExpr
    (ifccParser::AssignmentExprContext *ctx)
{
    // Checking if variable already exist
    std::string name = ctx->ID()->getText();
    if (!symboltable.find(name)) { 
        std::cout << "La variable a déjà été déclarée" << std::endl;
        exit(EXIT_FAILURE);
    }
    int address = symboltable.store(ctx->ID()->getText()); 
    cgstorevar(stoi(ctx->CONST()->getText()), address) ;
    return visitChildren(ctx);
}*/

antlrcpp::Any Visitor::visitExpr(ifccParser::ExprContext *ctx)
{
    
    int left = visit(ctx->left);
    int right = visit(ctx->right);
    
    char op = ctx->op->getText().at(0);
    int ret; 
    switch(op) {
        case '+':
            ret  = cgadd(left, right);
            return ret;
            break;
        case '-': 
            ret = cgsub(left, right);
            return ret;
            break;
        case '*': 
            ret  = cgmul(left, right);
            return ret;
            break;
        case '/': 
            ret  = cgdiv(left, right); 
            return ret;
            break;
        default:
            std::cout << "Unknow operator" << std::endl;
            exit(EXIT_FAILURE);
            break;
    }
    return 0;
}

antlrcpp::Any Visitor::visitAssignmentExpr
(ifccParser::AssignmentExprContext *ctx) {
    int r =visit(ctx->arithExpr());
    std::string name = ctx->ID()->getText();
    int retval =  symboltable.getAddress(ctx->ID()->getText());
    
    cgloadvar(r, retval);
    return 0;
}

//antlrcpp::Any Visitor::visitRetval(ifccParser::RetvalContext *ctx) { 
    //int r  = visitChildren(ctx);
    //cgloadeax(r);
//    return 0;
//}


antlrcpp::Any Visitor::visitNumber(ifccParser::NumberContext *ctx) {
    int value = stoi(ctx->CONST()->getText());
    int r =cgloadint(value);
    return r;
}

antlrcpp::Any Visitor::visitVar(ifccParser::VarContext* ctx) {
    // We have reached var in an expresion
    std::string nom= ctx->ID()->getText();
    if(!symboltable.find(nom)){
        std::cout << "Undeclared Variable" << std::endl;
        exit(EXIT_FAILURE);
    }
    int addr =  symboltable.getAddress(ctx->ID()->getText());
    int r = cgvartoreg(addr);
    
    return r;
}

antlrcpp::Any Visitor::visitDeclaration(ifccParser::DeclarationContext *ctx) {
    /*
    auto v = ctx->ID();
    for (int i = 0; i< v.size(); ++i) {
        
        // Checking if variable already exist
        std::string name = ctx->ID(i)->getText();
        if (!symboltable.find(name)) { 
            std::cout << "La variable a déjà été déclarée" << std::endl;
            exit(EXIT_FAILURE);
        }
        
        int adr = symboltable.store(ctx->ID(i)->getText());
        cgstorevar(0, adr);
    }*/
    return visitChildren(ctx);
}

antlrcpp::Any Visitor::visitBlockItem(ifccParser::BlockItemContext *ctx) {
    return visitChildren(ctx);
}


antlrcpp::Any Visitor::visitInitDeclarator(ifccParser::InitDeclaratorContext* ctx) {
    std::string name = ctx->ID()->getText();
        if (symboltable.find(name)) { 
            std::cout << "La variable a déjà été déclarée" << std::endl;
            exit(EXIT_FAILURE);
        }
        std::cout << "\t# variable " << name << std::endl;    
        int value  = 0;
    if (ctx->arithExpr() == NULL) {
        
        int addr = symboltable.store(ctx->ID()->getText());
        cgstorevar(value, addr);
    }  else  {
        int reg = visit(ctx->arithExpr());
        int addr = symboltable.store(ctx->ID()->getText());
        cgloadvar(reg, addr); 
    }
    
    return 0;
}


antlrcpp::Any Visitor::visitEqualityExpression(ifccParser::EqualityExpressionContext *ctx) {
    return visitChildren(ctx);
}
/*
antlrcpp::Any Visitor::visitPrimaryExpression(ifccParser::PrimaryExpressionContext* ctx) {
    return visitChildren(ctx);
}*/


antlrcpp::Any Visitor::visitRelExpr(ifccParser::RelExprContext *ctx) {
    return visitChildren(ctx);
}
antlrcpp::Any Visitor::visitRe_var(ifccParser::Re_varContext *ctx) {
    return visitChildren(ctx);
}

antlrcpp::Any Visitor::visitRe_number(ifccParser::Re_numberContext *ctx) {
    return visitChildren(ctx);
}
