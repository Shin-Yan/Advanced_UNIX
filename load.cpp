#include "load.h"

int load_program(std::string program_name){
    if(state != STATE_NONE){
        printf("** state must be NOT LOADED\n");
        return 0;
    }
    state = STATE_LOAD;

    printf("** program '%s' loaded. entry point ?\n", program_name.c_str());
    return 0;
}