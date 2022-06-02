#ifndef _EXECMD_H
#define _EXECMD_H

#include<string>
#include<vector>
#include<sstream>

#include "help.h"
#include "load.h"
#include "start.h"
#include "vmmap.h"
#include "dump.h"
#include "break.h"
#include "register.h"
#include "cont.h"

#define STATE_NOT_LOADED 1
#define STATE_LOADED 2
#define STATE_RUNNING 3

#define RETURN_EXIT 1
using namespace std;
#include<iostream>

typedef void (*fptr)(void);
typedef int (*fptr1)(string);
typedef int (*fptr2)(string,string);

void init_cmd();
fptr find_cmd(string);
int exe_cmd(string);
int quit(string);

typedef struct Command{
    string cmd_name;
    string cmd_shortname;
    string help_msg;
    fptr cmd_fptr;
} Command;

typedef struct elf_header{
    unsigned long long entry_point;
    unsigned long long virtual_address;
    unsigned long long offset;
    size_t size;
} elf_header;

typedef struct program_info{
    string program_name;
    pid_t pid;
    // int dynamic;
    elf_header loaded_elf;
    // unsigned long long base_addr;
    vector<struct breakpoint> bps;
    struct user_regs_struct regs;
    unsigned long long asm_address;
    char* asm_code;
    int hit_id;
} program_info;

extern vector<Command> Commands;
extern program_info loaded_program;
// extern long long int entry_point;
#endif