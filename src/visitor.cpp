#include <cassert>
#include <map>
#include <vector>
#include <string>

#include "verbose.h"
#include "visitor.h"
#include "node.h"

/* Static member of function param */
map<string, int> Symboltable::fct_params;

/*  Improving readability */
using namespace antlrcpp;
using namespace std;

typedef Visitor Vis;
typedef ifccParser prs;

Any Vis::visitAxiom(prs::AxiomContext *ctx)
{
        verbose("Call to visitAxiom");
        return visitChildren(ctx);
}

Any Vis::visitProg(prs::ProgContext *ctx)
{
        verbose("Call to visitProg");
        return visitChildren(ctx);
}

Any Vis::visitReturnInstr(prs::ReturnInstrContext *ctx)
{
        verbose("Call to visitReturnInstr");

        /* Node definition */
        Node *n = new Node(OP_RETURN, NULL, NULL, 0, 0);
        curfct->funcInstr.push_back(n);

        /*
         * Setting the return expression as a child for
         * our return node
         */
        int index = curfct->funcInstr.size() - 1;
        visitChildren(ctx);
        curfct->funcInstr[index]->ndlist[0] = curfct->funcInstr[index + 1];
        curfct->funcInstr.pop_back();

        return 0;
}

Any Vis::visitNumber(prs::NumberContext *ctx)
{
        verbose("Call to visitNumber");

        /* Node definition */
        int a = stoi(ctx->INT_CONST()->getText());
        Node *n = new Node(OP_CONST, NULL, NULL, a, 0);
        curfct->funcInstr.push_back(n);

        return visitChildren(ctx);
}

Any Vis::visitChar(prs::CharContext *ctx)
{
        verbose("Call to visitChar");

        /*  Node definition */
        string charConst = ctx->CHAR_CONST()->getText();
        int value = charConst[1];
        Node *n = new Node(OP_CONST, NULL, NULL, value, 0);
        curfct->funcInstr.push_back(n);

        return visitChildren(ctx);
}

Any Vis::visitVar(prs::VarContext *ctx)
{
        verbose("Call to visitVar");

        /* Node definition */
        string var_name = ctx->ID()->getText();
        int var_adr = curfct->symb->getAddress(var_name);
        Node *var = new Node(OP_IDENT, NULL, NULL, var_adr, 0);
        curfct->funcInstr.push_back(var);

        return visitChildren(ctx);
}

Any Vis::visitTab(prs::TabContext *ctx)
{
        string var_name = ctx->ID()->getText();
        verbose(" >> VisitTab: " + var_name);

        Node *n;
        /*
        if (ctx->INT_CONST()) {
                int value = stoi(ctx->INT_CONST()->getText());
                n = new Node(OP_CONST, NULL, NULL, value, 0);
        } else {
                string id_name = ctx->ID(1)->getText();
                int var_adr = curfct->symb->getAddress(id_name);
                n = new Node(OP_IDENT, NULL, NULL, var_adr, 0);
        }*/
        visit(ctx->arithExpr());

        n = curfct->funcInstr.back();
        curfct->funcInstr.pop_back();

        //n->display();

        Node *arrayElement = new Node(OP_ARRAY_ELEMENT, n, NULL, 0, 0);
        arrayElement->strarg = var_name;
        curfct->funcInstr.push_back(arrayElement);

        return 0;
}

Any Vis::visitStatement(prs::StatementContext *ctx)
{
        verbose("Call to visitStatement");
        return visitChildren(ctx);
}

Any Vis::visitDeclaration(prs::DeclarationContext *ctx)
{
        verbose("Call to visitDeclaration");
        return visitChildren(ctx);
}

Any Vis::visitInitDeclaratorList(prs::InitDeclaratorListContext *ctx)
{
        verbose("Call to visitInitDeclaratorList");
        return visitChildren(ctx);
}

