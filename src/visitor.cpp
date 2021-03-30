#include "visitor.h"
#include "ASTNode.h"
#include <cassert>
#include <vector>
#include <string>

// Improving readability
using namespace antlrcpp;
using std::cout;

Any Visitor::visitAxiom(ifccParser::AxiomContext *ctx) 
{
        return visitChildren(ctx);
}

Any Visitor::visitProg(ifccParser::ProgContext *ctx) 
{
        cout << "Call to visitProg" << std::endl;
        return visitChildren(ctx);
}

Any Visitor::visitReturnInstr(ifccParser::ReturnInstrContext *ctx) 
{    
        // Debug print
        cout << "Call to visitReturnInstr" << std::endl;
    
        // Node definition
        Node* n = new Node(OP_RETURN, NULL, NULL,0, 0); 
        ast_nodes.push_back(n);

        // Setting the return expression as a child for 
        // our return node
        int index = ast_nodes.size() - 1;
        visitChildren(ctx);
        assert(ast_nodes.size() == index+2);
        ast_nodes[index]->ndlist[0] = ast_nodes[index+1];
        ast_nodes.pop_back();

        // My job here is done
        return 0;
}


Any Visitor::visitNumber(ifccParser::NumberContext *ctx) 
{
        // Debug print
        cout << "Call to visitNumber" << std::endl;


        // Node definition
        int a = stoi(ctx->CONST()->getText());
        Node* n = new Node(OP_CONST, NULL, NULL, a, 0);
        ast_nodes.push_back(n);
    
        return visitChildren(ctx);
}

Any Visitor::visitVar(ifccParser::VarContext *ctx) 
{    
        cout << "Call to visitVar" << std::endl;
    
        //Node definition
        std::string var_name = ctx->ID()->getText();
        int var_adr = symboltable.getAddress(var_name);
        Node* var = new Node(OP_IDENT, NULL, NULL, var_adr, 0);
        ast_nodes.push_back(var);
    
        return visitChildren(ctx);
}


Any Visitor::visitStatement(ifccParser::StatementContext *ctx) 
{
        // Debug print
        cout << "Call to visitStatement" << std::endl;
    
        return visitChildren(ctx);
}

Any Visitor::visitIfStatement(ifccParser::IfStatementContext *ctx) 
{
        //Debug print
        cout << "Call to visit Statement " << std::endl;
        exit(EXIT_FAILURE);

        return 0;
}

Any Visitor::visitDeclaration (ifccParser::DeclarationContext *ctx) 
{ 
        // Debug print
        cout << "Call to visitDeclaration" << std::endl;

        return visitChildren(ctx);
}

Any Visitor::visitInitDeclaratorList(ifccParser::InitDeclaratorListContext *ctx) 
{
        // Debug print
        cout << "Call to visitInitDeclaratorList" << std::endl;

        return visitChildren(ctx);

}

Any Visitor::visitInitDeclarator(ifccParser::InitDeclaratorContext *ctx) 
{
        cout << "Call to visitInitDeclarator" << std::endl;
    
        if(ctx->arithExpr()) {
                cout << " >> Declaration and affectaion" ;
        } else {
                cout << " >> Declaration w/o affectation";
        }
        cout << " of " << ctx->ID()->getText() << std::endl;


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
                ast_nodes.push_back(n);

                // Finishing with the expr node

                int index = ast_nodes.size() - 1;
                visitChildren(ctx);
                assert(ast_nodes.size() == index+2);
                ast_nodes[index]->ndlist[1] = ast_nodes[index+1];
                ast_nodes.pop_back();
        }
        
        return 0;
}

