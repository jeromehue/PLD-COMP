#include <cassert>
#include <map>
#include <vector>
#include <string>

#include "visitor.h"
#include "node.h"

/* Static member of function param */
map<string, int > Symboltable::fct_params;

/*  Improving readability */
using namespace antlrcpp;
using namespace std;


Any Visitor::visitAxiom(ifccParser::AxiomContext *ctx)
{
        return visitChildren(ctx);
}

Any Visitor::visitProg(ifccParser::ProgContext *ctx)
{
        cout << "Call to visitProg" << endl;
        return visitChildren(ctx);
}

Any Visitor::visitReturnInstr(ifccParser::ReturnInstrContext *ctx)
{
        cout << "Call to visitReturnInstr" << endl;

        /* Node definition */
        Node* n = new Node(OP_RETURN, NULL, NULL,0, 0);
        curfct->funcInstr.push_back(n);

        /* 
         * Setting the return expression as a child for
         * our return node
         */
        int index = curfct->funcInstr.size() - 1;
        visitChildren(ctx);
        assert(curfct->funcInstr.size() == index+2);
        curfct->funcInstr[index]->ndlist[0] = curfct->funcInstr[index+1];
        curfct->funcInstr.pop_back();

        return 0;
}

Any Visitor::visitNumber(ifccParser::NumberContext *ctx)
{
        cout << "Call to visitNumber" << endl;

        /* Node definition */
        int a = stoi(ctx->INT_CONST()->getText());
        Node* n = new Node(OP_CONST, NULL, NULL, a, 0);
        curfct->funcInstr.push_back(n);

        return visitChildren(ctx);
}

Any Visitor::visitChar(ifccParser::CharContext *ctx)
{
        cout << "Call to visitChar" << endl;

        /*  Node definition */
        string charConst = ctx->CHAR_CONST()->getText();
        int value = charConst[1];
        Node* n = new Node(OP_CONST, NULL, NULL, value, 0);
        curfct->funcInstr.push_back(n);

        return visitChildren(ctx);
}

Any Visitor::visitVar(ifccParser::VarContext *ctx)
{
        cout << "Call to visitVar" << endl;

        /* Node definition */
        string var_name = ctx->ID()->getText();
        int var_adr = curfct->symb->getAddress(var_name);
        Node* var = new Node(OP_IDENT, NULL, NULL, var_adr, 0);
        curfct->funcInstr.push_back(var);

        return visitChildren(ctx);
}

Any Visitor::visitTab(ifccParser::TabContext *ctx) 
{       
        string var_name = ctx->ID(0)->getText();
        cout << " >> VisitTab: " << var_name << endl;

        Node* n;
        if (ctx->INT_CONST()) {
                int value = stoi(ctx->INT_CONST()->getText());
                n = new Node(OP_CONST, NULL, NULL, value, 0);
        } else {
                string id_name = ctx->ID(1)->getText();
                int var_adr = curfct->symb->getAddress(id_name);
                n = new Node(OP_IDENT, NULL, NULL, var_adr, 0);
        }
        
        Node* arrayElement = new Node(OP_ARRAY_ELEMENT, n, NULL, 0, 0);
        arrayElement->strarg = var_name;
        curfct->funcInstr.push_back(arrayElement);
        
        arrayElement->display();

        return 0;
}



Any Visitor::visitStatement(ifccParser::StatementContext *ctx)
{
        cout << "Call to visitStatement" << endl;

        return visitChildren(ctx);
}



Any Visitor::visitDeclaration (ifccParser::DeclarationContext *ctx)
{
        cout << "Call to visitDeclaration" << endl;
        return visitChildren(ctx);
}



Any Visitor::visitInitDeclaratorList(ifccParser::InitDeclaratorListContext *ctx)
{
        cout << "Call to visitInitDeclaratorList" << endl;
        return visitChildren(ctx);

}



