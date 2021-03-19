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

enum TYPE {
    INT = 0,
    CHAR = 1
};

struct Variable {
    int address;
    int type;
};

class Symboltable {
public:
    Symboltable(){
        var_addr = 0;
    }
    
    int store(std::string name,  int type){
        var_addr -= 4;
        
        Variable v;
        v.address = var_addr;
        v.type = type;

        symbols.insert(std::pair<std::string, Variable>(name, v));
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
    
    int getAddress(std::string name) {
        auto it = symbols.find(name);
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
    // address of the last variable declared on the rbp stack
    int var_addr;
};

