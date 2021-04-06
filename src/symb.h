/*************************************************************************
                                Symboltable
                             -------------------
        start                   : 2021-03
        copyright               : (C) 2021 by H4244
*************************************************************************/

#ifndef SYMB_H
#define SYMB_H

//---- Interface of the <Symboltable> class (symboltable.h file) ---------

//-------------------------------------------------------- Interfaces Used
#include <map>
#include <string>
#include <algorithm>

//------------------------------------------------------------------ Types
typedef  enum {
        INT = 0,
        CHAR = 1
} Type;

struct Variable {
        int address;
        int type;
        bool defined;
        bool isParam;
};

//----------------------------------------------------------------- PUBLIC

class Symboltable {
public:
//--------------------------------------------------------- Public methods
        Symboltable()
        {
                next_offset = -4;
        }


        int store(std::string name, int type, bool isParam = false)
        {
                // Has the variable already been declared ?
                if(symbols.find(name) != symbols.end()) {
                        std::cout
                        << "Erreur : la variable a déjà été déclarée"
                        << std::endl;
                        exit(EXIT_FAILURE);
                }

                Variable v;
                v.address       = next_offset;
                v.type          = type;
                v.defined       = false;
                v.isParam       = isParam; 

                symbols.insert(std::pair<std::string, Variable>(name, v));
                std::cout << " >> New entry in symbol table : "
                << name << " @"<< v.address<< std::endl;

                // Let's not forget to update next offset
                next_offset = next_offset - 4;

                // Return var address
                return v.address;
        }

        int getNextOffset()
        {
                return next_offset;
        }

        bool find(std::string variable)
        {
                auto it = symbols.find(variable);
                if (it == symbols.end())
                        return false;
                return true;
        }

        std::string getName(int index)
        {
                for (auto& it : symbols) {
                        if (it.second.address == index) {
                                return it.first;
                        }
                }
                std::cout << "Nom de variable introuvable" <<std::endl;
                exit(EXIT_FAILURE);
        }

        int getAddress(std::string name)
        {
                auto it = symbols.find(name);
                if (it == symbols.end()) {
                        std::cout << "erreur, variable non déclarée"
                                  << std::endl;
                        exit(EXIT_FAILURE);
                }
                return it->second.address;
        }

        void printSymbols()
        {
                auto it = symbols.begin();
                for (auto const& x : symbols) {
                        std::cout << x.first << std::endl;
                }
        }

        int getNbParams() {
                int nb_params = 0;
                for (auto const& x : symbols) {
                        if (x.second.isParam)
                                nb_params++;
                }
                return nb_params;
        }

        std::map<std::string, Variable> symbols;

protected:
        int next_offset;
};

#endif
