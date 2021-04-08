#ifndef IR_H
#define IR_H

#include <vector>
#include <string>
#include <iostream>
#include <initializer_list>

#include "symb.h"

using namespace std;

class BasicBlock;
class CFG;
class DefFonction;

/*
 * A fex definitions :
 * - IR instructions : an abstractio of an asm language instruction
 * - Basic Block : an atomic sequence of instructions
 * - Control Flow Graph : a graph of basic blocks
 */

//! The class for one 3-address instruction
class IRInstr {
public:
	/** The instructions themselves -- feel free to subclass instead */
	typedef enum {
		ldconst,
		copy,
		add,
		sub,
		mul,
		div,
		bit_and,
		bit_xor,
                bit_or,
		rmem,
		wmem,
		call,
		cmp_eq,
		cmp_uneq,
		cmp_le,
                greater,
                lower,
                cmpl,
                jmp,
                label,
                array_access,
                putchar,
                getchar,

	} Operation;

	/** constructor */
	IRInstr(BasicBlock* bb_, Operation op,
                Type t, vector<string> params)
        {
                this->bb= bb_;
                this->op = op;
                this->t =t;
                this->params = params;
        }

        /**< Helper function */
        static const string getOpByIndex(int index)
        {
                switch(index) {
                case 0:  return " >> ldconst [dest,const]";
                case 1:  return " >> copy [dest, op1]";
                case 2:  return " >> add [dest, op1, op2]";
                case 3:  return " >> sub";
                case 4:  return " >> mul";
                case 5:  return " >> div";
                case 6:  return " >> bit_and";
                case 7:  return " >> bit_xor";
                case 8:  return " >> bit_or";
                case 9:  return " >> rmem";
                case 10: return " >> wmem [addr, var]";
                case 11: return " >> call";
                case 12: return " >> cmp_eq";
                case 13: return " >> cmp_uneq";
                case 14: return " >> cmp_le";
                case 15: return " >> greater";
                case 16: return " >> lower";
                case 17: return " >> cmpl";
                case 18: return " >> jmp";
                case 19: return " >> label";
                case 20: return " >> array_access";
                case 21: return " >> putchar";
                case 22: return " >> getchar";
                default:
                        cout<<"Erreur : Opérateur inconnu"<<endl;
                        exit(EXIT_FAILURE);
                }
        }
	
        /** Actual code generation */

        /**< x86 assembly code generation for this IR instruction */
        void gen_asm(ostream &o);

        inline friend ostream& operator<<(ostream& os,IRInstr& instr)
        {
                os << getOpByIndex(instr.op) << " Args : ";
                for (int i=0; i< (int)instr.params.size(); ++i)
                        os << instr.params.at(i) << " ";
                os << endl;
                return os;
        }

private:
        /**< The BB this instruction belongs to, which provides
        a pointer to the CFG this instruction belong to */
	BasicBlock* bb;

	Operation op;
	Type t;

        /*
         * For 3-op instrs: d, x, y; for ldconst: d, c;
         * For call: label, d, params;
         * for wmem and rmem: choose yourself *
         */
	vector<string> params;

        /*
         * if you subclass IRInstr, each IRInstr subclass has its
         * parameters ant the previous (very important )
         * comment become useless: it would be a better design.
         */
};

/**  The class for a basic block */

/* A few important comments.
 *
 * IRInstr has no jump instructions.
 *	
 * cmp_* instructions behaves as an arithmetic
 * two-operand instruction (add or mult),
 * returning a boolean value (as an int)
 *
 * Assembly jumps are generated as follows:
 * BasicBlock::gen_asm() first calls IRInstr::gen_asm()
 * on all its instructions, and then
 * if  exit_true  is a  nullptr,
 *      the epilogue is generated
 * else if exit_false is a nullptr,
 *      an unconditional jmp to the exit_true branch is generated
 * else (we have two successors, hence a branch)
 *      an instruction comparing the value of test_var_name
 *      to true is generated, followed by a conditional
 *      branch to the exit_false branch, followed
 *      by an unconditional branch to the exit_true branch
 *	
 * The attribute test_var_name itself is defined
 * when converting the if, while, etc of the AST  to IR.
 *
 * Possible optimization:
 * a cmp_* comparison instructions, if it is the last
 * instruction of its block, generates an actual assembly comparison
 * followed by a conditional jump to the exit_false branch
 */
