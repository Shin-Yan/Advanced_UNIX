#include "start.h"

// void get_base_addr(){
//     string path = "/proc/" + to_string(loaded_program.pid) + "/stat";
//     FILE* f = fopen(path.c_str(),"r");
//     if(f == NULL){
//         perror("fopen");
//         return;
//     }
//     string tmp;
//     unsigned long long base_addr;
//     fstream file_stream(path);
//     for(int i = 0 ; i < 25 ; i ++ ){
//         file_stream >> tmp;
//     }
//     file_stream >> base_addr;
//     loaded_program.base_addr = base_addr;
//     fclose(f);
// }

int start(string cmd){
    if(state != STATE_LOADED){
        cout << "** state must be LOADED" <<endl;
        return 0;
    }
    loaded_program.pid = fork();
    if(loaded_program.pid < 0){
        perror("** fork");
        return 0;
    }
    else if(loaded_program.pid == 0){ //child process
        if(ptrace(PTRACE_TRACEME, 0, NULL, NULL) < 0) {
            perror("** ptrace");
            return 0;
        }
        char * args[] = {(char*)("./"+loaded_program.program_name).c_str(), NULL};
        
        if(execvp(args[0],args) == -1){
            perror("** execvp");
            exit(0);
        }
    }
    else{ //parent process
        int status;
        waitpid(loaded_program.pid, &status , 0);
        ptrace(PTRACE_SETOPTIONS, loaded_program.pid, 0, PTRACE_O_EXITKILL);
        state = STATE_RUNNING;
        for(auto &x: loaded_program.bps){
            x.origin_command = change_byte(x.address,(unsigned char)0xcc);
        }

        cout << "** pid " << loaded_program.pid << endl;
    }
    return 0;
}