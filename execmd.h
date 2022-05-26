#ifndef _EXECMD_H
#define _EXECMD_H

#include<string>
#include<vector>

#include "help.h"

#define STATE_NOT_LOADED 1
#define STATE_LOADED 2
#define STATE_RUNNING 3

using namespace std;
#include<iostream>

int exe_cmd(string cmd);
void init_cmd();
typedef int (*fptr)(string);

typedef struct Command{
    string cmd_name;
    string cmd_shortname;
    string help_msg;
    fptr cmd_fptr;
} Command;

extern vector<Command> Commands;
#endif