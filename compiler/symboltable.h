#pragma once

#include <string>
#include <map>

// Stocke l'adresse de la variable, 
// sa valeur et peut être d'autres
// trucs dans le futur
/*
struct Variable {
    int adress;
    int value;
}*/

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
    
    int getAdress(std::string name) {
        auto it = symbols.find(name);
        return it->second;
    }    

    std::map<std::string, int> symbols;
    protected:
    // address of the last variable declared on the rbp stack
    int var_addr;
};

