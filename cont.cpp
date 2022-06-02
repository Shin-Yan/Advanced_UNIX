#include "cont.h"

void disasm_one_instruction(unsigned long long address){
    unsigned long long offset = address - loaded_program.asm_address;
    unsigned char* pos = (unsigned char*)loaded_program.asm_code + offset;
    csh h;
    cs_insn* insn;
    cs_open(CS_ARCH_X86,CS_MODE_64,&h);
    cs_disasm(h,pos,100,address,0,&insn);

    printf("%12lx: ", insn[0].address);
            
    for (size_t j = 0; j < 12; j++) {
        if (j < insn[0].size) {
            printf("%02hhx ", insn[0].bytes[j]);
        }else{
            printf("   ");
        }
    }
    printf("\t%s\t%s\n", insn[0].mnemonic,
                insn[0].op_str);
    cs_close(&h);
}

unsigned long long get_reg_value(string reg){
    if(ptrace(PTRACE_GETREGS, loaded_program.pid, 0, &loaded_program.regs) < 0) {
        perror("** ptrace");
        return 0;
    }
    for(int i = 0; i < reg_num; i++) {
        if(reg == REGS[i].name) {
            return REGS[i].get(&loaded_program.regs);
        }
    }
    cout << "** no such register. " << endl;
    return 0;
}

int check_bp(){
    int status;
    waitpid(loaded_program.pid, &status, 0);
    if(WIFSTOPPED(status)){
        printf("PC = 0x%llx\n",get_reg_value("RIP")); 
        if(WSTOPSIG(status) != SIGTRAP){
            printf("** %d stop by signal\n", loaded_program.pid);
            loaded_program.pid = 0;
            state = STATE_LOADED;
            return -1;
        }
        int i = 0;
        for(auto &x: loaded_program.bps){
            unsigned long long program_counter = get_reg_value("RIP");
            
            if(x.address == program_counter){
                
                cout << "** breakpoint @" ;
                disasm_one_instruction(x.address);
                change_byte(x.address, x.origin_command);
                return i;
            }
            i++;
            // else
            //     printf("PC = 0x%llx\n",program_counter); 
        }
        return -1;
    }    
    

    if(WIFEXITED(status)) {
        if (WIFSIGNALED(status))
            printf("** %d stop by signal", loaded_program.pid);
        else
           printf("** chlid process %d terminiated normally (code %d)\n", loaded_program.pid, status);
       loaded_program.pid = 0;
       state = STATE_LOADED;
       return -1;
    }
    return -1;
}

int si(string cmd){
    if(state != STATE_RUNNING){
        cout << "** state must be RUNNING" << endl;
        return 0;
    }
    if(ptrace(PTRACE_SINGLESTEP , loaded_program.pid , NULL , NULL)<0){
        perror("** ptrace");
        return 0;
    }
    int ret,i ;
    ret = check_bp();
    
    if(ret >= 0){
        i = 0;
        for(auto &x : loaded_program.bps){
            if(i == loaded_program.hit_id && i != ret){
                unsigned char origin = change_byte(x.address, 0xcc);
                x.origin_command = origin;
            }
            i++;
        }
    }
    loaded_program.hit_id = ret;
    return 0;
}

int cont(string cmd){

}