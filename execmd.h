#ifndef _EXECMD_H
#define _EXECMD_H

#include<string>
#include<vector>
#include<sstream>

#include "help.h"
#include "load.h"
#include "start.h"

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

typedef struct elf_header{
    unsigned long long entry_point;
    unsigned long long virtula_address;
    unsigned long long offset;
    size_t size;
} elf_header;

typedef struct program_info{
    string program_name;
    pid_t pid;
    int dynamic;
    elf_header loaded_elf;
    unsigned long long base_addr;
} program_info;

extern vector<Command> Commands;
extern program_info loaded_program;
// extern long long int entry_point;
#endif