Any Vis::visitInitDeclarator(prs::InitDeclaratorContext *ctx)
{
        verbose("Call to visitInitDeclarator");

        if (ctx->arithExpr()) {
                verbose(" >> Declaration and affectaion");

        } else if (ctx->arrayInitialisation()) {
                verbose(" >> Declaration and affectation (Array)");

        } else if (ctx->assignmentExpr()) {
                verbose(" >> chained assignment");

        } else {
                verbose(" >> Declaration w/o affectation");
        }

        verbose(" >> of " + ctx->ID()->getText());

        /* Let'ts insert our var in the symbol table */
        string var_name = ctx->ID()->getText();

        if (!ctx->arrayInitialisation()) {
                curfct->symb->store(var_name, INT);
        }

        if (ctx->arithExpr()) {
                int var_adr = curfct->symb->getAddress(var_name);
                Node *var = new Node(OP_IDENT, NULL, NULL, var_adr, 0);

                /* Now let's deal with the ASSIGN node */
                Node *n = new Node(OP_ASSIGN, var, NULL, 0, 0);
                curfct->funcInstr.push_back(n);

                /* Finishing with the expr node */
                int index = curfct->funcInstr.size() - 1;
                visitChildren(ctx);
                assert(curfct->funcInstr.size() == index + 2);

                curfct->funcInstr[index]->ndlist[1] =
                        curfct->funcInstr[index + 1];
                curfct->funcInstr.pop_back();

        } else if (ctx->assignmentExpr()) {
                int var_adr = curfct->symb->getAddress(var_name);
                Node *var = new Node(OP_IDENT, NULL, NULL, var_adr, 0);

                /* Now let's deal with the ASSIGN node */
                Node *n = new Node(OP_ASSIGN, var, NULL, 0, 0);
                curfct->funcInstr.push_back(n);

                /* Finishing with the expr node */
                int index = curfct->funcInstr.size() - 1;

                /* assignmentExpr*/
                visitChildren(ctx);
                assert(curfct->funcInstr.size() == index + 2);
                curfct->funcInstr[index]->ndlist[1] =
                        curfct->funcInstr[index + 1];
                curfct->funcInstr.pop_back();

        } else if (ctx->arrayInitialisation()) {
                
                /* Array size */
                int arraySize;
                if (ctx->INT_CONST()) {
                        arraySize = stoi(ctx->INT_CONST()->getText());
                } else {
                        arraySize = -1;
                }
                int type =
                        ctx->arrayInitialisation()->INT_CONST().size() > 0
                        ? INT : CHAR;

                /* Array values */
                if (type == INT) {
                        auto arrayValues =
                                ctx->arrayInitialisation()->INT_CONST();
                        
                        if (arraySize == -1) {
                                arraySize = arrayValues.size(); 
                        }

                        if (arraySize != arrayValues.size()) {
                                cerr << "Error, array " << var_name
                                     << " was declared with a size of "
                                     << arraySize << " but found "
                                     << arrayValues.size() << " elements."
                                     << endl;
                                exit(EXIT_FAILURE);
                        }

                        Node *array = new Node(OP_ARRAY, 0, 0);
                        array->strarg = var_name;

                        for (int i = 0; i < arraySize; ++i) {
                                int value =
                                        stoi(arrayValues.at(i)->getText());
                                verbose(" >> " + to_string(value));
                                Node *c = new Node(OP_CONST, NULL,
                                                   NULL, value, 0);
                                array->addNode(c);

                                string v_name = "tab" + var_name +
                                                to_string(arraySize -i-1);

                                curfct->symb->store(v_name, INT);
                        }

                        verbose(to_string(curfct->symb->getOffset()));
                        int base = -curfct->symb->getOffset();
                        curfct->funcInstr.push_back(array);

                } else if (type == CHAR) {
                        auto arrayValues =
                                ctx->arrayInitialisation()->CHAR_CONST();

                        if (arraySize == -1) {
                                arraySize = arrayValues.size(); 
                        }

                        if (arraySize != arrayValues.size()) {
                                cerr << "Error, array " << var_name
                                     << " was declared with a size of "
                                     << arraySize << " but found "
                                     << arrayValues.size() << " elements."
                                     << endl;
                                exit(EXIT_FAILURE);
                        }

                        Node *array = new Node(OP_ARRAY, 0, 0);
                        array->strarg = var_name;

                        for (int i = 0; i < arraySize; ++i) {
                                string charConst =
                                        arrayValues.at(i)->getText();
                                int charValue = charConst[1];
                                verbose(" >> " + to_string(charValue));
                                Node *c = new Node(OP_CONST, NULL,
                                                   NULL, charValue, 0);
                                array->addNode(c);

                                string v_name = "tab" + var_name +
                                                to_string(arraySize -i-1);
                                curfct->symb->store(v_name, CHAR);
                        }

                        verbose(to_string(curfct->symb->getOffset()));
                        int base = -curfct->symb->getOffset();
                        curfct->funcInstr.push_back(array);
                }

        } else if (ctx->functionCall()) {

                verbose("initializing with a function call");

                int var_adr = curfct->symb->getAddress(var_name);
                Node *var = new Node(OP_IDENT, NULL, NULL, var_adr, 0);

                /* Now let's deal with the ASSIGN node */
                Node *n = new Node(OP_ASSIGN, var, NULL, 0, 0);
                curfct->funcInstr.push_back(n);

                /* Finishing with the expr node */
                int index = curfct->funcInstr.size() - 1;
                visitChildren(ctx);
                assert(curfct->funcInstr.size() == index + 2);
                curfct->funcInstr[index]->ndlist[1] =
                        curfct->funcInstr[index + 1];
                curfct->funcInstr.pop_back();

                verbose("initializing with a function call - Done");
        }

        return 0;
}

