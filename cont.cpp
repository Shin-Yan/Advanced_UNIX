#include "cont.h"

int si(string cmd){
    if(state != STATE_RUNNING){
        cout << "** state must be RUNNING" << endl;
        return 0;
    }
    ptrace(PTRACE_SINGLESTEP , loaded_program.pid , NULL , NULL);
    return 0;
}

int check_bp(){
    int status;
    waitpid(loaded_program.pid, &status, 0);
    if(WIFSTOPPED(status)){
        if(WSTOPSIG(status) != SIGTRAP){
            printf("** %d stop by signal\n", loaded_program.pid);
            return -1;
        }
    }
    return 0;
}