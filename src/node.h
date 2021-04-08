/************************************************************************

                             -------------------
    début                : 2021-02-27
    copyright            : (C) 2021 par H4244
*************************************************************************/

#ifndef NODE_H
#define NODE_H

#include <iostream>
#include <sstream>
#include <string>
#include "ir.h"
#include "symb.h"

using namespace std;

/* Les différents types de noeuds */
enum nodeOp {
        OP_ADD,
        OP_SUB,
        OP_MUL,
        OP_ASSIGN,
        OP_IDENT,
        OP_CONST,
        OP_RETURN,
        OP_BLOCK,
        OP_FUNCTION,
        OP_LOWER,
        OP_GREATER,
        OP_EQUAL,
        OP_UNEQUAL,
        OP_IF
};
static int counter=0;
class Node {
public:
        Node(int op, Node *left, Node *right, int arg0, int arg1)
        {
                this->op = op;
                this->ndlist.push_back(left);
                this->ndlist.push_back(right);
                this->args[0] = arg0;
                this->args[1] = arg1;
        }
        void pushBackToNdList(Node * node) 
        {
                this->ndlist.push_back(node);
        }
        void display()
        {
                switch (op) {
                case OP_ADD:
                        std::cout << "OP_ADD" << std::endl;
                        std::cout << "\tleft : ";
                        ndlist[0]->display();
                        std::cout << "\tright : ";
                        ndlist[1]->display();
                        std::cout << std::endl;
                        break;

                case OP_CONST:
                        std::cout << "OP_CONST" << std::endl;
                        break;

                case OP_IDENT:
                        std::cout << "OP_IDENT" << std::endl;
                        break;

                case OP_ASSIGN:
                        std::cout << "OP_ASSIGN | " << std::endl;
                        std::cout << "\tleft : ";
                        ndlist[0]->display();
                        std::cout << "\tright : ";
                        ndlist[1]->display();
                        std::cout << std::endl;
                        break;

                case OP_LOWER:
                        std::cout << "OP_LOWER | " << std::endl;
                        std::cout << "\tleft : ";
                        ndlist[0]->display();
                        std::cout << "\tright : ";
                        ndlist[1]->display(); 
                        std::cout << std::endl;
                        break;

                case OP_GREATER:
                        std::cout << "OP_GREATER | " << std::endl;
                        std::cout << "\tleft : ";
                        ndlist[0]->display();
                        std::cout << "\tright : ";
                        ndlist[1]->display();
                        std::cout << std::endl;
                        break;

                case OP_EQUAL:
                        std::cout << "OP_EQUAL | " << std::endl;
                        std::cout << "\tleft : ";
                        ndlist[0]->display();
                        std::cout << "\tright : ";
                        ndlist[1]->display();
                        std::cout << std::endl;
                        break;

                case OP_UNEQUAL:
                        std::cout << "OP_UNEQUAL | " << std::endl;
                        std::cout << "\tleft : ";
                        ndlist[0]->display();
                        std::cout << "\tright : ";
                        ndlist[1]->display();
                        std::cout << std::endl;
                        break;

                case OP_IF:
                        std::cout << "OP_IF | " << std::endl;
                        std::cout << "\texpresionn : ";
                        ndlist[0]->display();
                        std::cout << "\tthenbloc: ";
                        ndlist[1]->display();
                        std::cout << "\telsebloc: ";
                        ndlist[2]->display();
                        std::cout << std::endl;
                        break;
                case OP_RETURN:
                        std::cout << "OP_RETURN" << std::endl;
                        break;
                        /*
                         * In a non void case, we first generate the code
                         * for the expression, and copy the result in the
                         * special variable !retval. The translation
                         * IR->ASM will generate the code that places
                         * this walue where the ABI tells it should go.
                         * */

                default:
                        std::cout << "Unknown Node" << std::endl;
                        break;
                }
        }