Any Vis::visitAssignAssign(prs::AssignAssignContext *ctx)
{
        verbose("Call to AssignAssign ");

        int ref = curfct->funcInstr.size();

        string var_name = ctx->ID()->getText();
        int var_adr = curfct->symb->getAddress(var_name);
        Node *o = new Node(OP_IDENT, NULL, NULL, var_adr, 0);

        visitChildren(ctx);

        int addedAssign = curfct->funcInstr.size() - ref;
        verbose("added assign (1) :" + to_string(addedAssign));

        Node *n = new Node(OP_ASSIGN, o, curfct->funcInstr.back(), 0, 0);
        curfct->funcInstr.pop_back();
        curfct->funcInstr.push_back(n);

        return 0;
}

Any Vis::visitAssignArithExpr(prs::AssignArithExprContext *ctx)
{
        verbose("Call to AssignArithExpr ");
        verbose(" >> Assigning " + ctx->ID()->getText()
                + " to " + ctx->arithExpr()->getText());

        /*
         * n->left is going to be the variable
         * n->right is going to be an expression
        */

        string var_name = ctx->ID()->getText();
        int var_adr = curfct->symb->getAddress(var_name);
        Node *o = new Node(OP_IDENT, NULL, NULL, var_adr, 0);

        int ref = curfct->funcInstr.size();
        /*  Expression */
        visitChildren(ctx);
        assert(curfct->funcInstr.size() == ref + 1);

        Node *n = new Node(OP_ASSIGN, o, curfct->funcInstr.back(), 0, 0);
        curfct->funcInstr.pop_back();
        curfct->funcInstr.push_back(n);

        return 0;
}

Any Vis::visitBlockStatement(prs::BlockStatementContext *ctx)
{
        return visitChildren(ctx);
}

Any Vis::visitIfElse(prs::IfElseContext *ctx)
{
        /* Debug print */
        verbose("Call to visitIfElse ");

        /* conditionNode */
        visit(ctx->relationalExpression());
        Node *conditionNode =
                curfct->funcInstr[curfct->funcInstr.size() - 1];
        curfct->funcInstr.pop_back();

        /* thenBlockNode */
        int startingStackSize = curfct->funcInstr.size();
        visit(ctx->thenBloc);
        int afterThenSizeStack = curfct->funcInstr.size();

        Node *thenBlocNode = new Node(OP_BLOCK, 0, 0);

        for (int i = startingStackSize; i < afterThenSizeStack; ++i) {
                thenBlocNode->pushBackToNdList(curfct->funcInstr[i]);
        }

        for (int i = startingStackSize; i < afterThenSizeStack; ++i) {
                curfct->funcInstr.pop_back();
        }

        /* elseBlokNode */
        int beforeElseSizeStack = curfct->funcInstr.size();
        visit(ctx->elseBloc);
        int afterElseSizeStack = curfct->funcInstr.size();

        Node *elseBlocNode = new Node(OP_BLOCK, 0, 0);

        for (int j = beforeElseSizeStack; j < afterElseSizeStack; ++j) {
                elseBlocNode->pushBackToNdList(curfct->funcInstr[j]);
        }

        for (int j = beforeElseSizeStack; j < afterElseSizeStack; ++j) {
                curfct->funcInstr.pop_back();
        }

        /* ifelseBlock */
        Node *ifelseNode = new Node(OP_IFELSE, conditionNode,
                                    thenBlocNode, 0, 0);
        ifelseNode->pushBackToNdList(elseBlocNode);

        curfct->funcInstr.push_back(ifelseNode);

        return 0;
}

