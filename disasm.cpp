#include "disasm.h"

int disasm(string address){
    if(address == ""){
        cout << "** no addr is given" <<endl;
        return 0;
    }
    if(state != STATE_RUNNING){
        cout << "** state must be RUNNING" <<endl;
        return 0;
    }
    unsigned long long disasm_address = strtol(address.c_str(),NULL,16);
    if(disasm_address == 0){
        cout << "** no addr is given" <<endl;
        return 0;
    } 
    if((disasm_address < loaded_program.loaded_elf.entry_point) ||
    (disasm_address >= loaded_program.loaded_elf.entry_point + loaded_program.loaded_elf.size)){
        cout << "** the address is out of the range of the text segment" << endl;
        return 0;
    }
    return 0;
}
