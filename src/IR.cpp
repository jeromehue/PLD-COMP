#include "IR.h"
#include <iostream>

void IRInstr::gen_asm(std::ostream &o) {

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
            case wmem: {
                //the value of variable var is written at address addr

                // Special case : return
                if ( params[0] == "!retval" ) {
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
            default:
                std::cout<<"gen_asm not implemented"<<std::endl;
                //exit(EXIT_FAILURE);
                return;
        
        }

    return;
}