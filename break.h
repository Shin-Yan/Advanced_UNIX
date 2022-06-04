#ifndef _BREAK_H
#define _BREAK_H

#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include "execmd.h"
using namespace std;

typedef struct breakpoint{
    unsigned long long address;
}breakpoint;

bool addr_in_text(unsigned long long);
int set_breakpoint(string);
int list(string);
int del(string);

#endif