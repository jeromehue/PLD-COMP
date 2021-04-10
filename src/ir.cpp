#include "ir.h"
#include <iostream>

#include "verbose.h"

using namespace std;

void IRInstr::gen_asm(ostream &o)
{
        switch (this->op) {
        case ldconst: {
                // load a constant (#1) in a variable (#0)
                int index = bb->cfg->symbols->getAddress(params[0]);
                o << "\tmovl\t$" << params[1] << "," << index << "(%rbp)\n";
                break;
        }

        case add: {
                int index = bb->cfg->symbols->getAddress(params[1]);
                o << "\tmovl\t" << index << "(%rbp),%eax\n";
                int index2 = bb->cfg->symbols->getAddress(params[2]);
                o << "\taddl\t" << index2 << "(%rbp),%eax\n";
                int index3 = bb->cfg->symbols->getAddress(params[0]);
                o << "\tmovl\t%eax," << index3 << "(%rbp)\n";
                break;
        }

        case cmpl: {
                int index = bb->cfg->symbols->getAddress(params[0]);
                string elseLabel = params[1];
                o << "\tcmpl\t$0, " << index << "(%rbp)\n";
                o << "\tje\t" << elseLabel << "\n";
                break;
        }

        case jmp: {
                string afterLabel = params[params.size() - 1];
                o << "\tjmp\t" << afterLabel << "\n";
                break;
        }

        case label: {
                o << bb->label << ":\n";
                break;
        }

        case sub: {
                int index = bb->cfg->symbols->getAddress(params[1]);
                int index2 = bb->cfg->symbols->getAddress(params[2]);
                o << "\tmovl\t" << index2 << "(%rbp),%eax\n";
                o << "\tsubl\t" << index << "(%rbp),%eax\n";
                int index3 = bb->cfg->symbols->getAddress(params[0]);
                o << "\tmovl\t%eax," << index3 << "(%rbp)\n";
                break;
        }

        case mul: {
                int index = bb->cfg->symbols->getAddress(params[1]);
                o << "\tmovl\t" << index << "(%rbp),%eax\n";
                int index2 = bb->cfg->symbols->getAddress(params[2]);
                o << "\timull\t" << index2 << "(%rbp),%eax\n";
                int index3 = bb->cfg->symbols->getAddress(params[0]);
                o << "\tmovl\t%eax," << index3 << "(%rbp)\n";
                break;
        }

        case div: {
                int index = bb->cfg->symbols->getAddress(params[1]);
                int index2 = bb->cfg->symbols->getAddress(params[2]);
                o << "\tmovl\t" << index << "(%rbp),%eax\n";
                o << "\tcltd\n";
                o << "\tidivl\t" << index2 << "(%rbp)\n";
                int index3 = bb->cfg->symbols->getAddress(params[0]);
                o << "\tmovl\t%eax," << index3 << "(%rbp)\n";
                break;
        }

        case bit_and: {
                int index = bb->cfg->symbols->getAddress(params[1]);
                o << "\tmovl\t" << index << "(%rbp),%eax\n";
                int index2 = bb->cfg->symbols->getAddress(params[2]);
                o << "\tandl\t" << index2 << "(%rbp),%eax\n";
                int index3 = bb->cfg->symbols->getAddress(params[0]);
                o << "\tmovl\t%eax," << index3 << "(%rbp)\n";
                break;
        }

        case bit_xor: {
                int index = bb->cfg->symbols->getAddress(params[1]);
                o << "\tmovl\t" << index << "(%rbp),%eax\n";
                int index2 = bb->cfg->symbols->getAddress(params[2]);
                o << "\txorl\t" << index2 << "(%rbp),%eax\n";
                int index3 = bb->cfg->symbols->getAddress(params[0]);
                o << "\tmovl\t%eax," << index3 << "(%rbp)\n";
                break;
        }

        case bit_or: {
                int index = bb->cfg->symbols->getAddress(params[1]);
                o << "\tmovl\t" << index << "(%rbp),%eax\n";
                int index2 = bb->cfg->symbols->getAddress(params[2]);
                o << "\torl\t" << index2 << "(%rbp),%eax\n";
                int index3 = bb->cfg->symbols->getAddress(params[0]);
                o << "\tmovl\t%eax," << index3 << "(%rbp)\n";
                break;
        }

        case greater: {
                int index = bb->cfg->symbols->getAddress(params[1]);
                int index2 = bb->cfg->symbols->getAddress(params[2]);
                o << "\tmovl\t" << index2 << "(%rbp), %eax\n";
                o << "\tcmpl\t" << index << "(%rbp), %eax\n";
                o << "\tsetg\t%al\n";
                o << "\tmovzbl\t%al, %eax\n";
                int index3 = bb->cfg->symbols->getAddress(params[0]);
                o << "\tmovl\t%eax, " << index3 << "(%rbp)\n";
                break;
        }

        case lower: {
                int index = bb->cfg->symbols->getAddress(params[1]);
                int index2 = bb->cfg->symbols->getAddress(params[2]);
                o << "\tmovl\t" << index2 << "(%rbp), %eax\n";
                o << "\tcmpl\t" << index << "(%rbp), %eax\n";
                o << "\tsetl\t%al\n";
                o << "\tmovzbl\t%al, %eax\n";
                int index3 = bb->cfg->symbols->getAddress(params[0]);
                o << "\tmovl\t%eax, " << index3 << "(%rbp)\n";
                break;
        }

        case cmp_eq: {
                int index = bb->cfg->symbols->getAddress(params[1]);
                int index2 = bb->cfg->symbols->getAddress(params[2]);
                o << "\tmovl\t" << index2 << "(%rbp), %eax\n";
                o << "\tcmpl\t" << index << "(%rbp), %eax\n";
                o << "\tsete\t%al\n";
                o << "\tmovzbl\t%al, %eax\n";
                int index3 = bb->cfg->symbols->getAddress(params[0]);
                o << "\tmovl\t%eax, " << index3 << "(%rbp)\n";
                break;
        }

        case cmp_uneq: {
                int index = bb->cfg->symbols->getAddress(params[1]);
                int index2 = bb->cfg->symbols->getAddress(params[2]);
                o << "\tmovl\t" << index2 << "(%rbp), %eax\n";
                o << "\tcmpl\t" << index << "(%rbp), %eax\n";
                o << "\tsetne\t%al\n";
                o << "\tmovzbl\t%al, %eax\n";
                int index3 = bb->cfg->symbols->getAddress(params[0]);
                o << "\tmovl\t%eax, " << index3 << "(%rbp)\n";
                break;
        }

        case wmem: {
                // The value of variable var is written at address addr
                // Special case : return
                if (params[0] == "!retval") {
                        verbose("return wmem");
                        int index = bb->cfg->symbols->getAddress(params[1]);
                        o << "\tmovl\t" << index << "(%rbp),%eax\n";
                        break;
                }

                // Fetch addresses from our variables names
                int var_index, addr;
                addr = this->bb->cfg->symbols->getAddress(params[0]);
                var_index = this->bb->cfg->symbols->getAddress(params[1]);

                // Debug prints
                if (Verbose) {
                        cout <<"addr : "<<params[0]<< " " << addr << " \n";
                        cout <<"var  : "<<params[1]<< " " << var_index;
                        cout << endl;
                }

                o << "\tmovl\t" << var_index << "(%rbp),%eax\n";
                o << "\tmovl\t%eax," << addr << "(%rbp)\n";
                break;
        }

        case call: {
                if (Verbose) {
                        cout << "number of params  :"
                             << this->params.size() - 2 << endl;
                }

                int nb_params = this->params.size() - 2;

                if (nb_params > 6) {
                        cout << "Too much parameters" << endl;
                        exit(EXIT_FAILURE);
                }

                string name = this->params[1];
                int exp = 
                this->bb->cfg->symbols->fct_params.find(name)->second;

                if (Verbose) {
                        cout << "calling " << name << " who has "
                             << exp << " paramerers" << endl;
                }

                if (nb_params != exp) {
                        cout << "Error : function " << name
                             << " called, expected " << exp << " arguments"
                             << " but found " << nb_params << "."
                             << endl;
                        exit(EXIT_FAILURE);
                }

                string treg[6] =
                {"%edi", "%esi", "%edx", "%ecx", "%r8d", "%r9d"};

                for (int i = 0; i < nb_params; ++i) {
                        int idx = 
                                bb->cfg->symbols->getAddress(params[i + 2]);
                        o<<"\tmovl\t"<<idx<<"(%rbp), "<<treg[i]<< "\n";
                }

                o << "\tcall\t" << this->params[1] << "\n";
                int index3 = bb->cfg->symbols->getAddress(params[0]);
                o << "\tmovl\t%eax," << index3 << "(%rbp)\n";
                break;
        }

        case array_access: {
                int index = bb->cfg->symbols->getAddress(params[1]);
                o << "\tmovl\t" << index << "(%rbp), %eax\n";
                o << "\tcltq\n";
                string firstElement = "tab" + params[0] + "0";
                verbose("Array starts at index " + firstElement);
                int startIndex = bb->cfg->symbols->getAddress(firstElement);
                o << "\tmovl\t" << startIndex << "(%rbp,%rax,4), %eax\n";
                int dest = bb->cfg->symbols->getAddress(params[2]);
                o << "\tmovl\t%eax, " << dest << "(%rbp)\n";
                break;
        }

        case putchar: {
                int index = bb->cfg->symbols->getAddress(params[0]);
                o << "\tmovl\t" << index << "(%rbp), %edi\n";
                o << "\tcall\tputchar\n";
                break;
        }

        case getchar: {
                cout << "getchar gen asm " << endl;
                o << "\tmovl\t$0,%eax\n";
                o << "\tcall\tgetchar\n";
                int index = bb->cfg->symbols->getAddress(params[0]);
                o << "\tmovl\t%eax," << index << "(%rbp)\n";
                break;
        }

        default:
                cout << "gen_asm not implemented" << endl;
                break;
        }
}

