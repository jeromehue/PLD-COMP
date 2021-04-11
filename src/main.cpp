/*************************************************************************
                                   main.cpp 
                             -------------------
    start                : 2021-03-01
    copyright            : (C) 2021 by H4244
*************************************************************************/

/*
 *      H4244 members :
 *
 *      - Aziz Kanoun
 *      - Charly Poirier
 *      - Jérôme Hue
 *      - Lucie Clémenceau
 *      - Quentin Regaud
 *      - Sylvain de Joannis de Verclos
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>

#include "antlr4-runtime.h"
#include "ifccLexer.h"
#include "ifccParser.h"
#include "ifccBaseVisitor.h"
#include "node.h"
#include "visitor.h"
#include "verbose.h"

using namespace antlr4;
using namespace std;

static ofstream output;

bool Verbose;


int main(int argn, const char **argv)
{
        stringstream in;
        setVerbose(false);

        if (argn >= 2) {
                ifstream lecture(argv[1]);
                in << lecture.rdbuf();

                for (int i = 2; i < argn; ++i) {
                        if (argv[2] == string("-v")) {
                                setVerbose(true);
                                verbose("Verbose mode ON");
                        }
                }
        }

        ANTLRInputStream input(in.str());
        ifccLexer lexer(&input);
        CommonTokenStream tokens(&lexer);

        tokens.fill();

        ifccParser parser(&tokens);
        tree::ParseTree *tree = parser.axiom();

        if (parser.getNumberOfSyntaxErrors() != 0) {
                cout << "Syntax error";
                return 1;
        }

        Visitor visitor;
        visitor.visit(tree);


        verbose("visit ended");
        /* temp */
        bool hasMain = false;
        int mainFuncIndex;

        vector<Function *> functions = visitor.getFunctions();

        for (int i = 0; i < functions.size(); ++i) {
                if (Verbose) {
                        cout << "------------" << endl;
                        cout << "Function n°" << i << " : "
                             << functions.at(i)->name << endl;
                }

                vector<Node *> n = functions.at(i)->funcInstr;

                if (functions.at(i)->name == "main") {
                        hasMain = true;
                        mainFuncIndex = i;
                }

                if (Verbose) {
                        cout << "\n### List of AST Nodes for function n° "
                             << i
                             << " ###"
                             << endl;
                }

                bool missingReturn = true;

                for (int j = 0; j < n.size(); ++j) {
                        // Check for a return instruction
                        if (n.at(j)->op == OP_RETURN) {
                                missingReturn = false;
                        }

                        // Display AST nodes
                        if (Verbose) {
                                n.at(j)->display();
                        }
                }

                verbose("");

                /* Manually add a return 0 node id missing return */
                if (missingReturn) {
                        verbose("Adding missing return node");

                        Node *returnNode =
                                new Node(OP_RETURN, NULL, NULL, 0, 0);

                        Node *zeroNode =
                                new Node(OP_CONST, NULL, NULL, 0, 0);

                        returnNode->ndlist.at(0) = zeroNode;
                        functions.at(i)->funcInstr.push_back(returnNode);
                }
        }

        if (!hasMain) {
                cout << "No main function found " << endl;
                exit(EXIT_FAILURE);
        }

        int mfi = mainFuncIndex;

        vector<Node *> n = functions.at(mfi)->getInstr();

        // Now generate the IR
        verbose("\n### IR Generation ###\n");

        output.open("output.s");

        for (int i = 0; i < functions.size(); ++i) {
                string name = functions.at(i)->name;

                CFG *mainCFG = new CFG(functions.at(i)->getSymboltable());

                // prologue
                BasicBlock *BBinput = new BasicBlock(mainCFG, name);

                // body
                BasicBlock *BBbody = new BasicBlock(mainCFG, name + "Body");

                // epilogue
                BasicBlock *BBoutput = new BasicBlock(mainCFG,
                                                      name + "Output");

                // Default behaviour
                BBbody->exit_true = BBoutput;

                mainCFG->add_bb(BBinput);
                mainCFG->add_bb(BBbody);
                mainCFG->add_bb(BBoutput);
                mainCFG->current_bb = BBbody;

                vector<Node *> n = functions.at(i)->getInstr();

                for (int i = 0; i < n.size(); ++i) {
                        n[i]->buildIR(mainCFG);
                }

                //cout<<"  maincfg size: "<<mainCFG->getsizebbs()<<endl;
                verbose("gen asm");
                mainCFG->gen_asm(output);
        }

        output.close();
        cout << "Compilation passed" << std::endl;
        return 0;
}
