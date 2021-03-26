#pragma once

#include <string>

void freeall_registers(void);
void cgprologue();
void cgreturnvar(std::string ret);
void cgreturnreg(int r);
void cgepilogue();
int cgloadint(int value);
void cgloadvar(int r, int addr);
int cgvartoreg(int addr);
int cgadd(int r1, int r2);
int cgsub(int r1, int r2);
int cgmul(int r1, int r2);
int cgdiv(int r1, int r2);
int cgloadeax(int r1);
void cgstorevar(int value, int addr);
int cggreater(int r1,int r2);
int cglower(int r1,int r2);
