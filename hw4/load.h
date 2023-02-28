#ifndef _LOAD_H
#define _LOAD_H

#include<string>
#include<string.h>
#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<iostream>
#include"execmd.h"
using namespace std;

#define ENTRY_POINT_OFFSET 0x18
#define ENTRY_POINT_SIZE 0x8
#define TYPE_OFFSET 0x10
#define TYPE_SIZE 0x02
#define E_SHOFF_OFFSET 0x28
#define E_SHOFF_SIZE 0x8
#define E_SHENTSIZE_OFFSET 0x3A
#define E_SHENTSIZE_SIZE 0x2
#define E_SHNUM_OFFSET 0x3C
#define E_SHNUM_SIZE 0x2
#define E_SHSTRNDX_OFFSET 0x3E
#define E_SHSTRNDX_SIZE 0x2
#define SH_OFFSET_OFFSET 0x18
#define SH_OFFSET_SIZE 0x8
#define SH_SIZE_OFFSET 0x20
#define SH_SIZE_SIZE 0x8
#define SH_NAME_OFFSET 0x0
#define SH_NAME_SIZE 0x4
#define SH_ADDRESS_OFFSET 0x10
#define SH_ADDRESS_SIZE 0x08

unsigned long long get_Lvalue(unsigned long long, unsigned long long, unsigned char*);
int get_elf_info(string);
int load_program(string);

extern int state;

#endif 