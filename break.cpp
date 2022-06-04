#include"break.h"

bool addr_in_text(unsigned long long addr){
    return (addr >= loaded_program.loaded_elf.entry_point && addr < (loaded_program.loaded_elf.entry_point + loaded_program.loaded_elf.size));
}

unsigned char change_byte(unsigned long long address, unsigned char cmd) {
    auto code = ptrace(PTRACE_PEEKTEXT, loaded_program.pid, address, NULL);
    ptrace(PTRACE_POKETEXT, loaded_program.pid, address, (code & 0xffffffffffffff00) | (cmd & 0xff));
    return code & 0xff;
}

int set_breakpoint(string addr){
    if(addr == ""){
        cout << "** no address is given" << endl;
        return 0;
    }
    unsigned long long b_addr = strtol(addr.c_str(),NULL,16);
    if(state != STATE_RUNNING){
        cout << "** state must be RUNNING" << endl;
        return 0;
    }
    if(addr_in_text(b_addr)){
        
        int i = 0;
        for(auto &x: loaded_program.bps){
            if(b_addr == x.address){
                printf("** the breakpoint is already exists. (breakpoint %d)\n", i);
                return 0;
            }
            i++;
        }
        unsigned char replace = 0xcc;
        unsigned char origin_cmd = change_byte(b_addr, replace);
        breakpoint newbp = {b_addr, origin_cmd};
        loaded_program.bps.push_back(newbp);        
    }
    else{
        cout << "** the address is out of the range of the text segment" <<endl;
    }
    return 0;
}

int list(string cmd){
    
    int i = 0 ;
    for(auto &x: loaded_program.bps){
        printf("  %d: %llx\n",i,x.address);
        i++;
    }
    return 0;
}

int del(string breakpoint_id) {
    if(state != STATE_RUNNING){
        cout << "** state must be RUNNING" << endl;
    }
    int id = strtol(breakpoint_id.c_str(), NULL, 10);
    if(id <= (int)loaded_program.bps.size()-1){
        change_byte(loaded_program.bps[id].address, loaded_program.bps[id].origin_command);
        loaded_program.bps.erase(loaded_program.bps.begin() + id);
        printf("** breakpoint %d deleted.\n", id);
    }
    else
        printf("** no breakpoint number %d.\n", id);
    return 0;
}