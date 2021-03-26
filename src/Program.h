#pragma once

#include<iostream>
#include<vector>
#include"ASTNode.h"
#include"symboltable.h"
class Program {
public:


        // Todo un jour il faudra encapsuler :
        // Chauqe fonction renvoie un vector d'AST
        // Puis on construit les CFG.
        void buildIR();
        void addCFG(CFG* cfg) {
            CFGs.push_back(cfg);
        }

protected:
    std::vector<CFG *> CFGs; 

};

class DefFonction {


public:
    // This class appears in IR.h 
    // without any detail
    std::vector<Node*> funcInstr;



};
