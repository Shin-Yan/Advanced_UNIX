#include "load.h"
program_info loaded_program;
unsigned long long get_Lvalue(unsigned long long size, unsigned long long offset, unsigned char* buf) { //L means little endian
    unsigned long long ret = 0;
    for(int i = size + offset - 1; i >= (int)offset; i--) {
        ret = ret * 256 + buf[i];
    }
    return ret;
}

int get_elf_info(string path){
    int fd = open(path.c_str(), O_RDONLY);
    if(fd < 0)
        return -1;
    unsigned char elf[0x40];
    read(fd, elf, 0x40);
    // get entry point
    unsigned long long entry_point = get_Lvalue(ENTRY_POINT_SIZE,ENTRY_POINT_OFFSET,elf);
    loaded_program.loaded_elf.entry_point = entry_point;

    // get 
    return 0;
}

int load_program(string program_name){
    if(state != STATE_NOT_LOADED){
        printf("** state must be NOT LOADED\n");
        return 0;
    }
    get_elf_info(program_name);
    
    state = STATE_LOADED;

    printf("** program '%s' loaded. entry point 0x%-6llx\n", program_name.c_str(),loaded_program.loaded_elf.entry_point);
    return 0;
}