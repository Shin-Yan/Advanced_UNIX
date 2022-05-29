#ifndef _BREAK_H
#define _BREAK_H

#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include "execmd.h"
using namespace std;

typedef struct breakpoint{
    int id;
    unsigned long long address;
    unsigned char origin_command;
    bool fix_flag;
}breakpoint;

bool addr_in_text(unsigned long long);
unsigned char change_byte(unsigned long long, unsigned char);
int set_breakpoint(string);

#endif