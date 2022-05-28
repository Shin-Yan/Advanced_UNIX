#include "execmd.h"

Command cmd_exit = {"exit","q","- exit: terminate the debugger",quit};
Command cmd_help = {"help","h","- help: show this message",help};
Command cmd_load = {"load","","- load {path/to/a/program}: load a program",load_program};
Command cmd_vmmap = {"vmmap","m","- vmmap: show memory layout",vmmap};
Command cmd_start = {"start","","- start: start the program and stop at the first instruction", start};
vector<Command> Commands;

void init_cmd(){
    Commands.push_back(cmd_exit);
    Commands.push_back(cmd_help);
    Commands.push_back(cmd_load);
    Commands.push_back(cmd_vmmap);
    Commands.push_back(cmd_start);
}

fptr find_cmd(string cmd){
    vector<Command>::iterator it = Commands.begin();
    for(; it != Commands.end(); it++){
        if(it->cmd_name == cmd)
            return it->cmd_fptr;
        if(it->cmd_shortname == cmd && cmd != "")
            return it->cmd_fptr;
    }
    // cout << "return NULL!!!"<<endl;
    return NULL;
}

int exe_cmd(string cmd){
    // cout << cmd << endl;
    stringstream cmds = stringstream(cmd);
    string command, arg;
    cmds >> command >> arg;
    fptr exe_cmd = find_cmd(command);
    if(exe_cmd != NULL) 
        return exe_cmd(arg);
    return 0;
}

int quit(string cmd){
    if(loaded_program.pid != 0){
        kill(loaded_program.pid, SIGTERM);
    }
    return RETURN_EXIT;
}