Any Visitor::visitAssignArithExpr(ifccParser::AssignArithExprContext *ctx) 
{
        // Debug print
        cout <<"Call to AssignArithExpr " << std::endl;
        cout << " >> Assigning " << ctx->ID()->getText()
        << " to " << ctx->arithExpr()->getText() << std::endl;

        // Node definition
        /*
         * n->left is going to be the variable
         * n->right is going to be an expression
        */  
        
        std::string var_name = ctx->ID()->getText();
        int var_adr = symboltable.getAddress(var_name);
        Node* o = new Node(OP_IDENT, NULL, NULL, var_adr, 0);
   
        int ref = ast_nodes.size();
        // Expression 
        visitChildren(ctx);
        assert(ast_nodes.size()== ref+1);


        Node* n = new Node(OP_ASSIGN, o, ast_nodes.back(), 0, 0);
        ast_nodes.pop_back(); 
        ast_nodes.push_back(n);

        return 0; 
}
Any Visitor::visitAssignRelExpr(ifccParser::AssignRelExprContext *ctx) 
{
        // Debug print
        cout <<"Call to AssignRelExpr " << std::endl;
        cout << " >> Assigning " << ctx->ID()->getText()
        << " to " << ctx->relationalExpression()->getText() << std::endl;

        // Node definition
        /*
         * n->left is going to be the variable
         * n->right is going to be an expression
        */  
        
        std::string var_name = ctx->ID()->getText();
        int var_adr = symboltable.getAddress(var_name);
        Node* o = new Node(OP_IDENT, NULL, NULL, var_adr, 0);
   
        int ref = ast_nodes.size();
        // Expression 
        visitChildren(ctx);
        assert(ast_nodes.size()== ref+1);


        Node* n = new Node(OP_ASSIGN, o, ast_nodes.back(), 0, 0);
        ast_nodes.pop_back(); 
        ast_nodes.push_back(n);

        return 0; 
}

Any Visitor::visitExpr(ifccParser::ExprContext* ctx)
{
        cout << "Call to visit Expr" << std::endl;

        char op = ctx->op->getText().at(0);
        int ref = ast_nodes.size();
        visit(ctx->left);
        visit(ctx->right);
    
   
        assert(ast_nodes.size() == ref+2);
 
        switch(op) 
        {
        case '+': {
                cout << "Addition" << std::endl;
                Node* nop = new Node(
                    OP_ADD, ast_nodes[ref+1], ast_nodes[ref], 0, 0);
                ast_nodes.pop_back();
                ast_nodes.pop_back();
                ast_nodes.push_back(nop);
                nop->display();
                break;
        }
        case '-': {
                cout << "Subtraction" << std::endl;
                Node* nop = new Node(
                    OP_SUB, ast_nodes[ref+1], ast_nodes[ref], 0, 0);
                ast_nodes.pop_back();
                ast_nodes.pop_back();
                ast_nodes.push_back(nop);
                nop->display();
                break;
        } 
        case '*': {
                cout << "Multiplication" << std::endl;
                Node* nop = new Node(OP_MUL, ast_nodes[ref+1], 
                                     ast_nodes[ref], 0, 0);
                ast_nodes.pop_back();
                ast_nodes.pop_back();
                ast_nodes.push_back(nop);
                nop->display();
                break;
                return 0;
        }
        default:
                cout << "Erreur, opérateur non reconnu" << std::endl;
                exit(EXIT_FAILURE);
        }

        return 0;
}

Any Visitor::visitRelExpr(ifccParser::RelExprContext* ctx)
{
        cout << "Call to visit RelExpr" << std::endl;

        char relop = ctx->relOp->getText().at(0);
        int ref = ast_nodes.size();
        visit(ctx->left);
        visit(ctx->right);
    
   
        assert(ast_nodes.size() == ref+2);
 
        switch(relop) 
        {
        case '<': {
                cout << "inferieur" << std::endl;
                Node* nop = new Node(
                    OP_LOWER, ast_nodes[ref+1], ast_nodes[ref], 0, 0);
                ast_nodes.pop_back();
                ast_nodes.pop_back();
                ast_nodes.push_back(nop);
                nop->display();
                break;
        }
        case '>': {
                cout << "superieur" << std::endl;
                Node* nop = new Node(
                    OP_GREATER, ast_nodes[ref+1], ast_nodes[ref], 0, 0);
                ast_nodes.pop_back();
                ast_nodes.pop_back();
                ast_nodes.push_back(nop);
                nop->display();
                break;
        } 
        default:
                cout << "Erreur, opérateur non reconnu" << std::endl;
                exit(EXIT_FAILURE);
        }

        return 0;
}
Any Visitor::visitPrExpr(ifccParser::PrExprContext* ctx) 
{
        cout << "Call to visit prExpression" << std::endl;
        return visitChildren(ctx);
}
