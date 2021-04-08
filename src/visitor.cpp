#include <cassert>
#include <map>
#include <vector>
#include <string>

#include "visitor.h"
#include "node.h"

std::map<std::string, int > Symboltable::fct_params;

// Improving readability
using namespace antlrcpp;

Any Visitor::visitAxiom(ifccParser::AxiomContext *ctx)
{
        return visitChildren(ctx);
}

Any Visitor::visitProg(ifccParser::ProgContext *ctx)
{
        std::cout << "Call to visitProg" << std::endl;
        return visitChildren(ctx);
}

Any Visitor::visitReturnInstr(ifccParser::ReturnInstrContext *ctx)
{
        // Debug print
        std::cout << "Call to visitReturnInstr" << std::endl;

        // Node definition
        Node* n = new Node(OP_RETURN, NULL, NULL,0, 0);
        curfct->funcInstr.push_back(n);

        // Setting the return expression as a child for
        // our return node
        int index = curfct->funcInstr.size() - 1;
        visitChildren(ctx);
        assert(curfct->funcInstr.size() == index+2);
        curfct->funcInstr[index]->ndlist[0] = curfct->funcInstr[index+1];
        curfct->funcInstr.pop_back();

        // My job here is done
        return 0;
}

Any Visitor::visitNumber(ifccParser::NumberContext *ctx)
{
        // Debug print
        std::cout << "Call to visitNumber" << std::endl;

        // Node definition
        int a = stoi(ctx->INT_CONST()->getText());
        Node* n = new Node(OP_CONST, NULL, NULL, a, 0);
        curfct->funcInstr.push_back(n);

        return visitChildren(ctx);
}

Any Visitor::visitChar(ifccParser::CharContext *ctx)
{
        // Debug print
        std::cout << "Call to visitChar" << std::endl;

        // Node definition
        std::string charConst = ctx->CHAR_CONST()->getText();
        int value = charConst[1];
        Node* n = new Node(OP_CONST, NULL, NULL, value, 0);
        curfct->funcInstr.push_back(n);

        return visitChildren(ctx);
}

Any Visitor::visitVar(ifccParser::VarContext *ctx)
{
        std::cout << "Call to visitVar" << std::endl;

        //Node definition
        std::string var_name = ctx->ID()->getText();
        int var_adr = curfct->symb->getAddress(var_name);
        Node* var = new Node(OP_IDENT, NULL, NULL, var_adr, 0);
        curfct->funcInstr.push_back(var);

        return visitChildren(ctx);
}

Any Visitor::visitStatement(ifccParser::StatementContext *ctx)
{
        // Debug print
        std::cout << "Call to visitStatement" << std::endl;

        return visitChildren(ctx);
}

Any Visitor::visitDeclaration (ifccParser::DeclarationContext *ctx)
{
        // Debug print
        std::cout << "Call to visitDeclaration" << std::endl;
        // TODO: Handle variable type here, or change grammar
        return visitChildren(ctx);
}

Any Visitor::visitInitDeclaratorList(ifccParser::InitDeclaratorListContext *ctx)
{
        // Debug print
        std::cout << "Call to visitInitDeclaratorList" << std::endl;

        return visitChildren(ctx);

}

