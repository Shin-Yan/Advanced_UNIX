#include "execmd.h"

Command cmd_help = {"help","h","- help: show this message",help};
Command cmd_load = {"load","","- load {path/to/a/program}: load a program",load_program};
vector<Command> Commands;

void init_cmd(){
    Commands.push_back(cmd_help);
    Commands.push_back(cmd_load);
}

fptr find_cmd(std::string cmd){
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

int exe_cmd(std::string cmd){
    // cout << cmd << endl;
    stringstream cmds = stringstream(cmd);
    string command, arg;
    cmds >> command >> arg;
    fptr exe_cmd = find_cmd(command);
    if(exe_cmd != NULL) 
        return exe_cmd(arg);
    return 0;
}