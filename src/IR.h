#ifndef IR_H
#define IR_H

#include <vector>
#include <string>
#include <iostream>
#include <initializer_list>



// Personnal includes
#include "symboltable.h"

// Declarations from the parser -- replace with your own
//#include "type.h"
//#include "symbole.h"
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
		ldconst,    //  0
		copy,       //  1
		add,        //  2   
		sub,        //  3
		mul,        //  4
		rmem,       //  5
		wmem,       //  6
		call,       //  7 
		cmp_eq,     //  8 
		cmp_lt,     //  9  
		cmp_le      // 10
	} Operation;


	/**  constructor */
	IRInstr(BasicBlock* bb_, 
            Operation op, 
            Type t, 
            std::vector<std::string> params) {
        this->bb= bb_;
        this->op = op;
        this->t =t;
        this->params = params;
    }

        /**< Helper function */
        static const std::string getOpByIndex(int index) 
        {
                switch(index) {
                case 0:
                        return " >> ldconst [dest,const]";
                        break;
                case 1:
                        return " >> copy [dest, op1]";
                        break;
                case 2:
                        return " >> add [dest, op1, op2]";
                        break;
                case 3:
                        return " >> sub";
                        break;
                case 4:
                        return " >> mul";
                        break;
                case 5:
                        return " >> rmem";
                        break;
                case 6:
                        return " >> wmem [addr, var]";
                        break;
                case 7:
                        return " >> call";
                        break;
                case 8:
                        return " >> cmp_eq";
                        break;
                case 9:
                        return " >> cmp_lt";
                        break;
                case 10:
                        return " >> cmp_le";
                        break;
                default:
                        std::cout<<"Erreur : Opérateur inconnu"<<std::endl;
                        exit(EXIT_FAILURE);
                }
        }
	
        /** Actual code generation */
    
        /**< x86 assembly code generation for this IR instruction */
        void gen_asm(std::ostream &o);

        inline friend ostream& operator<<(ostream& os,IRInstr& instr) 
        {
                os << getOpByIndex(instr.op) << " Args : "; 
                for (int i=0; i< (int)instr.params.size(); ++i)
                        os << instr.params.at(i) << " ";
                os << std::endl;
                return os;
        }



private:

        /**< The BB this instruction belongs to, which provides 
        a pointer to the CFG this instruction belong to */
	BasicBlock* bb; 

	Operation op;
	Type t;
        
        
        /**<    For 3-op instrs: d, x, y; for ldconst: d, c;  
            For call: label, d, params;  
            for wmem and rmem: choose yourself */
	std::vector<std::string> params; 
        
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
        BasicBlock(CFG* cfg, std::string entry_label) 
        {
                this->cfg =cfg;
                this->label = entry_label;
        } 

        /* x86 assembly code generation for this basic block  */
        void gen_asm(std::ostream &o) 
        {
                for(int i = 0; i < (int)instrs.size(); ++i) {
                        instrs.at(i)->gen_asm(o);
                }
        }

	void add_IRInstr (IRInstr::Operation op, Type t,  
                          std::vector<std::string> params ) 
        {
                std::cout << "Writing IR instruction "<< op<<std::endl;
                IRInstr* instr = new IRInstr(this, op, t, params);
                this->instrs.push_back(instr);
                std::cout << "Current BB instructions : \n" ;
        
                for (int i = 0; i < (int)instrs.size(); i++)
                        std::cout << *(instrs.at(i));

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
        std::string label; 
	
        /** < the CFG where this block belongs */
	CFG* cfg;

        /** < the instructions themselves. */
        std::vector<IRInstr*> instrs; 

        /**<when generating IR code for an if(expr) or 
        while(expr) etc, store here the name of the variable 
        that holds the value of expr*/
        std::string test_var_name;  
 
};




 /* 
  * The class for the control flow graph, also includes the symbol table 
  * 
  * A few important comments:
  * 
  * The entry block is the one with the same label as the AST function name.
  * ( it could be the first of bbs, ot it could be defined by an attribute
  * node )
  *
  * The exit block is the one with both exit pointers equal to nullptr.
  * ( again it could be identified in a more explicit way )
  *
  * There is one cfg for one function
  *
  */ 

class CFG {
public:
        CFG(Symboltable * st) : symbols(st){};


	DefFonction* ast; /**< The AST this CFG comes from */
	
        void 
        add_bb(BasicBlock* bb) 
        {
                bbs.push_back(bb);
        }

	/*
         * x86 code generation: 
         * could be encapsulated in a processor class 
         * in a retargetable compiler.
         */
        void gen_asm(std::ostream& o);

        /* 
         * helper method: inputs a IR reg or input variable,
         * returns e.g. "-24(%rbp)" for the proper value of 24 
         */
	std::string IR_reg_to_asm(std::string reg);

	void gen_asm_prologue(std::ostream& o);
	void gen_asm_epilogue(std::ostream& o);

	// symbol table methods
	void add_to_symbol_table(std::string name, Type t);

        std::string create_new_tempvar(int t) 
        {

                int index= symbols->getNextOffset();
                std::string var_name = "!tmp" + to_string(- index);
        
                symbols->store(var_name, INT);

                // Debug prints
                std::cout<<"Creating new tempvar :"<<var_name<<std::endl;
                return var_name;
        }

	int get_var_index(std::string name);
	Type get_var_type(std::string name);

	// basic block management
	std::string new_BB_name();
	BasicBlock* current_bb;

        Symboltable* symbols;

protected:
   
     
        /**< part of the symbol table  */
	std::map <std::string, Type> SymbolType; 

        /**< part of the symbol table  */
        std::map <std::string, int> SymbolIndex; 

        /**< to allocate new symbols in the symbol table */
        int nextFreeSymbolIndex; 
	
        /**< just for naming */
        int nextBBnumber; 
	
        /**< all the basic blocks of this CFG*/
        std::vector <BasicBlock*> bbs;


};


#endif

