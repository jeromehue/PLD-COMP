#include "visitor.h"
#include "ASTNode.h"
#include "cgen.h"
#include <cassert>
#include <vector>
#include <string>

antlrcpp::Any Visitor::visitAxiom(ifccParser::AxiomContext *ctx) {
    return visitChildren(ctx);
}

antlrcpp::Any Visitor::visitProg(ifccParser::ProgContext *ctx) {
    std::cout << "Call to visitProg" << std::endl;
    return visitChildren(ctx);
}

antlrcpp::Any Visitor::visitReturnInstr(ifccParser::ReturnInstrContext *ctx) {

    // Debug print
    std::cout << "Call to visitReturnInstr" << std::endl;
    
    // Node definition
    Node* n = new Node(OP_RETURN, NULL, NULL,0, 0); 
    ast_nodes.push_back(n);

    // Setting the return expression as a child for 
    // our return node
    int index = ast_nodes.size() - 1;
    visitChildren(ctx);
    ast_nodes[index]->left = ast_nodes[index+1];
    ast_nodes.pop_back();

    // My job here is done
    return 0;
}


antlrcpp::Any Visitor::visitNumber(ifccParser::NumberContext *ctx) {
    // Debug print
    std::cout << "Call to visitNumber" << std::endl;


    // Node definition
    int a = stoi(ctx->CONST()->getText());
    Node* n = new Node(OP_CONST, NULL, NULL, a, 0);
    ast_nodes.push_back(n);
    
    return visitChildren(ctx);
}

antlrcpp::Any Visitor::visitVar(ifccParser::VarContext *ctx) {
        std::cout << "Call to visitVar" << std::endl;
        return visitChildren(ctx);
}


antlrcpp::Any Visitor::visitStatement(ifccParser::StatementContext *ctx) {

    // Debug print
    std::cout << "Call to visitStatement" << std::endl;
    
    return visitChildren(ctx);
}

antlrcpp::Any Visitor::visitDeclaration(ifccParser::DeclarationContext *ctx) { 

    // Debug print
    std::cout << "Call to visitDeclaration" << std::endl;

    return visitChildren(ctx);
}

antlrcpp::Any Visitor::visitInitDeclaratorList(ifccParser::InitDeclaratorListContext *ctx) {

    // Debug print
    std::cout << "Call to visitInitDeclaratorList" << std::endl;

    return visitChildren(ctx);

}

antlrcpp::Any Visitor::visitInitDeclarator(ifccParser::InitDeclaratorContext *ctx) {
    std::cout << "Call to visitInitDeclarator" << std::endl;
    
    if(ctx->arithExpr()) {
        std::cout << " >> Declaration and affectaion" ;
    }  else {
        std::cout << " >> Declaration w/o affectation";
    }
    std::cout << " of " << ctx->ID()->getText() << std::endl;


    // Let's insert or var in the symbol table 
    std::string var_name = ctx->ID()->getText();
    symboltable.store(var_name, 0);

    if (ctx->arithExpr()) {
        
        // If assign, we create an IDENT node that's going to 
        // be the left child of out ASSIGN node
        int var_adr = symboltable.getAddress(var_name);
        Node* var = new Node(OP_IDENT, NULL, NULL, var_adr, 0);

        // Now let's deal with the ASSIGN node
        Node* n = new Node(OP_ASSIGN, var, NULL, 0, 0);

        // Finishing with the expr node
        int index = ast_nodes.size() - 1;
        visitChildren(ctx);
        /*
        // In case we added nothing or more than one node 
        assert(ast_nodes.size() == index+2);
        ast_nodes[index]->left = ast_nodes[index+1];
        ast_nodes.pop_back();
        */
    }
        
    return 0;
}

antlrcpp::Any Visitor::visitAssignArithExpr(ifccParser::AssignArithExprContext *ctx) {
    // Debug print
    std::cout <<"Call to AssignArithExpr " << std::endl;
    std::cout << " >> Assigning " << ctx->ID()->getText()
        << " to " << ctx->arithExpr()->getText() << std::endl;

    // Node definition
    Node* n = new Node(OP_ASSIGN, NULL, NULL, 0, 0);
    ast_nodes.push_back(n);
    /*
        n->left is going to be the variable
        n->right is going to be an expression
    */  
    std::string var_name = ctx->ID()->getText();
    // TODO A finir sur le modèle du neour retour :
    // On empile et après on mets à jour les enfants
    Node* o = new Node(OP_IDENT, NULL, NULL, 0, 0);
    

    return visitChildren(ctx);
}

antlrcpp::Any Visitor::visitExpr(ifccParser::ExprContext* ctx){
    std::cout << "Call to visit Expr" << std::endl;

    return visitChildren(ctx);
}


antlrcpp::Any Visitor::visitPrExpr(ifccParser::PrExprContext* ctx) {
    std::cout << "Call to visit prExpression" << std::endl;

    return visitChildren(ctx);
}