        /*
         * The code generation is a recursive walk of the AST of
         * the function body. Each node of the AST has a method
         * buildIR(CFG* cfg)
         */
        std::string buildIR(CFG *cfg)
        {
                // Debug print
                std::cout << "Generating IR for : ";
                this->display();

                // Avoid jumps that bypasses var init.
                std::string sright, sleft;
                std::string var1, var2, var3;
                std::vector<std::string> retvector;

                switch (op) {
                case OP_ADD:
                        /* Fetching data */
                        var1 = ndlist[0]->buildIR(cfg);
                        var2 = ndlist[1]->buildIR(cfg);
                        var3 = cfg->create_new_tempvar(INT);
                        retvector.push_back(var3);
                        retvector.push_back(var1);
                        retvector.push_back(var2);
                        /* Actual instruction */
                        cfg->current_bb->add_IRInstr(IRInstr::add, INT, retvector);
                        return var3;
                        break;

                case OP_SUB:
                        //TODO Factoriser '+'/'-'
                        // Fetching data
                        var1 = ndlist[0]->buildIR(cfg);
                        var2 = ndlist[1]->buildIR(cfg);
                        var3 = cfg->create_new_tempvar(INT);
                        retvector.push_back(var3);
                        retvector.push_back(var1);
                        retvector.push_back(var2);
                        // Actual instruction
                        cfg->current_bb->add_IRInstr(IRInstr::sub, INT, retvector);
                        return var3;
                        break;

                case OP_MUL:
                        var1 = ndlist[0]->buildIR(cfg);
                        var2 = ndlist[1]->buildIR(cfg);
                        var3 = cfg->create_new_tempvar(INT);
                        retvector.push_back(var3);
                        retvector.push_back(var1);
                        retvector.push_back(var2);
                        // Actual instruction
                        cfg->current_bb->add_IRInstr(IRInstr::mul, INT, retvector);
                        return var3;
                        break;

                case OP_LOWER: //<
                        /* Fetching data */
                        var1 = ndlist[0]->buildIR(cfg);
                        var2 = ndlist[1]->buildIR(cfg);
                        var3 = cfg->create_new_tempvar(INT);
                        retvector.push_back(var3);
                        retvector.push_back(var1);
                        retvector.push_back(var2);
                        /* Actual instruction */
                        cfg->current_bb->add_IRInstr(IRInstr::lower, INT, retvector);
                        return var3;
                        break;

                case OP_GREATER:
                        /* Fetching data */
                        var1 = ndlist[0]->buildIR(cfg);
                        var2 = ndlist[1]->buildIR(cfg);
                        var3 = cfg->create_new_tempvar(INT);
                        retvector.push_back(var3);
                        retvector.push_back(var1);
                        retvector.push_back(var2);
                        /* Actual instruction */
                        cfg->current_bb->add_IRInstr(IRInstr::greater, INT, retvector);
                        return var3;
                        break;

                case OP_EQUAL:
                        /* Fetching data */
                        var1 = ndlist[0]->buildIR(cfg);
                        var2 = ndlist[1]->buildIR(cfg);
                        var3 = cfg->create_new_tempvar(INT);
                        retvector.push_back(var3);
                        retvector.push_back(var1);
                        retvector.push_back(var2);
                        /* Actual instruction */
                        cfg->current_bb->add_IRInstr(IRInstr::cmp_eq, INT, retvector);
                        return var3;
                        break;

                case OP_UNEQUAL:
                        /* Fetching data */
                        var1 = ndlist[0]->buildIR(cfg);
                        var2 = ndlist[1]->buildIR(cfg);
                        var3 = cfg->create_new_tempvar(INT);
                        retvector.push_back(var3);
                        retvector.push_back(var1);
                        retvector.push_back(var2);
                        /* Actual instruction */
                        cfg->current_bb->add_IRInstr(IRInstr::cmp_uneq, INT, retvector);
                        return var3;
                        break;

                case OP_CONST:
                        var3 = cfg->create_new_tempvar(INT);
                        retvector.push_back(var3);
                        // Pas le choix, la constante est cast en string
                        retvector.push_back(to_string(args[0]));
                        cfg->current_bb->add_IRInstr(IRInstr::ldconst,
                                                     INT, retvector);
                        return var3;
                        break;

                case OP_IDENT:
                        std::cout << "Generating IR for var @" << args[0] << " : "
                                  << cfg->symbols->getName(args[0])
                                  << std::endl;
                        return cfg->symbols->getName(args[0]);

                case OP_ASSIGN:
                        sleft = ndlist[0]->buildIR(cfg);  // adresse variable
                        sright = ndlist[1]->buildIR(cfg); // adresse résultat expression
                        retvector.push_back(sleft);
                        retvector.push_back(sright);
                        std::cout << "-------------> Assign node building ir " << sleft << std::endl;
                        cfg->current_bb->add_IRInstr(
                            IRInstr::wmem, INT, retvector);
                        return sright;
                        break;
                case OP_IF:
                       { 
                        counter++;
                        std::cout << "---------------START OP_IF \n ";

                        //test
                        BasicBlock * testBB = cfg->current_bb;
                        var1 = ndlist[0]->buildIR(cfg); // ir de l'expression
                        std::stringstream testBBAdressTostring;       
                        testBBAdressTostring << &testBB; 
                        std::cout << "---------------end TEST  \n ";

                        //then
                        string elselabel="elseBB" + to_string(counter);//testBBAdressTostring.str();
                        string afterlabel="afterBB"+to_string(counter);//testBBAdressTostring.str();
                        retvector.push_back(var1);
                        retvector.push_back(elselabel);
                        retvector.push_back(afterlabel);
                        BasicBlock * thenBB = new BasicBlock(cfg, "thenBB" + testBBAdressTostring.str());
                        cfg->add_bb(thenBB);
                        cfg->current_bb = thenBB;
                        ndlist[1]->buildIR(cfg);
                        cfg->current_bb->add_IRInstr(IRInstr::jmp, INT, retvector);
                        std::cout << "---------------end THEN \n ";
                        
                        //BasicBlock * jumpBB = new BasicBlock(cfg, "jumpBB");
                        //cfg->add_bb(jumpBB);
                        //else
                        BasicBlock * elseBB = new BasicBlock(cfg, elselabel);
                        cfg->current_bb = elseBB;
                        elseBB->add_IRInstr(IRInstr::label, INT, retvector);
                        ndlist[2]->buildIR(cfg);
                        std::cout << "---------------end OELSEP_IF \n ";
                        cfg->add_bb(elseBB);

                        //after
                        BasicBlock * afterBB = new BasicBlock(cfg, afterlabel);
                       
                        cfg->current_bb = afterBB;
                        std::cout << "---------------end AFTER \n ";
                        cfg->add_bb(afterBB);

                        //liaison entre les if else avec le afterBB
                        testBB->exit_false=elseBB;
                        testBB->exit_true=thenBB;
                        thenBB->exit_true=afterBB;
                        thenBB->exit_false=NULL;
                        elseBB->exit_false=NULL;
                        elseBB->exit_true=afterBB;

                        
                        

                        testBB->add_IRInstr(IRInstr::cmpl, INT, retvector);
                       // thenBB->add_IRInstr(IRInstr::jmp, INT, retvector);
                        elseBB->add_IRInstr(IRInstr::jmp, INT, retvector);
                        afterBB->add_IRInstr(IRInstr::label, INT, retvector);
                        break;
                       } 
                case OP_RETURN:
                        sleft = ndlist[0]->buildIR(cfg);
                        sright = "!retval";
                        retvector.push_back(sright);
                        retvector.push_back(sleft);
                        cfg->current_bb->add_IRInstr(
                            IRInstr::wmem,
                            INT,
                            retvector);
                        break;

                default:
                        std::cout << "Erreur lors de la génération de l'IR" << std::endl;
                        std::cout << "Fonctionnalité non implémentée" << std::endl;
                        exit(EXIT_FAILURE);
                        break;
                }
                return "";
        }

        /* Operator */
        int op;

        /*
         * List of AST nodes
         * For expressions, left is ndlist[0], right is ndlist[1]
         * Used mostly for blocs
         */
        std::vector<Node *> ndlist;

        /* 0, 1 or 2 arguments */
        int args[1];
};

class Function {
public:
        std::string name;
        Symboltable *symb;
        std::vector<Node *> funcInstr;
        int offset;

        Function(std::string name)
        {
                this->name = name;
                cout << "Creating Function named : " << this->name << endl;
                symb = new Symboltable();
                funcInstr = std::vector<Node *>();
        }

        void computeOffset()
        {
                int a = symb->getNextOffset();
                std::cout << "True offset  : " << a << std::endl;
                int round = (-a + 16) - (-a % 16);
                std::cout << "Offset rounded to 16 : " << round << std::endl;
        }

        /* Getters and Setters */
        std::vector<Node *> getInstr()
        {
                return this->funcInstr;
        }

        Symboltable *getSymboltable()
        {
                return this->symb;
        }
};

#endif