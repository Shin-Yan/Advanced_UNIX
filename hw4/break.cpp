#include"break.h"

bool addr_in_text(unsigned long long addr){
    return (addr >= loaded_program.loaded_elf.entry_point && addr < (loaded_program.loaded_elf.entry_point + loaded_program.loaded_elf.size));
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
        breakpoint newbp = {b_addr};
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
        loaded_program.bps.erase(loaded_program.bps.begin() + id);
        printf("** breakpoint %d deleted.\n", id);
    }
    else
        printf("** no breakpoint number %d.\n", id);
    return 0;
}