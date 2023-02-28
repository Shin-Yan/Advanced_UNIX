#include "execmd.h"

Command cmd_break = {"break","b","- break {instruction-address}: add a break point",(fptr)&set_breakpoint};
Command cmd_cont = {"cont","c","- cont: continue execution",(fptr)&cont};
Command cmd_del = {"delete","","- delete {break-point-id}: remove a break point",(fptr)&del};
Command cmd_disasm = {"disasm","d","- disasm addr: disassemble instructions in a file or a memory region",(fptr)&disasm};
Command cmd_dump = {"dump","x","- dump addr: dump memory content", (fptr)&dump};
Command cmd_exit = {"exit","q","- exit: terminate the debugger",(fptr)&quit};
Command cmd_getreg = {"get", "g" , "- get reg: get a single value from a register", (fptr)&getreg};
Command cmd_getregs = {"getregs", "" , "- getregs: show registers", (fptr)&get_all_regs};
Command cmd_help = {"help","h","- help: show this message",(fptr)&help};
Command cmd_list = {"list","l","- list: list break points",(fptr)&list};
Command cmd_load = {"load","","- load {path/to/a/program}: load a program",(fptr)&load_program};
Command cmd_run = {"run","r","- run: run the program",(fptr)&run};
Command cmd_vmmap = {"vmmap","m","- vmmap: show memory layout",(fptr)&vmmap};
Command cmd_set = {"set" ,"s" , "- set reg val: get a single value to a register", (fptr)&setreg};
Command cmd_si = {"si","","- si: step into instruction",(fptr)&si};
Command cmd_start = {"start","","- start: start the program and stop at the first instruction", (fptr)&start};
vector<Command> Commands;

void init_cmd(){
    Commands.push_back(cmd_break);
    Commands.push_back(cmd_cont);
    Commands.push_back(cmd_del);
    Commands.push_back(cmd_disasm);
    Commands.push_back(cmd_dump);
    Commands.push_back(cmd_exit);
    Commands.push_back(cmd_getreg);
    Commands.push_back(cmd_getregs);
    Commands.push_back(cmd_help);
    Commands.push_back(cmd_list);
    Commands.push_back(cmd_load);
    Commands.push_back(cmd_run);
    Commands.push_back(cmd_vmmap);
    Commands.push_back(cmd_set);
    Commands.push_back(cmd_si);
    Commands.push_back(cmd_start);
}

void init_program(){
    loaded_program.pid = 0;
}

fptr find_cmd(string cmd){
    
    for(auto &x: Commands){
        if(x.cmd_name == cmd)
            return x.cmd_fptr;
        if(x.cmd_shortname == cmd && cmd != "")
            return x.cmd_fptr;
    }
    
    return NULL;
}

int exe_cmd(string cmd){
    int ret = 0;
    stringstream cmds = stringstream(cmd);
    string command, arg1, arg2;
    cmds >> command >> arg1 >> arg2;
    fptr exe_cmd = find_cmd(command);
    if((command == "set" || command =="s") && exe_cmd != NULL)
        ret = ((fptr2)exe_cmd)(arg1,arg2);
    else if(exe_cmd != NULL) 
        ret = ((fptr1)exe_cmd)(arg1);
    fflush(stdout);
    return ret;
}

int quit(string cmd){
    if(loaded_program.pid != 0){
        kill(loaded_program.pid, SIGTERM);
    }
    free(loaded_program.asm_code);
    return RETURN_EXIT;
}