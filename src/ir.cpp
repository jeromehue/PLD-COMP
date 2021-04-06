#include "ir.h"
#include <iostream>

void IRInstr::gen_asm(std::ostream &o)
{
        std::cout << "genrating asm for ir" << std::endl;
        //std::cout << "It worked, i'm on cpp file " << std::endl;
        //std::cout << this->bb->cfg->symbols->getAddress("b") << std::endl;
        switch(this->op) {
        case ldconst: {
                // load a constant (#1) in a variable (#0)
                int index = bb->cfg->symbols->getAddress(params[0]);
                o<<"\tmovl\t$"<<params[1]<<","<< index <<"(%rbp)\n";
                return;
        }
        case add: {
                int index = bb->cfg->symbols->getAddress(params[1]);
                o<<"\tmovl\t"<<index<<"(%rbp),%eax\n";
                int index2 = bb->cfg->symbols->getAddress(params[2]);
                o<<"\taddl\t"<<index2<<"(%rbp),%eax\n";
                int index3 = bb->cfg->symbols->getAddress(params[0]);
                o<<"\tmovl\t%eax," << index3 << "(%rbp)\n";
                return;
        }
        case sub: {
                int index = bb->cfg->symbols->getAddress(params[1]);
                int index2 = bb->cfg->symbols->getAddress(params[2]);
                o<<"\tmovl\t"<<index2<<"(%rbp),%eax\n";
                o<<"\tsubl\t"<<index<<"(%rbp),%eax\n";
                int index3 = bb->cfg->symbols->getAddress(params[0]);
                o<<"\tmovl\t%eax," << index3 << "(%rbp)\n";
                return;
        }
        case mul: {
                int index = bb->cfg->symbols->getAddress(params[1]);
                o<<"\tmovl\t"<<index<<"(%rbp),%eax\n";
                int index2 = bb->cfg->symbols->getAddress(params[2]);
                o<<"\timull\t"<<index2<<"(%rbp),%eax\n";
                int index3 = bb->cfg->symbols->getAddress(params[0]);
                o<<"\tmovl\t%eax," << index3 << "(%rbp)\n";
                return;
        }
        case div: {
                int index = bb->cfg->symbols->getAddress(params[1]);
                int index2 = bb->cfg->symbols->getAddress(params[2]);
                o<<"\tmovl\t"<<index<<"(%rbp),%eax\n";
                o<<"\tcltd\n";
                o<<"\tidivl\t"<<index2<<"(%rbp)\n";
                int index3 = bb->cfg->symbols->getAddress(params[0]);
                o<<"\tmovl\t%eax," << index3 << "(%rbp)\n";
                return;
        }
        case bit_and: {
                int index = bb->cfg->symbols->getAddress(params[1]);
                o<<"\tmovl\t"<<index<<"(%rbp),%eax\n";
                int index2 = bb->cfg->symbols->getAddress(params[2]);
                o<<"\tandl\t"<<index2<<"(%rbp),%eax\n";
                int index3 = bb->cfg->symbols->getAddress(params[0]);
                o<<"\tmovl\t%eax," << index3 << "(%rbp)\n";
                return;
        }
        case bit_xor: {
                int index = bb->cfg->symbols->getAddress(params[1]);
                o<<"\tmovl\t"<<index<<"(%rbp),%eax\n";
                int index2 = bb->cfg->symbols->getAddress(params[2]);
                o<<"\txorl\t"<<index2<<"(%rbp),%eax\n";
                int index3 = bb->cfg->symbols->getAddress(params[0]);
                o<<"\tmovl\t%eax," << index3 << "(%rbp)\n";
                return;
        }
        case bit_or: {
                int index = bb->cfg->symbols->getAddress(params[1]);
                o<<"\tmovl\t"<<index<<"(%rbp),%eax\n";
                int index2 = bb->cfg->symbols->getAddress(params[2]);
                o<<"\torl\t"<<index2<<"(%rbp),%eax\n";
                int index3 = bb->cfg->symbols->getAddress(params[0]);
                o<<"\tmovl\t%eax," << index3 << "(%rbp)\n";
                return;
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
                return;
        }
        case lower: {
                int index = bb->cfg->symbols->getAddress(params[1]);
                int index2 = bb->cfg->symbols->getAddress(params[2]);
                o<< "\tmovl\t" << index2 << "(%rbp), %eax\n";
                o<< "\tcmpl\t" << index << "(%rbp), %eax\n";
                o<< "\tsetl\t%al\n";
                o<< "\tmovzbl\t%al, %eax\n";
                int index3 = bb->cfg->symbols->getAddress(params[0]);
                o<< "\tmovl\t%eax, " << index3 << "(%rbp)\n";
                return;
        }
        case cmp_eq: {
                int index = bb->cfg->symbols->getAddress(params[1]);
                int index2 = bb->cfg->symbols->getAddress(params[2]);
                o<< "\tmovl\t" << index2 << "(%rbp), %eax\n";
                o<< "\tcmpl\t" << index << "(%rbp), %eax\n";
                o<< "\tsete\t%al\n";
                o<< "\tmovzbl\t%al, %eax\n";
                int index3 = bb->cfg->symbols->getAddress(params[0]);
                o<< "\tmovl\t%eax, " << index3 << "(%rbp)\n";
                return;
        }
        case cmp_uneq: {

                int index = bb->cfg->symbols->getAddress(params[1]);
                int index2 = bb->cfg->symbols->getAddress(params[2]);
                o<< "\tmovl\t" << index2 << "(%rbp), %eax\n";
                o<< "\tcmpl\t" << index << "(%rbp), %eax\n";
                o<< "\tsetne\t%al\n";
                o<< "\tmovzbl\t%al, %eax\n";
                int index3 = bb->cfg->symbols->getAddress(params[0]);
                o<< "\tmovl\t%eax, " << index3 << "(%rbp)\n";
                return;
        }
        case wmem: {
                // The value of variable var is written at address addr

                // Special case : return
                if (params[0] == "!retval") {
                        std::cout << "return " << std::endl;
                        int index = bb->cfg->symbols->getAddress(params[1]);
                        o<<"\tmovl\t"<<index<<"(%rbp),%eax\n";
                        return;
                }

                // Fetch addresses from our variables names
                int var_index, addr;
                addr = this->bb->cfg->symbols->getAddress(params[0]);
                var_index= this->bb->cfg->symbols->getAddress(params[1]);

                // Debug prints
                std::cout << "addr : " << params[0] << " "<< addr <<" \n";
                std::cout << "var  : " << params[1] << " "<<var_index;
                std::cout << std::endl;

                o<<"\tmovl\t"<<var_index<<"(%rbp),%eax\n";
                o<<"\tmovl\t%eax,"<<addr<<"(%rbp)\n";
                return;
        }
        case call: {
                std::cout << "One day we will generate asm for function call" 
                        << std::endl;
                std::cout << "number of params  :" << this->params.size()-2 << std::endl;
                
                int nb_params = this->params.size() - 2;
                if (nb_params == 1) {
                        exit(EXIT_FAILURE);
                } else if (nb_params == 2) {
                        o<<"\tmovl\t$"<< this->params[2]<<",%edi\n";
                        o<<"\tmovl\t$"<< this->params[3]<<",%esi\n";
                } else {
                exit(EXIT_FAILURE);
                }
                o << "\tcall\t" << this->params[1] << "\n";
                int index3 = bb->cfg->symbols->getAddress(params[0]);
                o<<"\tmovl\t%eax," << index3 << "(%rbp)\n";
                std::cout << "Fin de la generation ir pour call" << std::endl;
                break;
        }
        default:
                std::cout<<"gen_asm not implemented"<<std::endl;
                //exit(EXIT_FAILURE);
                return;
        }
        return;
}

void CFG::gen_asm(std::ostream& o)
{
        std::cout << "call to CFG::gen_asm" << std::endl;
        std::cout << "Number of bbs : " << bbs.size() << std::endl;
        // Generate prologue
        // !!!!!!!!!!!!!!!!!!!!!!!!!
        // TODO FAIRE LE VRAI CALCUL POUR OFFSET (MIS A 256 POUR TEST)
        // !!!!!!!!!!!!!!!!!!!!!!!!!
        o <<
                ".global " << bbs.at(0)->label << "\n"
                << bbs.at(0)->label << ":\n"
                "\t# Prologue\n"
                "\tpushq\t%rbp\n"
                "\tmovq\t%rsp, %rbp\n"
                "\tsubq\t$256, %rsp"
                "\n"
                "\t# Body\n";

        // Current bb is pointing to body function
        this->current_bb->gen_asm(o);

        o <<
                "\t# Epilogue\n"
                "\tleave\n"
                "\tret\n";
}