int CFG::getsizebbs()
{
        return bbs.size();
}

void writeBB(BasicBlock *bb, ostream &o)
{
        bb->gen_asm(o);

        if (bb->exit_true != nullptr) {
                if (bb->exit_false != nullptr) {
                        writeBB(bb->exit_true, o);
                        writeBB(bb->exit_false, o);

                } else {
                        //writeBB(bb->exit_true, o);
                }
        } else {
                cout << "basicBlock sans exitTrue ni exitFalse\n";
        }
}

void CFG::load_parameters(ostream &o, int nb_params)
{
        o << "\n\t# nb params " << nb_params << endl;

        if (nb_params == 0) return;

        string tab_reg[6] = {"%edi","%esi","%edx","%ecx","%r8d","%r9d"};

        for (int i = 0; i < nb_params; ++i) {
                int index = -(i + 1) * 4;
                o << "\tmovl\t" << tab_reg[i] << "," << index << "(%rbp)"
                  << "\n";
        }
}

void CFG::gen_asm(ostream &o)
{
        verbose("call to CFG::gen_asm");
        verbose("Number of bbs : " + to_string(bbs.size()));

        int exp = this->symbols->fct_params.find("f2")->second;
        verbose(" >> func expected numbers of parameters " +to_string(exp));

        o << ".global " << bbs.at(0)->label << "\n"
          << bbs.at(0)->label << ":\n"
          "\t# Prologue\n"
          "\tpushq\t%rbp\n"
          "\tmovq\t%rsp, %rbp\n"
          "\tsubq\t$"
          << this->symbols->getOffset() << ", %rsp";
        load_parameters(o, this->symbols->getNbParams());
        o << "\n\t# Body\n";

        // Current bb is pointing to body function
        for (BasicBlock *bb : bbs) {
                bb->gen_asm(o);
        }

        o << "\t# Epilogue\n"
          "\tleave\n"
          "\tret\n";
}