Any Vis::visitIf(prs::IfContext *ctx)
{
        verbose("Call to visitIf ");

        /* conditionNode */
        visit(ctx->relationalExpression());
        Node *conditionNode =
                curfct->funcInstr[curfct->funcInstr.size() - 1];
        curfct->funcInstr.pop_back();

        /* thenBlockNode */
        int startingStackSize = curfct->funcInstr.size();
        visit(ctx->thenBloc);
        int afterThenSizeStack = curfct->funcInstr.size();

        Node *thenBlocNode = new Node(OP_BLOCK, 0, 0);

        for (int i = startingStackSize; i < afterThenSizeStack; ++i) {
                thenBlocNode->pushBackToNdList(curfct->funcInstr[i]);
        }

        for (int i = startingStackSize; i < afterThenSizeStack; ++i) {
                curfct->funcInstr.pop_back();
        }

        /* ifBlock */
        Node *ifNode = new Node(OP_IF, conditionNode, thenBlocNode, 0, 0);

        curfct->funcInstr.push_back(ifNode);

        return 0;
}

Any Vis::visitWhileStatement(prs::WhileStatementContext *ctx)
{
        /* Debug print */
        verbose("Call to visitWhile ");

        /* conditionNode */
        visit(ctx->relationalExpression());
        Node *conditionNode =
                curfct->funcInstr[curfct->funcInstr.size() - 1];
        curfct->funcInstr.pop_back();

        /* whileBlockNode */
        int startingStackSize = curfct->funcInstr.size();
        visit(ctx->whileBlockStatement);
        int afterWhileSizeStack = curfct->funcInstr.size();

        Node *whileBlockNode = new Node(OP_BLOCK, 0, 0);

        for (int i = startingStackSize; i < afterWhileSizeStack; ++i) {
                whileBlockNode->pushBackToNdList(curfct->funcInstr[i]);
        }

        for (int i = startingStackSize; i < afterWhileSizeStack; ++i) {
                curfct->funcInstr.pop_back();
        }

        /* whileBlockNode */
        Node *whileNode = new Node(OP_WHILE, conditionNode,
                                   whileBlockNode, 0, 0);

        curfct->funcInstr.push_back(whileNode);

        return 0;
}

Any Vis::visitAssignRelExpr(prs::AssignRelExprContext *ctx)
{
        /* Debug prints */
        verbose("Call to AssignRelExpr ");
        verbose(" >> Assigning " + ctx->ID()->getText()
                + " to " + ctx->relationalExpression()->getText());

        /*
         * n->left is going to be the variable
         * n->right is going to be an expression
         */

        string var_name = ctx->ID()->getText();
        int var_adr = curfct->symb->getAddress(var_name);
        Node *o = new Node(OP_IDENT, NULL, NULL, var_adr, 0);

        int ref = curfct->funcInstr.size();
        /* Expression  */
        visitChildren(ctx);
        assert(curfct->funcInstr.size() == ref + 1);

        Node *n = new Node(OP_ASSIGN, o, curfct->funcInstr.back(), 0, 0);
        curfct->funcInstr.pop_back();
        curfct->funcInstr.push_back(n);

        return 0;
}

Any Vis::visitExpr(prs::ExprContext *ctx)
{
        verbose("Call to visitExpr");

        /* Get operator */
        char op = ctx->op->getText().at(0);
        int ref = curfct->funcInstr.size();

        /* Visit children */
        visit(ctx->left);
        visit(ctx->right);

        assert(curfct->funcInstr.size() == ref + 2);

        switch (op) {
        case '+': {
                verbose("Addition");
                Node *nop = new Node(OP_ADD, curfct->funcInstr[ref + 1],
                                     curfct->funcInstr[ref], 0, 0);
                curfct->funcInstr.pop_back();
                curfct->funcInstr.pop_back();
                curfct->funcInstr.push_back(nop);
                break;
        }

        case '-': {
                verbose("Subtraction");
                Node *nop = new Node(OP_SUB, curfct->funcInstr[ref + 1],
                                     curfct->funcInstr[ref], 0, 0);
                curfct->funcInstr.pop_back();
                curfct->funcInstr.pop_back();
                curfct->funcInstr.push_back(nop);
                break;
        }

        case '*': {
                verbose("Multiplication");
                Node *nop = new Node(OP_MUL, curfct->funcInstr[ref + 1],
                                     curfct->funcInstr[ref], 0, 0);
                curfct->funcInstr.pop_back();
                curfct->funcInstr.pop_back();
                curfct->funcInstr.push_back(nop);
                break;
        }

        case '/': {
                verbose("Division");
                Node *nop = new Node(OP_DIV, curfct->funcInstr[ref + 1],
                                     curfct->funcInstr[ref], 0, 0);
                curfct->funcInstr.pop_back();
                curfct->funcInstr.pop_back();
                curfct->funcInstr.push_back(nop);
                break;
        }

        case '&': {
                verbose("Bitwise AND");
                Node *nop = new Node(OP_BIT_AND, curfct->funcInstr[ref + 1],
                                     curfct->funcInstr[ref], 0, 0);
                curfct->funcInstr.pop_back();
                curfct->funcInstr.pop_back();
                curfct->funcInstr.push_back(nop);
                break;
        }

        case '^': {
                verbose("Bitwise XOR");
                Node *nop = new Node(OP_BIT_XOR, curfct->funcInstr[ref + 1],
                                     curfct->funcInstr[ref], 0, 0);
                curfct->funcInstr.pop_back();
                curfct->funcInstr.pop_back();
                curfct->funcInstr.push_back(nop);
                break;
        }

        case '|': {
                verbose("Bitwise OR");
                Node *nop = new Node(OP_BIT_OR, curfct->funcInstr[ref + 1],
                                     curfct->funcInstr[ref], 0, 0);
                curfct->funcInstr.pop_back();
                curfct->funcInstr.pop_back();
                curfct->funcInstr.push_back(nop);
                break;
        }

        default:
                verbose("Erreur, opérateur non reconnu");
                exit(EXIT_FAILURE);
        }

        return 0;
}

