#ifndef _LOAD_H
#define _LOAD_H

#include<string>
#define STATE_NONE 0x00
#define STATE_LOAD 0x01

int load_program(std::string program_name);

extern int state;
#endif 