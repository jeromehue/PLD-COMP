#include <string>
#include <iostream>

// List of available registers
// and their names
static int freereg[5];
static std::string reglist[5] = {
    "%r8d", "%r9d", "%r10d", "%r11d", "%r12d"
};

// Set all registers as available
void freeall_registers(void) {
    freereg[0] = freereg[1] = freereg[2] = freereg[3] = 1;
}

// Allocate a free register. Return the number of
// the register. Die if no available registers.
static int alloc_register(void) {
    for (int i = 0; i < 4; i++) {
        if (freereg[i]) {
            freereg[i] = 0;
            return (i);
        }
    }
    std::cout << "Out of registers!\n"
              << std::endl;
    exit(1);
}

// Return a register to the list of available registers.
// Check to see if it's not already there.
static void free_register(int reg) {
    if (freereg[reg] != 0) {
        std::cout << "Error trying to free register %d\n"
                  << std::endl;
        exit(1);
    }
    freereg[reg] = 1;
}

void cgprologue() {
    std::cout << ".global main\n"
                 " main: \n"
                 "	#prologue\n"
                 "	pushq %rbp\n"
                 "	movq %rsp, %rbp\n"
                 "\n"
                 "	#body\n";
}

void cgepilogue() {
    std::cout << "\n"
                 "	#epilogue\n"
                 "\tpopq %rbp\n"
                 " 	ret\n";
}

void cgreturnvar(std::string reg) {
    std::cout << "	movl    "
              << reg
              << ", %eax\n";
}

void cgreturnreg(int r) {
    std::cout << "	movl    "
              << reglist[r]
              << ", %eax\n";
    free_register(r);
}

// Load a value into a register and return register
int cgloadint(int value) {
    // Get a new register
    int r = alloc_register();
    // Print out the code to initialise it
    std::cout << "\tmovl\t$" << value << ", " << reglist[r] << "\n";
    return (r);
}

int cgadd(int r1, int r2) {
    std::cout << "\taddl\t" << reglist[r1] << ", " << reglist[r2] << "\n";
    free_register(r1);
    return (r2);
}

int cgsub(int r1, int r2) {
    // r1 - r2
    std::cout << "\tsubl\t" << reglist[r2] << ", " << reglist[r1] << "\n";
    free_register(r2);
    return (r1);
}

int cggreater(int r1, int r2) {
    std::cout << "\t#cggreater\n";
    std::cout << "\tmovl    " << reglist[r1] << ", %eax\n"
              << "\tcmpl\t" << reglist[r2] << ", %eax\n"
              << "\tsetg\t%al\n"
              << "\tmovzbl\t%al, %eax\n"
              << "\tmovl\t%eax, " << reglist[r1] << "\n";
    free_register(r2);
    return (r1);
}

int cglower(int r1, int r2) {
    std::cout << "\t#cglower\n";
    std::cout << "\tmovl    " << reglist[r1] << ", %eax\n"
              << "\tcmpl    " << reglist[r2] << ", %eax\n"
              << "\tsetl\t%al\n"
              << "\tmovzbl\t%al, %eax\n"
              << "\tmovl\t%eax, " << reglist[r1] << "\n";
    free_register(r2);
    return (r1);
}

int cgmul(int r1, int r2) {
    std::cout << "\tmovl    " << reglist[r1] << ", %eax\n";
    std::cout << "\tmull\t" << reglist[r2] << "\n";
    std::cout << "\tmovl    %eax"
              << ", " << reglist[r2] << "\n";
    free_register(r1);
    return (r2);
}

int cgdiv(int r1, int r2) {
    // Division de r1 par r2
    // TODO Eviter les divisions par 0
    // Voir la doc de div
    std::cout << "\tmovl    $0, %edx\n";
    std::cout << "\tmovl    " << reglist[r1] << ", %eax\n";
    std::cout << "\tdivl\t  " << reglist[r2] << "\n";
    std::cout << "\tmovl    %eax"
              << ", " << reglist[r2] << "\n";
    free_register(r1);
    return (r2);
}

void cgloadeax(int r) {
    std::cout << "\tmovl    " << reglist[r] << ", %eax\n";
}

// load a register into a variable
void cgloadvar(int r, int addr) {
    std::string retstr = std::to_string(addr) + "(%rbp)";
    std::cout << "        movl    "
              << reglist[r]
              << ", "
              << retstr
              << "\n";
    free_register(r);
}

// load a varibale into a register and return that register
int cgvartoreg(int addr) {
    int r = alloc_register();
    std::string var_register = std::to_string(addr) + "(%rbp)";
    std::cout << "        movl    "
              << var_register
              << ", "
              << reglist[r]
              << "\n";
    return r;
}

// Store a variable
void cgstorevar(int value, int addr) {
    std::string var_register = std::to_string(addr) + "(%rbp)";
    std::cout << "\tmovl     $" << value << "," << var_register << "\n";
}