Any Vis::visitRelExpr(prs::RelExprContext *ctx)
{
        verbose("Call to visitRelExpr");

        /* Get operator */
        string relop = ctx->relOp->getText();
        int ref = curfct->funcInstr.size();

        /* Visit Childrens */
        visit(ctx->left);
        visit(ctx->right);

        assert(curfct->funcInstr.size() == ref + 2);

        if (relop == "<") {
                /* less than*/
                Node *nop = new Node(OP_LOWER,
                                     curfct->funcInstr[ref + 1],
                                     curfct->funcInstr[ref], 0, 0);
                curfct->funcInstr.pop_back();
                curfct->funcInstr.pop_back();
                curfct->funcInstr.push_back(nop);

        } else if (relop == ">") {
                /* greater than*/
                Node *nop = new Node(OP_GREATER,
                                     curfct->funcInstr[ref + 1],
                                     curfct->funcInstr[ref], 0, 0);
                curfct->funcInstr.pop_back();
                curfct->funcInstr.pop_back();
                curfct->funcInstr.push_back(nop);

        } else if (relop == "==") {
                /* equal */
                Node *nop = new Node(OP_EQUAL,
                                     curfct->funcInstr[ref + 1],
                                     curfct->funcInstr[ref], 0, 0);
                curfct->funcInstr.pop_back();
                curfct->funcInstr.pop_back();
                curfct->funcInstr.push_back(nop);

        } else if (relop == "!=") {
                /* different */
                Node *nop = new Node(OP_UNEQUAL,
                                     curfct->funcInstr[ref + 1],
                                     curfct->funcInstr[ref], 0, 0);
                curfct->funcInstr.pop_back();
                curfct->funcInstr.pop_back();
                curfct->funcInstr.push_back(nop);

        } else {
                verbose("Error: Unknown operator");
                exit(EXIT_FAILURE);
        }

        return 0;
}

Any Vis::visitPrExpr(prs::PrExprContext *ctx)
{
        verbose("Call to visitPrExpr");
        return visitChildren(ctx);
}

Any Vis::visitFunction(prs::FunctionContext *ctx)
{
        /* Debug print */
        verbose("Call to visitFunction ");

        /* Creating function */
        verbose(ctx->getText());
        string name = ctx->ID()->getText();
        Function *f = new Function(name);

        /* Setting it as out current function*/
        (this->functions).push_back(f);
        this->curfct = f;

        /* Visiting instructions */
        visitChildren(ctx);

        /* Storing it in static part of symbol table */
        int nb = curfct->symb->getNbParams();
        curfct->symb->fct_params.insert(pair<string, int>(name, nb));

        return 0;
}

Any Vis::visitParameterlist(prs::ParameterlistContext *ctx)
{
        verbose("Call to visitParameterList");
        auto v = ctx->ID();
        auto vt = ctx->TYPE();
        verbose("Number of parameters : " + to_string(v.size()));

        for (int i = 0; i < v.size(); ++i) {

                verbose(" >> " + vt.at(i)->getText()
                        + " " + v.at(i)->getText());
                int type = 0;
                string strtype = vt.at(i)->getText();

                if (strtype == "int") {
                        type = 0;

                } else if (strtype == "char") {
                        type = 1;
                }

                curfct->symb->store(v.at(i)->getText(), type, true);
        }

        return 0;
}

