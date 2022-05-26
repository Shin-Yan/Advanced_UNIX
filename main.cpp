#include<stdio.h>
#include<string.h>
#include<string>
#include<iostream>

#include"load.h"
#include"execmd.h"

int state = STATE_NOT_LOADED;

int main(int argc,char* argv[]){
    FILE* input = stdin;
    init_cmd();
    // std::cout<< "argc = " << argc << std::endl;
    if(argc != 1){
        if(argc == 2 ){
            std::string load_file = std::string(argv[1]);
            load_program(load_file);
        }
        else if(argc == 3 || argc == 4){
            
        }
        else
            printf("usage: ./hw4 [-s script] [program]\n");
    }   
    while(true){
        printf("sdb> ");

        std::string cmd;
        std::getline(std::cin, cmd);
        if(strcmp(cmd.c_str() , "") == 0)
            continue;
        exe_cmd(cmd);
    }
    return 0;
}