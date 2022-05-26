#ifndef _LOAD_H
#define _LOAD_H

#include<string>
#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<iostream>
#include"execmd.h"
using namespace std;

#define ENTRY_POINT_OFFSET 0x18
#define ENTRY_POINT_SIZE 0x8

int load_program(std::string program_name);

extern int state;

#endif 