Any Visitor::visitInitDeclarator(ifccParser::InitDeclaratorContext *ctx)
{
        std::cout << "Call to visitInitDeclarator" << std::endl;

        if(ctx->arithExpr()) {
                std::cout << " >> Declaration and affectaion";
        } else if (ctx->arrayInitialisation()) {
                std::cout << " >> Declaration and affectation (Array)";
        } else {
                std::cout << " >> Declaration w/o affectation";
        }
        std::cout << " of " << ctx->ID()->getText() << std::endl;

        // Let'ts insert our var in the symbol table
        std::string var_name = ctx->ID()->getText();
        curfct->symb->store(var_name, INT); // TODO: Could be CHAR

        if (ctx->arithExpr()) {
                // If assign, we create an IDENT node that's going to
                // be the left child of out ASSIGN node
                int var_adr = curfct->symb->getAddress(var_name);
                Node* var = new Node(OP_IDENT, NULL, NULL, var_adr, 0);

                // Now let's deal with the ASSIGN node
                Node* n = new Node(OP_ASSIGN, var, NULL, 0, 0);
                curfct->funcInstr.push_back(n);

                // Finishing with the expr node
                int index = curfct->funcInstr.size() - 1;
                visitChildren(ctx);
                assert(curfct->funcInstr.size() == index+2);
                curfct->funcInstr[index]->ndlist[1] = curfct->funcInstr[index+1];
                curfct->funcInstr.pop_back();
        } else if (ctx->arrayInitialisation()) {
                // Array size
                int arraySize = stoi(ctx->INT_CONST()->getText());

                // Array values
                auto arrayValues = ctx->arrayInitialisation()->INT_CONST();
                assert(arraySize == arrayValues.size());


                Node* array = new Node(OP_ARRAY, 0, 0);
                array->strarg = var_name;
                // Start at 1 because array[0] has already
                // been added to the symbol table
                for (int i = 0; i < arraySize; ++i) {
                        int value = stoi(arrayValues.at(i)->getText());
                        std::cout << " >> "<<value << std::endl;
                        
                        Node* c = new Node(OP_CONST, NULL, NULL, value, 0);
                        array->addNode(c);
                }
                std::cout << curfct->symb->getOffset() 
                                << std::endl;
                array->display();
                curfct->funcInstr.push_back(array);
        }

        return 0;
}

Any Visitor::visitAssignArithExpr(ifccParser::AssignArithExprContext *ctx)
{
        // Debug print
        std::cout <<"Call to AssignArithExpr " << std::endl;
        std::cout << " >> Assigning " << ctx->ID()->getText()
        << " to " << ctx->arithExpr()->getText() << std::endl;

        // Node definition
        /*
         * n->left is going to be the variable
         * n->right is going to be an expression
        */

        std::string var_name = ctx->ID()->getText();
        int var_adr = curfct->symb->getAddress(var_name);
        Node* o = new Node(OP_IDENT, NULL, NULL, var_adr, 0);

        int ref = curfct->funcInstr.size();
        // Expression
        visitChildren(ctx);
        assert(curfct->funcInstr.size()== ref+1);

        Node* n = new Node(OP_ASSIGN, o, curfct->funcInstr.back(), 0, 0);
        curfct->funcInstr.pop_back();
        curfct->funcInstr.push_back(n);

        return 0;
}
Any Visitor::visitBlockStatement(ifccParser::BlockStatementContext* ctx) {
        return visitChildren(ctx);
}


Any Visitor::visitIfElse(ifccParser::IfElseContext *ctx)
{
        //Debug print
        std::cout << "Call to visit IF " << std::endl;
        
        //conditionNode
        visit(ctx->relationalExpression());
        Node* conditionNode = curfct->funcInstr[curfct->funcInstr.size()-1];
        curfct->funcInstr.pop_back();

        //thenBlockNode
        int startingStackSize = curfct->funcInstr.size();
        visit(ctx -> thenBloc);
        int afterThenSizeStack = curfct->funcInstr.size();

        Node* thenBlocNode = new Node(OP_BLOCK,0, 0);
        for(int i = startingStackSize; i < afterThenSizeStack; ++i){
              thenBlocNode->pushBackToNdList(curfct->funcInstr[i]);
        } 
        for(int i = startingStackSize; i < afterThenSizeStack; ++i){
              curfct->funcInstr.pop_back();
        } 

        //elseBlokNode
        int beforeElseSizeStack = curfct->funcInstr.size();
        visit(ctx -> elseBloc);
        int afterElseSizeStack = curfct->funcInstr.size();

        Node* elseBlocNode = new Node(OP_BLOCK,0, 0);
        for(int j = beforeElseSizeStack ; j < afterElseSizeStack ; ++j){
              elseBlocNode->pushBackToNdList(curfct->funcInstr[j]);
        }
        for(int j = beforeElseSizeStack ; j < afterElseSizeStack ; ++j){
              curfct->funcInstr.pop_back();
        } 

        //ifelseBlock
        Node* ifelseNode = new Node(OP_IFELSE, conditionNode, thenBlocNode, 0, 0);
        ifelseNode->pushBackToNdList(elseBlocNode);

        curfct->funcInstr.push_back(ifelseNode);


        return 0;
}

