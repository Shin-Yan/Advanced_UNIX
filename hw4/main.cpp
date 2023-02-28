#include<stdio.h>
#include<string.h>
#include<string>
#include<iostream>

#include"load.h"
#include"execmd.h"

int state = STATE_NOT_LOADED;

int main(int argc,char* argv[]){
    init_cmd();
    init_program();
    std::ifstream input;
    bool script_flag = false;
    bool endfile = false;
    if(argc != 1){
        if(argc == 2 ){
            std::string load_file = std::string(argv[1]);
            if(load_program(load_file) == RETURN_EXIT)
                return 0;
        }
        else if(argc == 3 || argc == 4){
            char opt;
            while((opt = getopt(argc, argv, "-s:")) != -1){
                switch(opt){
                    case 's':
                        input.open(optarg);
                        if(!input.is_open()){
                            printf("** %s: No such file or directory\n",optarg);
                            return 0;
                        }
                        script_flag = true;
                        break;
                    default:
                        std::string path = std::string(optarg);
                        if(load_program(path) == RETURN_EXIT)
                            return 0;
                }
            }
        }
        else
            printf("usage: ./hw4 [-s script] [program]\n");
    }   
    while(true){
        
        if(!script_flag)
            printf("sdb> ");

        std::string cmd;
        if(!script_flag)
            std::getline(std::cin, cmd);
        else
            endfile = std::getline(input, cmd).eof();
        if(cmd == "")
            continue;
        int res;
        res = exe_cmd(cmd);
        if(res == RETURN_EXIT)
            return 0;
        if(endfile == true)
            return 0;
    }
    return 0;
}