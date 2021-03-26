#include <vector>
#include <string>
#include "ASTGenerator.h"
#include "ASTNode.h"
#include "cgen.h"

antlrcpp::Any Visitor::visitAxiom(ifccParser::AxiomContext *ctx) {
    std::cout << "Bonjour" << std::endl;
    return visitChildren(ctx);
}