Any Visitor::visitIf(ifccParser::IfContext *ctx)
{
         //Debug print
        std::cout << "Call to visit IF " << std::endl;
        
        //conditionNode
        visit(ctx->relationalExpression());
        Node* conditionNode = curfct->funcInstr[curfct->funcInstr.size()-1];
        curfct->funcInstr.pop_back();

        //thenBlockNode
        int startingStackSize = curfct->funcInstr.size();
        visit(ctx -> thenBloc);
        int afterThenSizeStack = curfct->funcInstr.size();

        Node* thenBlocNode = new Node(OP_BLOCK,0, 0);
        for(int i = startingStackSize; i < afterThenSizeStack; ++i){
              thenBlocNode->pushBackToNdList(curfct->funcInstr[i]);
        } 
        for(int i = startingStackSize; i < afterThenSizeStack; ++i){
              curfct->funcInstr.pop_back();
        } 

        //ifBlock
        Node* ifNode = new Node(OP_IF, conditionNode, thenBlocNode, 0, 0);

        curfct->funcInstr.push_back(ifNode);

        return 0;
} 
Any Visitor::visitAssignRelExpr(ifccParser::AssignRelExprContext *ctx)
{
        // Debug print
        std::cout <<"Call to AssignRelExpr " << std::endl;
        std::cout << " >> Assigning " << ctx->ID()->getText()
        << " to " << ctx->relationalExpression()->getText() << std::endl;

        // Node definition
        /*
         * n->left is going to be the variable
         * n->right is going to be an expression
        */

        std::string var_name = ctx->ID()->getText();
        int var_adr = curfct->symb->getAddress(var_name);
        Node* o = new Node(OP_IDENT, NULL, NULL, var_adr, 0);

        int ref = curfct->funcInstr.size();
        // Expression
        visitChildren(ctx);
        assert(curfct->funcInstr.size()== ref+1);

        Node* n = new Node(OP_ASSIGN, o, curfct->funcInstr.back(), 0, 0);
        curfct->funcInstr.pop_back();
        curfct->funcInstr.push_back(n);

        return 0;
}