Any Visitor::visitInitDeclarator(ifccParser::InitDeclaratorContext *ctx)
{
        cout << "Call to visitInitDeclarator" << endl;

        if(ctx->arithExpr()) {
                cout << " >> Declaration and affectaion";
        } else if (ctx->arrayInitialisation()) {
                cout << " >> Declaration and affectation (Array)";
        } else {
                cout << " >> Declaration w/o affectation";
        }
        cout << " of " << ctx->ID()->getText() << endl;

        /* Let'ts insert our var in the symbol table */
        string var_name = ctx->ID()->getText();
        if (!ctx->arrayInitialisation()) {
                curfct->symb->store(var_name, INT);
        }

        if (ctx->arithExpr()) {
                int var_adr = curfct->symb->getAddress(var_name);
                Node* var = new Node(OP_IDENT, NULL, NULL, var_adr, 0);

                /* Now let's deal with the ASSIGN node */
                Node* n = new Node(OP_ASSIGN, var, NULL, 0, 0);
                curfct->funcInstr.push_back(n);

                /* Finishing with the expr node */
                int index = curfct->funcInstr.size() - 1;
                visitChildren(ctx);
                assert(curfct->funcInstr.size() == index+2);
                curfct->funcInstr[index]->ndlist[1] = 
                        curfct->funcInstr[index+1];
                curfct->funcInstr.pop_back();
        } else if (ctx->arrayInitialisation()) {

                /* Array size */
                int arraySize = stoi(ctx->INT_CONST()->getText());

                /* Array values */
                auto arrayValues = ctx->arrayInitialisation()->INT_CONST();
                assert(arraySize == arrayValues.size());


                Node* array = new Node(OP_ARRAY, 0, 0);
                array->strarg = var_name;
                /* 
                 * Start at 1 because array[0] has already
                 * been added to the symbol table
                 */ 
                for (int i = 0; i < arraySize; ++i) {
                        int value = stoi(arrayValues.at(i)->getText());
                        cout << " >> "<<value << endl; 
                        Node* c = new Node(OP_CONST, NULL, NULL, value, 0);
                        array->addNode(c);

                        string v_name = "tab" + var_name + 
                                to_string(arraySize - i - 1);
                        curfct->symb->store(v_name, INT); 
                }
                cout << curfct->symb->getOffset() 
                                << endl;
                int base = - curfct->symb->getOffset();
                array->display();
                curfct->funcInstr.push_back(array);
        } else if (ctx->functionCall()) {

                cout << "initializing with a function call" <<
                endl;
                
                int var_adr = curfct->symb->getAddress(var_name);
                Node* var = new Node(OP_IDENT, NULL, NULL, var_adr, 0);


                /* Now let's deal with the ASSIGN node */
                Node* n = new Node(OP_ASSIGN, var, NULL, 0, 0);
                curfct->funcInstr.push_back(n);

                /* Finishing with the expr node */
                int index = curfct->funcInstr.size() -1;
                visitChildren(ctx);
                assert(curfct->funcInstr.size() == index+2);
                curfct->funcInstr[index]->ndlist[1] = 
                        curfct->funcInstr[index+1];
                curfct->funcInstr.pop_back();

                cout << "initializing with a function call - Done" <<
                endl;
        }

        return 0;
}



Any Visitor::visitAssignArithExpr(ifccParser::AssignArithExprContext *ctx)
{
        cout <<"Call to AssignArithExpr " << endl;
        cout << " >> Assigning " << ctx->ID()->getText()
        << " to " << ctx->arithExpr()->getText() << endl;

        /*
         * n->left is going to be the variable
         * n->right is going to be an expression
        */

        string var_name = ctx->ID()->getText();
        int var_adr = curfct->symb->getAddress(var_name);
        Node* o = new Node(OP_IDENT, NULL, NULL, var_adr, 0);

        int ref = curfct->funcInstr.size();
        /*  Expression */
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
        /* Debug print */
        cout << "Call to visit IF " << endl;
        
        /* conditionNode */
        visit(ctx->relationalExpression());
        Node* conditionNode = curfct->funcInstr[curfct->funcInstr.size()-1];
        curfct->funcInstr.pop_back();

        /* thenBlockNode */
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

        /* elseBlokNode */
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

        /* ifelseBlock */
        Node* ifelseNode = new Node(OP_IFELSE, conditionNode, 
                                    thenBlocNode, 0, 0);
        ifelseNode->pushBackToNdList(elseBlocNode);

        curfct->funcInstr.push_back(ifelseNode);


        return 0;
}