class BasicBlock {
public:
        BasicBlock(CFG* cfg, string entry_label)
        {
                this->cfg =cfg;
                this->label = entry_label;
        }

        /* x86 assembly code generation for this basic block  */
        void gen_asm(ostream &o)
        {
                cout << "Call to BB::gen_asm()" << endl;
                for(int i = 0; i < (int)instrs.size(); ++i) {
                        instrs.at(i)->gen_asm(o);
                }
        }

	void add_IRInstr (IRInstr::Operation op, Type t,
                          vector<string> params )
        {
                cout << "Writing IR instruction "<< op<<endl;
                IRInstr* instr = new IRInstr(this, op, t, params);
                this->instrs.push_back(instr);
                /*For debug only*/

                cout << "Current BB instructions : \n" ;
                for (int i = 0; i < (int)instrs.size(); i++)
                        cout << *(instrs.at(i));
        }

	// No encapsulation whatsoever here. Feel free to do better.

        /**< pointer to the next basic block, true branch.
        If nullptr, return from procedure */
	BasicBlock* exit_true;

        /**< pointer to the next basic block, false branch.
        If null_ptr, the basic block ends with
        an unconditional jump */	
        BasicBlock* exit_false;

        /**< label of the BB, also will be the label
        in the generated code */
        string label;
	
        /** < the CFG where this block belongs */
	CFG* cfg;

        /** < the instructions themselves. */
        vector<IRInstr*> instrs;

        /*
         * when generating IR code for an if(expr) or while(expr) etc,
         * store here the name of the variable that holds the value
         * of expr
         */
        string test_var_name;
};

/*
 * The class for the control flow graph, also includes the symbol table
 *
 * A few important comments:
 *
 * There is one cfg for each C function
 *
 * Each BB of the cfg has a label
 *
 * The CFG of a C function is composed of :
 *      - an input BB (it will generate the prologue)
 *      - an output BB  (it will generate the epilogue)
 *      - an arbitrary number of other BBs for the body
 *
 * The entry block is the one with the same label as the AST function name.
 * ( it could be the first of bbs, ot it could be defined by
 * an attribute node )
 *
 * The exit block is the one with both exit pointers equal to nullptr.
 * ( again it could be identified in a more explicit way )
 *
 *
 */
class CFG {
public:
        CFG(Symboltable * st) : symbols(st){};

	/**< The AST this CFG comes from */
	DefFonction* ast;

        void add_bb(BasicBlock* bb)
        {
                bbs.push_back(bb);
        }

	/*   case OP_UNEQUAL:
                        cout << "OP_UNEQUAL | " << endl;
                        cout << "\tleft : ";
                        ndlist[0]->display();
                        cout << "\tright : ";
                        ndlist[1]->display();
                        cout << endl;
                        break;compiler.
         */
        void gen_asm(ostream& o);

        /* Helper function to generate asm */
        void load_parameters(ostream& o, int nbParams);

        /*
         * helper method: inputs a IR reg or input variable,
         * returns e.g. "-24(%rbp)" for the proper value of 24
         */
	string IR_reg_to_asm(string reg);

	void gen_asm_prologue(ostream& o);
	void gen_asm_epilogue(ostream& o);

	// symbol table methods
	void add_to_symbol_table(string name, Type t);

        string create_new_tempvar(int t)
        {
                int index= symbols->getNextOffset();
                string var_name = "!tmp" + to_string(- index);

                symbols->store(var_name, INT);

                // Debug prints
                cout<<"Creating new tempvar :"<<var_name<<endl;
                return var_name;
        }
        int getsizebbs();
	int get_var_index(string name);
	Type get_var_type(string name);

	// basic block management
	string new_BB_name();
	BasicBlock* current_bb;

        Symboltable* symbols;

protected:
        /**< part of the symbol table  */
	map <string, Type> SymbolType;

        /**< part of the symbol table  */
        map <string, int> SymbolIndex;

        /**< to allocate new symbols in the symbol table */
        int nextFreeSymbolIndex;
	
        /**< just for naming */
        int nextBBnumber;
	
        /**< all the basic blocks of this CFG*/
        vector <BasicBlock*> bbs;
};

#endif
