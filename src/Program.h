#pragma once

#include<iostream>
#include<vector>
#include<ASTNode.h>
#include"symboltable.h"
class Program {
    public:
        std::vector<ASTNode *> instructions;
        Symboltable * mainST;

        void genAST() {};

    protected:

};