Any Visitor::visitIf(ifccParser::IfContext *ctx)
{
        cout << "Call to visit IF " << endl;
        
        /* conditionNode */
        visit(ctx->relationalExpression());
        Node* conditionNode = curfct->funcInstr[curfct->funcInstr.size()-1];
        curfct->funcInstr.pop_back();

        /* thenBlockNode */
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

        /* ifBlock */
        Node* ifNode = new Node(OP_IF, conditionNode, thenBlocNode, 0, 0);

        curfct->funcInstr.push_back(ifNode);

        return 0;
} 

Any Visitor::visitWhileStatement(ifccParser::WhileStatementContext *ctx){

        /* Debug print */
        cout << "Call to visit WHILE " << endl;
        
        /* conditionNode */
        visit(ctx->relationalExpression());
        Node* conditionNode = curfct->funcInstr[curfct->funcInstr.size()-1];
        curfct->funcInstr.pop_back();

        /* whileBlockNode */
        int startingStackSize = curfct->funcInstr.size();
        visit(ctx -> whileBlockStatement);
        int afterWhileSizeStack = curfct->funcInstr.size();

        Node* whileBlockNode = new Node(OP_BLOCK,0, 0);
        for(int i = startingStackSize; i < afterWhileSizeStack; ++i){
              whileBlockNode->pushBackToNdList(curfct->funcInstr[i]);
        } 
        for(int i = startingStackSize; i < afterWhileSizeStack; ++i){
              curfct->funcInstr.pop_back();
        } 

        /* whileBlockNode */
        Node* whileNode = new Node(OP_WHILE, conditionNode, 
                                   whileBlockNode, 0, 0);

        curfct->funcInstr.push_back(whileNode);

        return 0;
} 


Any Visitor::visitAssignRelExpr(ifccParser::AssignRelExprContext *ctx)
{
        /* Debug prints */
        cout <<"Call to AssignRelExpr " << endl;
        cout << " >> Assigning " << ctx->ID()->getText()
        << " to " << ctx->relationalExpression()->getText() << endl;

        /*
         * n->left is going to be the variable
         * n->right is going to be an expression
         */

        string var_name = ctx->ID()->getText();
        int var_adr = curfct->symb->getAddress(var_name);
        Node* o = new Node(OP_IDENT, NULL, NULL, var_adr, 0);

        int ref = curfct->funcInstr.size();
        /* Expression  */
        visitChildren(ctx);
        assert(curfct->funcInstr.size()== ref+1);

        Node* n = new Node(OP_ASSIGN, o, curfct->funcInstr.back(), 0, 0);
        curfct->funcInstr.pop_back();
        curfct->funcInstr.push_back(n);

        return 0;
}

Any Visitor::visitExpr(ifccParser::ExprContext* ctx)
{
        cout << "Call to visit Expr" << endl;

        /* Get operator */
        char op = ctx->op->getText().at(0);
        int ref = curfct->funcInstr.size();
        
        /* Visit children */
        visit(ctx->left);
        visit(ctx->right);

        assert(curfct->funcInstr.size() == ref+2);

        switch(op) {
        case '+': {
                cout << "Addition" << endl;
                Node* nop = new Node(OP_ADD, curfct->funcInstr[ref+1], 
                                     curfct->funcInstr[ref], 0, 0);
                curfct->funcInstr.pop_back();
                curfct->funcInstr.pop_back();
                curfct->funcInstr.push_back(nop);
                break;
        }
        case '-': {
                cout << "Subtraction" << endl;
                Node* nop = new Node(OP_SUB, curfct->funcInstr[ref+1], 
                                     curfct->funcInstr[ref], 0, 0);
                curfct->funcInstr.pop_back();
                curfct->funcInstr.pop_back();
                curfct->funcInstr.push_back(nop);
                break;
        }
        case '*': {
                cout << "Multiplication" << endl;
                Node* nop = new Node(OP_MUL, curfct->funcInstr[ref+1],
                                     curfct->funcInstr[ref], 0, 0);
                curfct->funcInstr.pop_back();
                curfct->funcInstr.pop_back();
                curfct->funcInstr.push_back(nop);
                break;
        }
        case '/': {
                cout << "Division" << endl;
                Node* nop = new Node(OP_DIV, curfct->funcInstr[ref+1],
                                     curfct->funcInstr[ref], 0, 0);
                curfct->funcInstr.pop_back();
                curfct->funcInstr.pop_back();
                curfct->funcInstr.push_back(nop);
                break;
        }
        case '&': {
                cout << "Bitwise AND" << endl;
                Node* nop = new Node(OP_BIT_AND, curfct->funcInstr[ref+1],
                                     curfct->funcInstr[ref], 0, 0);
                curfct->funcInstr.pop_back();
                curfct->funcInstr.pop_back();
                curfct->funcInstr.push_back(nop);
                break;
        }
        case '^': {
                cout << "Bitwise XOR" << endl;
                Node* nop = new Node(OP_BIT_XOR, curfct->funcInstr[ref+1],
                                     curfct->funcInstr[ref], 0, 0);
                curfct->funcInstr.pop_back();
                curfct->funcInstr.pop_back();
                curfct->funcInstr.push_back(nop);
                break;
        }
        case '|': {
                cout << "Bitwise OR" << endl;
                Node* nop = new Node(OP_BIT_OR, curfct->funcInstr[ref+1],
                                     curfct->funcInstr[ref], 0, 0);
                curfct->funcInstr.pop_back();
                curfct->funcInstr.pop_back();
                curfct->funcInstr.push_back(nop);
                break;
        }
        default:
                cout << "Erreur, opérateur non reconnu" << endl;
                exit(EXIT_FAILURE);
        }

        return 0;
}

