#ifndef _DISASM_H
#define _DISASM_H

#define DISASM_MAX_INSTRUCTIONS 10

#include "execmd.h"
#include "break.h"
#include "cont.h"

#include<string>
#include<iostream>
using namespace std;

void disasm_all(unsigned long long);
int disasm(string);

#include <capstone/capstone.h>

#endif