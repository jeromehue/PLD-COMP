#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>

#include "antlr4-runtime.h"
#include "ifccLexer.h"
#include "ifccParser.h"
#include "ifccBaseVisitor.h"
#include "ASTNode.h"
#include "visitor.h"
#include "Program.h"

using namespace antlr4;
using namespace std;

static std::ofstream output;

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

        CFG* mainCFG = new CFG(functions.at(mfi)->getSymboltable());
        BasicBlock* mainBB =new BasicBlock(mainCFG,functions.at(mfi)->name);

        mainCFG->current_bb = mainBB;
        for(int i=0; i<n.size(); ++i) {
                n[i]->buildIR(mainCFG);
        }

        functions.at(mfi)->computeOffset();
        /*
        // One CFG for One Function
        CFG* mainCFG = new CFG(visitor.getglobalSymb());
        BasicBlock* inputBB;
        BasicBlock* outputBB;
        BasicBlock* mainBB = new BasicBlock(mainCFG, "main");
        mainCFG->current_bb = mainBB;
        
        for(int i=0; i<n.size(); ++i) {
                n[i]->buildIR(mainCFG);
        }*/
    // WIP : To be removed from here
    std::cout << "\n### ASM Generation ###" << std::endl;
    output.open("output.s");
    prologue();

    // Now the real thing
    mainBB->gen_asm(output); 

    epilogue();
    output.close();

    return 0;
}
