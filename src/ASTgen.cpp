#include <vector>
#include <string>
#include "ASTgen.h"
#include "decl.h"
#include "ASTNode.h"


antlrcpp::Any Visitor::visitAxiom(ifccParser::AxiomContext *ctx) 
 {  std::cout << "Bonjour" << std::endl;
     return visitChildren(ctx);}