Any Visitor::visitRelExpr(ifccParser::RelExprContext* ctx)
{
        cout << "Call to visit RelExpr" << endl;

        /* Get operator */
        string relop = ctx->relOp->getText();
        int ref = curfct->funcInstr.size();

        /* Visit Childrens */
        visit(ctx->left);
        visit(ctx->right);

        assert(curfct->funcInstr.size() == ref+2);

        if (relop == "<") {
                /* less than*/
                Node* nop = new Node(OP_LOWER, 
                                     curfct->funcInstr[ref+1], 
                                      curfct->funcInstr[ref], 0, 0);
                curfct->funcInstr.pop_back();
                curfct->funcInstr.pop_back();
                curfct->funcInstr.push_back(nop);

        } else if (relop == ">") {
                /* greater than*/
                Node* nop = new Node(OP_GREATER, 
                                     curfct->funcInstr[ref+1], 
                                     curfct->funcInstr[ref], 0, 0);
                curfct->funcInstr.pop_back();
                curfct->funcInstr.pop_back();
                curfct->funcInstr.push_back(nop);

        } else if (relop == "==") {
                /* equal */
                Node* nop = new Node(OP_EQUAL, 
                                     curfct->funcInstr[ref+1], 
                                     curfct->funcInstr[ref], 0, 0);
                curfct->funcInstr.pop_back();
                curfct->funcInstr.pop_back();
                curfct->funcInstr.push_back(nop);

        } else if (relop == "!=") {
                /* different */
                Node* nop = new Node(OP_UNEQUAL, 
                                     curfct->funcInstr[ref+1], 
                                     curfct->funcInstr[ref], 0, 0);
                curfct->funcInstr.pop_back();
                curfct->funcInstr.pop_back();
                curfct->funcInstr.push_back(nop);

        } else {
                cout << "Erreur, opérateur non reconnu" << endl;
                exit(EXIT_FAILURE);
        }

        return 0;
}

Any Visitor::visitPrExpr(ifccParser::PrExprContext* ctx)
{
        cout << "Call to visit prExpression" << endl;
        return visitChildren(ctx);
}



Any Visitor::visitFunction(ifccParser::FunctionContext* ctx)
{
        /* Debug print */
        cout << "Call to visitFunction " << endl;

        /* Creating function */
        cout << ctx->getText() << endl;
        string name = ctx->ID()->getText();
        Function * f = new Function(name);
        
        /* Setting it as out current function*/
        (this->functions).push_back(f);
        this->curfct = f;

        /* Visiting instructions */
        visitChildren(ctx);

        /* Storing it in static part of symbol table */
        int nb = curfct->symb->getNbParams();
        curfct->symb->fct_params.insert(pair<string,int>(name,nb));
        
        return 0;
}



Any Visitor::visitParameterlist(ifccParser::ParameterlistContext* ctx) 
{
        cout << "Call to visit parameter list" << endl;
        auto v = ctx->ID();
        auto vt = ctx->TYPE();
        cout << "Number of parameters : " << v.size() << endl;
        for (int i = 0; i < v.size(); ++i) {
                
                cout << " >> " << vt.at(i)->getText() 
                          << " "<< v.at(i)->getText() << endl;
                int type = 0;
                string strtype = vt.at(i)->getText();
                if ( strtype  == "int") {
                        type = 0;
                } else if (strtype == "char") {
                        type = 1;
                }         
                curfct->symb->store(v.at(i)->getText(), type, true);
        }
        return 0;
}

