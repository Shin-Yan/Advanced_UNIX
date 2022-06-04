#include "register.h"

struct REG REGS[] ={
    {"RAX", get_rax, set_rax},
    {"RBX", get_rbx, set_rbx},
    {"RCX", get_rcx, set_rcx},
    {"RDX", get_rdx, set_rdx},
    {"R8", get_r8,  set_r8},
    {"R9", get_r9,  set_r9},
    {"R10", get_r10, set_r10},
    {"R11", get_r11, set_r11},
    {"R12", get_r12, set_r12},
    {"R13", get_r13, set_r13},
    {"R14", get_r14, set_r14},
    {"R15", get_r15, set_r15},
    {"RDI", get_rdi, set_rdi},
    {"RSI", get_rsi, set_rsi},
    {"RBP", get_rbp, set_rbp},
    {"RSP", get_rsp, set_rsp},
    {"RIP", get_rip, set_rip},
    {"FLAGS", get_eflags, set_eflags}
};

int reg_num = (sizeof(REGS)/sizeof(REG));
string tolower(string register_name){
    std::for_each(register_name.begin(), register_name.end(), [](char & c){
        c = ::tolower(c);
    });
    return register_name;
}

int getreg(string reg){
    if(state != STATE_RUNNING){
        cout << "** state must be RUNNING" <<endl;
        return 0;
    }
    if(ptrace(PTRACE_GETREGS, loaded_program.pid, 0, &loaded_program.regs) < 0) {
        perror("** ptrace");
        return 0;
    }
    for(int i = 0 ; i < reg_num ; i++){
        string tmp = tolower(REGS[i].name);
        if( tmp == reg){
            unsigned long long target_addr = REGS[i].get(&loaded_program.regs);
            printf("%s = %lld (0x%llx)\n",tmp.c_str(),target_addr, target_addr);
            return 0;
        }
    }
    return 0;
}

int get_all_regs(string cmd){
    if(state != STATE_RUNNING){
        cout << "** state must be RUNNING" << endl;
        return 0;
    }
    if(ptrace(PTRACE_GETREGS, loaded_program.pid, 0, &loaded_program.regs) < 0) {
        printf("** getreg error: ptrace error\n");
        return 0;
    }
    for(int i = 0 ; i < reg_num ; i++){
        unsigned long long target_addr = REGS[i].get(&loaded_program.regs);
        if(i == 17)
            printf("%s %016llx\n",REGS[i].name.c_str(), target_addr);
        else
            printf("%-3s %-15llx",REGS[i].name.c_str(), target_addr);
        if(i%4 == 3) printf("\n");
    }
    return 0;
}

int setreg(string reg, string value){
    if(state != STATE_RUNNING){
        cout << "** state must be RUNNING" << endl;
        return 0;
    }
    unsigned long long reg_value = strtol(value.c_str(),NULL,16);
    for(int i = 0 ; i < reg_num ; i ++ ){
        if(tolower(REGS[i].name) == reg){
            REGS[i].set(&loaded_program.regs, reg_value);
            ptrace(PTRACE_SETREGS, loaded_program.pid , NULL , &loaded_program.regs);
            return 0;
        }
    }
    return 0;
}