#ifndef _CONT_H
#define _CONT_H

#include "execmd.h"
#include "register.h"
#include <iostream>
#include <string>
#include <capstone/capstone.h>

using namespace std;

void disasm_one_instruction(unsigned long long);
unsigned long long get_reg_value(string);
unsigned long long check_bp(void);
int si(string);
int cont(string);

#endif