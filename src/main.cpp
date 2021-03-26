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
#include "x86asmgen.h"

using namespace antlr4;
using namespace std;

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

    std::vector<Node*> n = visitor.getNodes();

    std::cout << "\n### List of AST Nodes ###" << std::endl;
    for(int i=0; i < n.size(); ++i) {
        n.at(i)->display();
    }
    //asmprint(n);
    //asmgen(n);

    return 0;
}
