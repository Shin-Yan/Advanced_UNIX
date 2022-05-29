#include"break.h"
bool addr_in_text(unsigned long long addr){
   if(state == STATE_RUNNING && loaded_program.dynamic == 1) {
        unsigned long long true_addr = loaded_program.base_addr + loaded_program.loaded_elf.virtual_address;
        return (addr >= true_addr && addr <= (true_addr + loaded_program.loaded_elf.size));
    }
    return (addr >= loaded_program.loaded_elf.virtual_address && addr < (loaded_program.loaded_elf.virtual_address + loaded_program.loaded_elf.size));
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
        vector<breakpoint>::iterator it = loaded_program.bps.begin();
        while(it != loaded_program.bps.end()){
            if(b_addr == it->address){
                printf("** the breakpoint is already exists. (breakpoint %d)\n", it->id);
                return 0;
            }
            it ++;
        }
        unsigned char replace = 0xcc;
        unsigned char origin_cmd = change_byte(b_addr, replace);
        breakpoint newbp = {(int)loaded_program.bps.size(), b_addr, origin_cmd, true};
        loaded_program.bps.push_back(newbp);
    }
    else{
        cout << "** the address is out of the range of the text segment" <<endl;
    }
    return 0;
}