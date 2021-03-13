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

struct Variable {
    int address;
    int value;
};

class Symboltable {
public:
    Symboltable(){
        var_addr = 0;
    }
    
    int store(std::string name){
        var_addr -= 4;
        symbols.insert(std::pair<std::string, int>(name, var_addr));
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
        return it->second;
    }    
    
    void createEntry(std::string name) {
        vardeclarations.push_back(name);        
    }

    std::map<std::string, int> symbols;
    std::vector<std::string> vardeclarations;
    protected:
    // address of the last variable declared on the rbp stack
    int var_addr;
};

