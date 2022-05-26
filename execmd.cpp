#include "execmd.h"

Command cmd_help = {"help","h","- help: show this message",help};
vector<Command> Commands;

void init_cmd(){
    Commands.push_back(cmd_help);
}

fptr find_cmd(std::string cmd){
    vector<Command>::iterator it = Commands.begin();
    for(; it != Commands.end(); it++){
        if(it->cmd_name == cmd)
            return it->cmd_fptr;
        if(it->cmd_shortname == cmd)
            return it->cmd_fptr;
    }
    // cout << "return NULL!!!"<<endl;
    return NULL;
}

int exe_cmd(std::string cmd){
    // cout << cmd << endl;
    fptr exe_cmd = find_cmd(cmd);
    if(exe_cmd != NULL) 
        return exe_cmd(cmd);
    return 0;
}