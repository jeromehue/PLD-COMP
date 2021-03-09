#pragma once

#include <string>

void freeall_registers(void);
void cgprologue();
void cgreturnvar(std::string ret);
void cgepilogue();
int cgloadint(int value);
void cgloadvar(int r, int addr);
int cgvartoreg(int addr);
int cgadd(int r1, int r2);
int cgsub(int r1, int r2);
int cgmul(int r1, int r2);
int cgdiv(int r1, int r2);
