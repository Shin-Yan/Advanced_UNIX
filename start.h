#ifndef _START_H
#define _START_H

#include <sys/ptrace.h>
#include <unistd.h>
#include <sys/wait.h>
#include<iostream>
#include<string>
#include<fstream>
#include "execmd.h"

using namespace std;
void get_base_addr();
int start(string);

#endif