Any Visitor::visitFunctionCall(ifccParser::FunctionCallContext* ctx) 
{
        cout << "Call to FunctionCall " << endl;
      
        /* Putchar or Getchar special case*/ 
        if ( ctx->label ) {
                
                cout << ctx->label->getText() << endl;
               
                /*Our function node */
                Node* f = new Node(OP_CALL, NULL, NULL, 1, 0);
                f->ndlist.pop_back();
                f->ndlist.pop_back();
                
                /*Its only parameter*/
                Node* n;

                if (ctx->CHAR_CONST()) {
                        cout << "char const"<< endl;
                string charConst = ctx->CHAR_CONST()->getText();
                        int value = charConst[1];
                        n = new Node(OP_CONST, NULL, NULL, value, 0);
                } else if (ctx->ID()) { 
                        cout << "id " << endl;
                        string var_name = ctx->ID()->getText();
                        int var_adr = curfct->symb->getAddress(var_name);
                        n = new Node(OP_IDENT, NULL, NULL, var_adr, 0);
                } else {
                
                }


                f->strarg = ctx->label->getText();
                f->addNode(n);
                curfct->funcInstr.push_back(f);

                return 0;
        }

        /* Our function parameters */ 
        auto v = ctx->primaryExpression();

        /*Creating function node*/
        Node* f = new Node(OP_CALL, NULL, NULL, v.size(), 0);

        /*Setting up function name*/
        f->strarg = ctx->ID()->getText();
        
        /*Removing constructor defaull NULL nodes*/
        f->ndlist.pop_back();
        f->ndlist.pop_back();
        
        /*Adding parameters node to our function */
        for (int i = 0; i< v.size(); ++i) {
                visit(ctx->primaryExpression(i));
                f->addNode(curfct->funcInstr.back());
                curfct->funcInstr.pop_back();
        }
        
        /*
         * Pushing this function call on top of 
         * our current function insructions
         */
        curfct->funcInstr.push_back(f);

        
        return 0;
}



Any Visitor::visitAssignFunction(ifccParser::AssignFunctionContext* ctx) 
{
        int ref = curfct->funcInstr.size();
        
        /* Debug print */
        cout <<"Call to AssignFunction " << endl;
        cout << " >> Assigning " << ctx->ID(0)->getText()
        << " to function " << ctx->ID(1)->getText() << endl;
       
        /* Fetching var info */ 
        string var_name = ctx->ID(0)->getText();
        int var_adr = curfct->symb->getAddress(var_name);
        
        /* Creating ident and function call nodes */ 
        auto v = ctx->primaryExpression();
        Node* o = new Node(OP_IDENT, NULL, NULL, var_adr, 0);
        Node* f = new Node(OP_CALL, ctx->ID(1)->getText(), v.size());
        
        /* Filling function parameters */
        for (int i = 0; i< v.size(); ++i) {
                visit(ctx->primaryExpression(i));
                f->addNode(curfct->funcInstr.back());
                curfct->funcInstr.pop_back();
        }

        /* Creating assign node and pushing it to our instructions */ 
        assert(ref == curfct->funcInstr.size()); 
        Node* n = new Node(OP_ASSIGN, o, f, 0, 0);
        curfct->funcInstr.push_back(n);
        
        return 0;

}



Any Visitor::visitAssignGetchar(ifccParser::AssignGetcharContext* ctx){
        cout << "Call to visitAssignGetChar" << endl;
        
        /* Fetch var info*/
        string var_name = ctx->ID()->getText();
        int var_adr = curfct->symb->getAddress(var_name);
        
        /* 
         * Create assign node by combining function call and
         * ident node.
         */
        Node* o = new Node(OP_IDENT, NULL, NULL, var_adr, 0);
        Node* f = new Node(OP_CALL, "getchar", 0); 
        Node* n = new Node(OP_ASSIGN, o, f, 0, 0);

        /*Push assign node*/
        curfct->funcInstr.push_back(n);

        return 0;


} 
