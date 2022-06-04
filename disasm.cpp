#include "disasm.h"

void disasm_all(unsigned long long address){
    unsigned long long offset = address - loaded_program.asm_address;
    unsigned char* pos = (unsigned char*)loaded_program.asm_code + offset;
    csh h;
    cs_insn* insn;
    cs_open(CS_ARCH_X86,CS_MODE_64,&h);
    if(cs_disasm(h,pos,100,address,0,&insn) >0){
        for(int i = 0 ; i < DISASM_MAX_INSTRUCTIONS; i++){
            if(!addr_in_text(insn[i].address)){
                cout << "** the address is out of the range of the text segment" << endl;
                cs_close(&h);
                return;
            }
            printf("%12lx: ", insn[i].address);
            for (size_t j = 0; j < 12; j++) {
                if (j < insn[i].size) {
                    printf("%02hhx ", insn[i].bytes[j]);
                }
                else{
                    printf("   ");
                }
            }   
            printf("\t%s\t%s\n", insn[i].mnemonic,insn[i].op_str);
        }
    }
    cs_close(&h);
    return;
}

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
    if(!addr_in_text(disasm_address)){
        cout << "** the address is out of the range of the text segment" << endl;
        return 0;
    }
    disasm_all(disasm_address);
    return 0;
}
