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

using namespace antlr4;
using namespace std;

static std::ofstream output;

/*
 * “Sometimes, the elegant implementation is just a function. 
 * Not a method. Not a class. Not a framework. Just a function.” 
 *                                                      – John Carmack
 */                                                      

void prologue() {
    output << 
        ".global main\n"
        "main:\n"
        "\t# Prologue\n"
        "\tpushq %rbp\n"
        "\tmovq %rsp, %rbp\n"
        "\n"
        "\t# Body\n";
}

void epilogue() {
    output <<
        "\n"
        "\t# Epilogue\n"
        "\tpopq %rbp\n"
        "\tret\n";
}


int main(int argn, const char **argv) {

    stringstream in;
    if (argn == 2) {
        ifstream lecture(argv[1]);
        in << lecture.rdbuf();
    }

    ANTLRInputStream input(in.str());
    ifccLexer lexer(&input);
    CommonTokenStream tokens(&lexer);

    tokens.fill();
    //  for (auto token : tokens.getTokens()) {
    //    std::cout << token->toString() << std::endl;
    //  }

    ifccParser parser(&tokens);
    tree::ParseTree *tree = parser.axiom();

    if (parser.getNumberOfSyntaxErrors() != 0) {
        cout << "erreur of syntax";
        return 1;
    }

    Visitor visitor;
    visitor.visit(tree);

        /* temp */
        bool hasMain = false;
        int mainFuncIndex;

        std::vector <Function *> functions = visitor.getFunctions();
        for(int i = 0; i<functions.size(); ++i){
                cout << "------------"<< std::endl;
                cout <<  "Function n°"<< i << " : " 
                << functions.at(i)->name <<endl;
                
                std::vector<Node*> n = functions.at(i)->funcInstr;

                if(functions.at(i)->name == "main" ) {
                        hasMain = true;
                        mainFuncIndex = i;
                }

                std::cout 
                << "\n### List of AST Nodes for function n° " << i 
                << " ###" << std::endl;
                for(int i=0; i < n.size(); ++i) {
                        n.at(i)->display();
                }       
                cout << std::endl;
        }

        if (!hasMain) {
                std::cout << "No main function found " << std::endl;
                exit(EXIT_FAILURE);
        }

        int mfi = mainFuncIndex;
        
        std::vector<Node* > n = functions.at(mfi)->getInstr();



        // Now generate the IR
        std::cout << "\n### IR Generation ###" << std::endl;
       
        output.open("output.s");
        for (int i = 0; i<functions.size(); ++i) {
                std::string name = functions.at(i)->name;

                 CFG* mainCFG =new CFG(functions.at(i)->getSymboltable());

                // prologue
                BasicBlock* BBinput = new BasicBlock(mainCFG,name);

                // body
                BasicBlock* BBbody = new BasicBlock(mainCFG, name + "Body");

                // epilogue
                BasicBlock* BBoutput=new BasicBlock(mainCFG, name+"Output");

                // Default behaviour
                BBbody->exit_true = BBoutput;
                
                mainCFG->add_bb(BBinput);
                mainCFG->add_bb(BBbody);
                mainCFG->add_bb(BBoutput);
                mainCFG->current_bb = BBbody;
                
                std::vector<Node* > n = functions.at(i)->getInstr();
                for(int i=0; i<n.size(); ++i) {
                        n[i]->buildIR(mainCFG);
                }


                mainCFG->gen_asm(output);
        }
        output.close();
        /*
        CFG* mainCFG = new CFG(functions.at(mfi)->getSymboltable());

        // prologue
        BasicBlock* BBinput =
                new BasicBlock(mainCFG,functions.at(mfi)->name);

        // body
        BasicBlock* BBbody =
                new BasicBlock(mainCFG, "body");

        // epilogue
        BasicBlock* BBoutput = 
                new BasicBlock(mainCFG, functions.at(mfi)->name+"Ouput");

        // Default behaviour
        BBbody->exit_true = BBoutput;

        mainCFG->add_bb(BBinput);
        mainCFG->add_bb(BBbody);
        mainCFG->add_bb(BBoutput);
        mainCFG->current_bb = BBbody;

        
        for(int i=0; i<n.size(); ++i) {
                n[i]->buildIR(mainCFG);
        }


        output.open("output.s");
        mainCFG->gen_asm(output);
        output.close();
        */
        /*
        for(int i=0; i<n.size(); ++i) {
                n[i]->buildIR(mainCFG);
        }*/
        /*
        output.open("output.s");
        functions.at(mfi)->computeOffset();
        
        // One CFG for One Function
        CFG* mainCFG = new CFG(visitor.getglobalSymb());
        BasicBlock* inputBB;
        BasicBlock* outputBB;
        BasicBlock* mainBB = new BasicBlock(mainCFG, "main");
        mainCFG->current_bb = mainBB;
        
        for(int i=0; i<n.size(); ++i) {
                n[i]->buildIR(mainCFG);
        }
         
        // WIP : To be removed from here
        std::cout << "\n### ASM Generation ###" << std::endl;
        output.open("output.s");
        prologue();

        // Now the real thing
        mainBB->gen_asm(output); 

        epilogue();
        output.close();
        */
    return 0;
}
