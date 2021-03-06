#include "dump.h"

int dump(string addr){
    unsigned long long dump_addr, offset;
    if(addr == ""){
        cout << "** no addr is given" << endl;
        return 0;
    }
    if(state != STATE_RUNNING){
        cout << "** state must be RUNNING" << endl;
        return 0;
    }
    dump_addr = strtol(addr.c_str(),NULL,16);
    if(dump_addr == 0){
        cout << "** no addr is given" << endl;
        return 0;
    }

    unsigned char dump_data[80];
    bool hit[80] = {false};
    for(auto &x: loaded_program.bps){
        offset = x.address -dump_addr;
        if(offset< 80){
            hit[offset] = true;
        }
    }
    for(int i=0 ;i<5 ;i++){
        printf("      %llx: ", dump_addr);
        for(int j=0 ; j<16 ; j++){
            if(!hit[16 * i + j]){
                dump_data[16 * i + j] = ptrace(PTRACE_PEEKDATA, loaded_program.pid, dump_addr, NULL);
                printf("%02x ", dump_data[16 * i + j]);
            }
            else
                printf("cc ");
            dump_addr ++ ;
        }
        printf("|"); 
        for(int j = 0; j < 16; j ++) {
            char c = dump_data[16 * i + j];
            if(isprint(c) != 0) {
                printf("%c", c);
            }else {
                printf(".");
            }
        }
        printf("|"); 
        printf("\n");
    }
    return 0;
}