Any Vis::visitFunctionCall(prs::FunctionCallContext *ctx)
{
        verbose("Call to FunctionCall ");

        /* Putchar or Getchar special case*/
        if (ctx->label) {

                verbose(ctx->label->getText());

                /*Our function node */
                Node *f = new Node(OP_CALL, NULL, NULL, 1, 0);
                f->ndlist.pop_back();
                f->ndlist.pop_back();

                /*Its only parameter*/
                Node *n;

                if (ctx->CHAR_CONST()) {
                        verbose("char const");
                        string charConst = ctx->CHAR_CONST()->getText();
                        int value = charConst[1];
                        n = new Node(OP_CONST, NULL, NULL, value, 0);

                } else if (ctx->ID()) {
                        verbose("id ");
                        string var_name = ctx->ID()->getText();
                        int var_adr = curfct->symb->getAddress(var_name);
                        n = new Node(OP_IDENT, NULL, NULL, var_adr, 0);
                }

                f->strarg = ctx->label->getText();
                f->addNode(n);
                curfct->funcInstr.push_back(f);

                return 0;
        }

        /* Our function parameters */
        auto v = ctx->primaryExpression();

        /*Creating function node*/
        Node *f = new Node(OP_CALL, NULL, NULL, v.size(), 0);

        /*Setting up function name*/
        f->strarg = ctx->ID()->getText();

        /*Removing constructor defaull NULL nodes*/
        f->ndlist.pop_back();
        f->ndlist.pop_back();

        /*Adding parameters node to our function */
        for (int i = 0; i < v.size(); ++i) {
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

Any Vis::visitAssignFunction(prs::AssignFunctionContext *ctx)
{
        int ref = curfct->funcInstr.size();

        /* Debug print */
        verbose("Call to AssignFunction ");
        verbose(" >> Assigning " + ctx->ID(0)->getText()
                + " to function " + ctx->ID(1)->getText());

        /* Fetching var info */
        string var_name = ctx->ID(0)->getText();
        int var_adr = curfct->symb->getAddress(var_name);

        /* Creating ident and function call nodes */
        auto v = ctx->primaryExpression();
        Node *o = new Node(OP_IDENT, NULL, NULL, var_adr, 0);
        Node *f = new Node(OP_CALL, ctx->ID(1)->getText(), v.size());

        /* Filling function parameters */
        for (int i = 0; i < v.size(); ++i) {
                visit(ctx->primaryExpression(i));
                f->addNode(curfct->funcInstr.back());
                curfct->funcInstr.pop_back();
        }

        /* Creating assign node and pushing it to our instructions */
        assert(ref == curfct->funcInstr.size());
        Node *n = new Node(OP_ASSIGN, o, f, 0, 0);
        curfct->funcInstr.push_back(n);

        return 0;
}

Any Vis::visitAssignTabArithExpr(prs::AssignTabArithExprContext *ctx)
{
        verbose("Call to AssignTabArithExpr");

        string var_name = ctx->ID()->getText();

        visit(ctx->arithExpr(0));
        Node *arrayElement = new Node(OP_ARRAY_ELEMENT,
                                      curfct->funcInstr.back(),
                                      NULL, 0, 0);
        arrayElement->strarg = var_name;
        curfct->funcInstr.pop_back();

        visit(ctx->arithExpr(1));
        Node *n = new Node(OP_ASSIGN, arrayElement,
                           curfct->funcInstr.back(), 0, 0);
        curfct->funcInstr.pop_back();
        curfct->funcInstr.push_back(n);

        return 0;
}

Any Vis::visitAssignGetchar(prs::AssignGetcharContext *ctx)
{
        verbose("Call to visitAssignGetChar");

        /* Fetch var info*/
        string var_name = ctx->ID()->getText();
        int var_adr = curfct->symb->getAddress(var_name);

        /*
         * Create assign node by combining function call and
         * ident node.
         */
        Node *o = new Node(OP_IDENT, NULL, NULL, var_adr, 0);
        Node *f = new Node(OP_CALL, "getchar", 0);
        Node *n = new Node(OP_ASSIGN, o, f, 0, 0);

        /*Push assign node*/
        curfct->funcInstr.push_back(n);

        return 0;
}