Any Visitor::visitExpr(ifccParser::ExprContext* ctx)
{
        std::cout << "Call to visit Expr" << std::endl;

        char op = ctx->op->getText().at(0);
        int ref = curfct->funcInstr.size();
        visit(ctx->left);
        visit(ctx->right);

        assert(curfct->funcInstr.size() == ref+2);

        switch(op) {
        case '+': {
                std::cout << "Addition" << std::endl;
                Node* nop = new Node(
                    OP_ADD, curfct->funcInstr[ref+1], curfct->funcInstr[ref], 0, 0);
                curfct->funcInstr.pop_back();
                curfct->funcInstr.pop_back();
                curfct->funcInstr.push_back(nop);
                break;
        }
        case '-': {
                std::cout << "Subtraction" << std::endl;
                Node* nop = new Node(
                    OP_SUB, curfct->funcInstr[ref+1], curfct->funcInstr[ref], 0, 0);
                curfct->funcInstr.pop_back();
                curfct->funcInstr.pop_back();
                curfct->funcInstr.push_back(nop);
                break;
        }
        case '*': {
                std::cout << "Multiplication" << std::endl;
                Node* nop = new Node(OP_MUL, curfct->funcInstr[ref+1],
                                     curfct->funcInstr[ref], 0, 0);
                curfct->funcInstr.pop_back();
                curfct->funcInstr.pop_back();
                curfct->funcInstr.push_back(nop);
                break;
        }
        case '/': {
                std::cout << "Division" << std::endl;
                Node* nop = new Node(OP_DIV, curfct->funcInstr[ref+1],
                                     curfct->funcInstr[ref], 0, 0);
                curfct->funcInstr.pop_back();
                curfct->funcInstr.pop_back();
                curfct->funcInstr.push_back(nop);
                break;
        }
        case '&': {
                std::cout << "Bitwise AND" << std::endl;
                Node* nop = new Node(OP_BIT_AND, curfct->funcInstr[ref+1],
                                     curfct->funcInstr[ref], 0, 0);
                curfct->funcInstr.pop_back();
                curfct->funcInstr.pop_back();
                curfct->funcInstr.push_back(nop);
                break;
        }
        case '^': {
                std::cout << "Bitwise XOR" << std::endl;
                Node* nop = new Node(OP_BIT_XOR, curfct->funcInstr[ref+1],
                                     curfct->funcInstr[ref], 0, 0);
                curfct->funcInstr.pop_back();
                curfct->funcInstr.pop_back();
                curfct->funcInstr.push_back(nop);
                break;
        }
        case '|': {
                std::cout << "Bitwise OR" << std::endl;
                Node* nop = new Node(OP_BIT_OR, curfct->funcInstr[ref+1],
                                     curfct->funcInstr[ref], 0, 0);
                curfct->funcInstr.pop_back();
                curfct->funcInstr.pop_back();
                curfct->funcInstr.push_back(nop);
                break;
        }
        default:
                std::cout << "Erreur, opérateur non reconnu" << std::endl;
                exit(EXIT_FAILURE);
        }

        return 0;
}

Any Visitor::visitRelExpr(ifccParser::RelExprContext* ctx)
{
        std::cout << "Call to visit RelExpr" << std::endl;

        string relop = ctx->relOp->getText();
        int ref = curfct->funcInstr.size();
        visit(ctx->left);
        visit(ctx->right);

        assert(curfct->funcInstr.size() == ref+2);

        if (relop == "<") {
                std::cout << "inferieur" << std::endl;
                Node* nop = new Node(
                    OP_LOWER, curfct->funcInstr[ref+1], curfct->funcInstr[ref], 0, 0);
                curfct->funcInstr.pop_back();
                curfct->funcInstr.pop_back();
                curfct->funcInstr.push_back(nop);
        } else if (relop == ">") {
                std::cout << "superieur" << std::endl;
                Node* nop = new Node(
                    OP_GREATER, curfct->funcInstr[ref+1], curfct->funcInstr[ref], 0, 0);
                curfct->funcInstr.pop_back();
                curfct->funcInstr.pop_back();
                curfct->funcInstr.push_back(nop);
        } else if (relop == "==") {
                std::cout << "égale à" << std::endl;
                Node* nop = new Node(
                OP_EQUAL, curfct->funcInstr[ref+1], curfct->funcInstr[ref], 0, 0);
                curfct->funcInstr.pop_back();
                curfct->funcInstr.pop_back();
                curfct->funcInstr.push_back(nop);
                nop->display();
        } else if (relop == "!=") {
                std::cout << "superieur" << std::endl;
                Node* nop = new Node(
                OP_UNEQUAL, curfct->funcInstr[ref+1], curfct->funcInstr[ref], 0, 0);
                curfct->funcInstr.pop_back();
                curfct->funcInstr.pop_back();
                curfct->funcInstr.push_back(nop);
        } else {
                std::cout << "Erreur, opérateur non reconnu" << std::endl;
                exit(EXIT_FAILURE);
        }

        return 0;
}

