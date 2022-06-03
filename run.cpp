#include "run.h"

int run(string cmd){
    if(state == STATE_RUNNING){
        printf("** program %s is already running.\n", loaded_program.program_name.c_str());
    }
    else if(state == STATE_LOADED){
        start("");
    }
    else {
        printf("** state must be LOADED or RUNNING.\n");
        return 0;
    }
    cont("");
    return 0;
}