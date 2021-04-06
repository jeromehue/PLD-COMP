/************************************************************************

                             -------------------
    début                : 2021-02-27
    copyright            : (C) 2021 par H4244
*************************************************************************/

#ifndef NODE_H
#define NODE_H

#include <iostream>
#include <string>
#include "ir.h"
#include "symb.h"

using namespace std;

/* Les différents types de noeuds */
enum nodeOp {
        OP_ADD,
        OP_SUB,
        OP_MUL,
        OP_DIV,
        OP_BIT_AND,
        OP_BIT_XOR,
        OP_BIT_OR,
        OP_ASSIGN,
        OP_IDENT,
        OP_CONST,
        OP_RETURN,
        OP_LOWER,
        OP_GREATER,
        OP_EQUAL,
        OP_UNEQUAL,
        OP_CALL,
};

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
                case OP_CALL:
                        std::cout << "OP CALL " << this->strarg << " with "
                                << this->args[0] << " arguments" << std::endl;
                        break;
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

                case OP_DIV:
                        var1 = ndlist[0]->buildIR(cfg);
                        var2 = ndlist[1]->buildIR(cfg);
                        var3 = cfg->create_new_tempvar(INT);
                        retvector.push_back(var3);
                        retvector.push_back(var2);
                        retvector.push_back(var1);
                        // Actual instruction
                        cfg->current_bb->add_IRInstr(IRInstr::div, INT, retvector);
                        return var3;
                        break;

                case OP_BIT_AND: // &
                        var1 = ndlist[0]->buildIR(cfg);
                        var2 = ndlist[1]->buildIR(cfg);
                        var3 = cfg->create_new_tempvar(INT);
                        retvector.push_back(var3);
                        retvector.push_back(var2);
                        retvector.push_back(var1);
                        // Actual instruction
                        cfg->current_bb->add_IRInstr(IRInstr::bit_and, INT, retvector);
                        return var3;
                        break;
 
                case OP_BIT_XOR: // ^
                        var1 = ndlist[0]->buildIR(cfg);
                        var2 = ndlist[1]->buildIR(cfg);
                        var3 = cfg->create_new_tempvar(INT);
                        retvector.push_back(var3);
                        retvector.push_back(var2);
                        retvector.push_back(var1);
                        // Actual instruction
                        cfg->current_bb->add_IRInstr(IRInstr::bit_xor, INT, retvector);
                        return var3;
                        break;

                case OP_BIT_OR: // |
                        var1 = ndlist[0]->buildIR(cfg);
                        var2 = ndlist[1]->buildIR(cfg);
                        var3 = cfg->create_new_tempvar(INT);
                        retvector.push_back(var3);
                        retvector.push_back(var2);
                        retvector.push_back(var1);
                        // Actual instruction
                        cfg->current_bb->add_IRInstr(IRInstr::bit_or, INT, retvector);
                        return var3;
                        break;

                case OP_LOWER: // <
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

                case OP_GREATER: // >
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
                        break;

                case OP_ASSIGN:
                        sleft = ndlist[0]->buildIR(cfg);  // adresse variable
                        sright = ndlist[1]->buildIR(cfg); // adresse résultat expression
                        retvector.push_back(sleft);
                        retvector.push_back(sright);
                        std::cout << "Assign node building ir " 
                                << sleft << std::endl;
                        cfg->current_bb->add_IRInstr(
                            IRInstr::wmem, INT, retvector);
                        return sright;
                        break;

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
                case OP_CALL: 
                {
                        cout << "Generating IR for function call" << endl;
                        std::cout << "function name : " << this->strarg;
                        std::cout << "; arguments : " << this->ndlist.size()
                        << std::endl;

                        var3 = cfg->create_new_tempvar(INT);
                        retvector.push_back(var3);
                        retvector.push_back(this->strarg);

                        for(int i = 0; i< this->ndlist.size(); ++i) {
                                std::cout << "argi n°" << i <<  std::endl;
                                this->ndlist.at(i)->display();
                                std::cout << std::endl;

                                
                                // Either a const or var
                                string tmp_var = ndlist.at(i)->buildIR(cfg);

                                retvector.push_back(tmp_var);
                                
                                cout << "pushed : " << tmp_var << endl;
                        }
                        cfg->current_bb->add_IRInstr(IRInstr::call, 
                                                     INT, retvector);
                        return var3;
                        break;
                }
                default:
                        std::cout << "Erreur lors de la génération de l'IR"                             << std::endl;
                        std::cout << "Fonctionnalité non implémentée" 
                                << std::endl;
                        exit(EXIT_FAILURE);
                        break;
                }
                return "";
        }


        void addNode(Node* n) {
                ndlist.push_back(n);
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
        int args[2];
        
        std::string strarg;
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
                int a = symb->getNextOffset() - 4;
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
