/*************************************************************************
                                Symboltable
                             -------------------
    start                   : 2021-03-01
    copyright               : (C) 2021 by H4244
*************************************************************************/

#ifndef SYMB_H
#define SYMB_H

using namespace std;

//---- Interface of the <Symboltable> class (symb.h file) ----------------

//-------------------------------------------------------- Interfaces Used
#include <map>
#include <string>
#include <algorithm>
#include "verbose.h"

//------------------------------------------------------------------ Types
typedef enum {
        INT = 0,
        CHAR = 1
} Type;

struct Variable {
        int address;
        int type;
        bool defined;
        bool isParam;
};

class Symboltable
{
public:
        Symboltable()
        {
                next_offset = -4;
        }

        int store(string name, int type, bool isParam = false)
        {
                // Has the variable already been declared ?
                if (symbols.find(name) != symbols.end()) {
                        cerr << "Error: variable was already declared"
                             << endl;
                        exit(EXIT_FAILURE);
                }

                Variable v;
                v.address = next_offset;
                v.type = type;
                v.defined = false;
                v.isParam = isParam;

                symbols.insert(pair<string, Variable>(name, v));

                if (Verbose) {
                        cout << " >> New entry in symbol table : "
                             << name << " @" << v.address << endl;
                }

                // Let's not forget to update next offset
                next_offset = next_offset - 4;

                // Return var address
                return v.address;
        }

        int getNextOffset()
        {
                return next_offset;
        }

        bool find(string variable)
        {
                auto it = symbols.find(variable);

                if (it == symbols.end())
                        return false;

                return true;
        }

        string getName(int index)
        {
                for (auto &it : symbols) {
                        if (it.second.address == index) {
                                return it.first;
                        }
                }

                cout << "Variable not found" << endl;
                exit(EXIT_FAILURE);
        }

        int getAddress(string name)
        {
                auto it = symbols.find(name);

                if (it == symbols.end()) {
                        cout << "Error: undeclared variable  '"
                             << name << "'"
                             << endl;
                        exit(EXIT_FAILURE);
                }

                return it->second.address;
        }

        void printSymbols()
        {
                auto it = symbols.begin();

                for (auto const &x : symbols) {
                        cout << x.first << endl;
                }
        }

        int getNbParams()
        {
                int nb_params = 0;

                for (auto const &x : symbols) {
                        if (x.second.isParam)
                                nb_params++;
                }

                return nb_params;
        }

        int getOffset()
        {
                int a = this->getNextOffset() - 4;
                int round = (-a + 16) - (-a % 16);
                return round;
        }

        void allocate(int base, int n)
        {
                next_offset = base - 4 * n;
        }

        map<string, Variable> symbols;

        static map<string, int> fct_params;

protected:
        int next_offset;
};

#endif
