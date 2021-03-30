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

class Function {

public:

        std::string name;
        Symboltable* symb;
        std::vector<Node*> funcInstr; 
        int offset; 
        Function(std::string name)
        {
		this->name=name;
                cout << "Creating Function named : " << this->name << endl;
                symb = new Symboltable();
                funcInstr = std::vector<Node*>();
        }
        
        void computeOffset() {
                int a = symb->getNextOffset();
                std::cout << "True offset  : " << a << std::endl;
                int round  = (-a+16) - (-a % 16 );
                std::cout << "Offset rounded to 16 : " <<round<< std::endl;
        }


        /* Getters and Setters */
        std::vector<Node*> getInstr() {
                return this->funcInstr;
        }

        Symboltable* getSymboltable() {
                return this->symb;
        }
};
