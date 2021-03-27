#pragma once

#include <string>
#include <map>

// Stocke l'adresse de la variable, 
// sa valeur et peut être d'autres
// trucs dans le futur

/*
A stocker dans la table :
    - Noms des variables (et constantes)
    - Fonctions et procédures
    - Variables temporaires générées par le compilateur
Pour toute variabe, garder en mémoire :
    - Nom
    - Type
    - Portée(pas forcément un attribut, à voir à l'implémentation)
*/


// Since IR.h need a 'Type' type
typedef  enum {
    INT = 0,
    CHAR = 1
} Type;

struct Variable {
    int address;
    int type;
};

class Symboltable {
public:
    Symboltable(){
        var_addr = 0;
    }

    int store(std::string name, int type) {
        if(symbols.find(name) != symbols.end()) {
            std::cout 
                << "Erreur : la variable a déjà été déclarée" 
                << std::endl;
            exit(EXIT_FAILURE);
        }
        
        var_addr -= 4;
        Variable v;
        v.address = var_addr;
        v.type = type;


        symbols.insert(std::pair<std::string, Variable>(name, v));
        std::cout << " >> New entry in symbol table : " 
            << name << " @"<< v.address<< std::endl;
        return var_addr;
    }
    
    int getNext_val_ptr() {
        return var_addr;
    }
   
    bool find(std::string variable) {
        auto it = symbols.find(variable);
        if (it == symbols.end()) 
            return false;
        return true;
    } 
    
    std::string getName(int index) {
        for (auto& it : symbols) { 
            if (it.second.address == index) {
            return it.first;
            }
        }    
        std::cout << "Nom de variable introuvable" <<std::endl;
        exit(EXIT_FAILURE);
    }
     
    int getAddress(std::string name) {
        auto it = symbols.find(name);
        if (it == symbols.end()) {
            std::cout << "erreur, variable non déclarée" << std::endl;
            exit(EXIT_FAILURE);
        }
        return it->second.address;
    }   

    void printSymbols() {
        auto it = symbols.begin();
        for (auto const& x : symbols) {
            std::cout << x.first << std::endl;
        }
    } 
    
    std::map<std::string, Variable> symbols;

protected:
    int var_addr; // Address of the last variable declared on the rbp stack
};

