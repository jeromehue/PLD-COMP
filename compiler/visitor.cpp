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
    
    if (ctx->retval()->CONST() != nullptr) {
        // Return an int
        retval = std::stoi(ctx->retval()->CONST()->getText());
        retstr = "$" + std::to_string(retval);
    } else {
        // return a var
        int retval =symboltable.getAdress(ctx->retval()->ID()->getText());
        retstr = std::to_string(retval) + "(%rbp)";
    }
   
    cgreturnvar(retstr);
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
    int adress = symboltable.store(ctx->ID()->getText()); 
    cgstorevar(stoi(ctx->CONST()->getText()), adress) ;
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
    int r =visit(ctx->expression());
    std::string name = ctx->ID()->getText();
    int retval =  symboltable.getAdress(ctx->ID()->getText());
    
    cgloadvar(r, retval);
    return 0;
}

antlrcpp::Any Visitor::visitRetval(ifccParser::RetvalContext *ctx) { 
    //int r  = visitChildren(ctx);
    //cgloadeax(r);
    return 0;
}


antlrcpp::Any Visitor::visitNumber(ifccParser::NumberContext *ctx) {
    int value = stoi(ctx->CONST()->getText());
    int r =cgloadint(value);
    return r;
}

antlrcpp::Any Visitor::visitVar(ifccParser::VarContext* ctx) {
    // We have reached var in an expresion
    int addr =  symboltable.getAdress(ctx->ID()->getText());
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
        if (!symboltable.find(name)) { 
            std::cout << "La variable a déjà été déclarée" << std::endl;
            exit(EXIT_FAILURE);
        }
        std::cout << "\t# variable " << name << std::endl;    
        int value  = 0;
    if (ctx->expression() == NULL) {
        
        int addr = symboltable.store(ctx->ID()->getText());
        cgstorevar(value, addr);
    }  else  {
        int reg = visit(ctx->expression());
        int addr = symboltable.store(ctx->ID()->getText());
        cgloadvar(reg, addr); 
    }
    
    return 0;
}