Any Visitor::visitPrExpr(ifccParser::PrExprContext* ctx)
{
        std::cout << "Call to visit prExpression" << std::endl;
        return visitChildren(ctx);
}

Any Visitor::visitFunction(ifccParser::FunctionContext* ctx)
{
        std::cout << "Call to visitFunction " << std::endl;
        std::cout << ctx->getText() << endl;
        std::string name = ctx->ID()->getText();
        Function * f = new Function(name);
        (this->functions).push_back(f);
        this->curfct = f;
        visitChildren(ctx);
        /*
        std::cout << "Display curfct->funcInstr"<<endl;
        for(int i=0; i < this->curfct->funcInstr.size(); ++i) {
                this->curfct->funcInstr.at(i)->display();
        }
        std::cout <<"End display curfct->funcInstr"<<endl;
        */
        /*
        std::cout << "Display funcInstr"<<endl;
        for(int i=0; i < f->funcInstr.size(); ++i) {
                f->funcInstr.at(i)->display();
        }
        */
        int nb = curfct->symb->getNbParams();
        curfct->symb->fct_params.insert(
                        std::pair<std::string,int>(name,nb));
        //std::cout <<"End display funcInstr"<<endl;
        return 0;
}

Any Visitor::visitParameterlist(ifccParser::ParameterlistContext* ctx) 
{
        std::cout << "Call to visit parameter list" << std::endl;
        auto v = ctx->ID();
        auto vt = ctx->TYPE();
        std::cout << "Number of parameters : " << v.size() << std::endl;
        for (int i = 0; i < v.size(); ++i) {
                
                std::cout << " >> " << vt.at(i)->getText() 
                          << " "<< v.at(i)->getText() << std::endl;
                int type = 0;
                std::string strtype = vt.at(i)->getText();
                if ( strtype  == "int") {
                        type = 0;
                } else if (strtype == "char") {
                        type = 1;
                }         
                curfct->symb->store(v.at(i)->getText(), type, true);
        }
        return 0;
}

Any Visitor::visitAssignFunction(ifccParser::AssignFunctionContext* ctx) 
{
        int ref = curfct->funcInstr.size();
        // Debug print
        std::cout <<"Call to AssignFunction " << std::endl;
        std::cout << " >> Assigning " << ctx->ID(0)->getText()
        << " to function " << ctx->ID(1)->getText() << std::endl;

        // Node definition
        /*
         * n->left is going to be the variable
         * n->right is going to be the function
        */
        
        std::string var_name = ctx->ID(0)->getText();
        int var_adr = curfct->symb->getAddress(var_name);
        
        auto v = ctx->primaryExpression();
        Node* o = new Node(OP_IDENT, NULL, NULL, var_adr, 0);
        Node* f = new Node(OP_CALL, NULL, NULL, v.size(), 0);
        f->strarg = ctx->ID(1)->getText();
        // enleve les 2  NULL
        f->ndlist.pop_back();
        f->ndlist.pop_back();
        

        for (int i = 0; i< v.size(); ++i) {
                visit(ctx->primaryExpression(i));
                f->addNode(curfct->funcInstr.back());
                curfct->funcInstr.pop_back();
        }
        

        std::cout << "decompte " << curfct->funcInstr.size() - ref 
                << std::endl;
        assert(ref == curfct->funcInstr.size()); 
        Node* n = new Node(OP_ASSIGN, o, f, 0, 0);
        curfct->funcInstr.push_back(n);

        /*
        int ref = curfct->funcInstr.size();
        // Function
        // Si on a une varible, la transformer en const
        // Passer la liste de parametres en const
        Node* funcnode = new Node(OP_FUNCTION, NULL, NULL, 0, 0);

        Node* n = new Node(OP_ASSIGN, o, curfct->funcInstr.back(), 0, 0);
        curfct->funcInstr.pop_back();
        curfct->funcInstr.push_back(n);
        */
        return 0;

}

