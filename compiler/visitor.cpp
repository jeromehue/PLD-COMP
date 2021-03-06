
// Generated from ifcc.g4 by ANTLR 4.7.2

#include "visitor.h"
#include <string>
antlrcpp::Any Visitor::visitAxiom(ifccParser::AxiomContext *ctx) 
 {return visitChildren(ctx);}


antlrcpp::Any Visitor::visitProg(ifccParser::ProgContext *ctx) {
    
    std::cout<<".global main\n"
           " main: \n"
           "	#prologue\n"
           "	pushq %rbp\n"
           "	movq %rsp, %rbp\n"
           "\n"
           "	#body\n";
    visitChildren(ctx);
    int retval = 12;
    std::string retstr;
    if (ctx->value()->CONST() != nullptr) {
        retval = std::stoi(ctx->value()->CONST()->getText());
        retstr = "$" + std::to_string(retval);
    } else {
        //etstr = std::to_string(symboltable.symbols["a"]);
        int retval =  symboltable.getAdress(ctx->value()->ID()->getText());
        retstr = std::to_string(retval) + "(%rbp)";
        retval = -1;
    }
    std::cout <<   
            "	movl	"<<retstr<<", %eax\n"
            "\n"
            "	#epilogue\n"
            "   popq %rbp\n"
            " 	ret\n";

     return 0;

 }


antlrcpp::Any Visitor::visitAffectation(ifccParser::AffectationContext *ctx){
    // Checking if variable already exist
    std::string name = ctx->ID()->getText();
    if (!symboltable.find(name)) { 
        std::cout << "La variable a déjà été déclarée" << std::endl;
        exit(EXIT_FAILURE);
    }

    std::cout << "        movl    $" 
            << ctx->CONST()->getText() 
            << ", " 
            << symboltable.store(ctx->ID()->getText()) 
            <<  "(%rbp)\n";
    return visitChildren(ctx);
}


antlrcpp::Any Visitor::visitValue(ifccParser::ValueContext *ctx) {
    return visitChildren(ctx);
}
