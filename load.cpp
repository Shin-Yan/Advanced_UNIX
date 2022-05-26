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
    if(fd < 0){
        perror("fopen");
        return -1;
    }
    unsigned char elf[0x40];
    read(fd, elf, 0x40);
    // get entry point
    unsigned long long entry_point = get_Lvalue(ENTRY_POINT_SIZE,ENTRY_POINT_OFFSET,elf);
    loaded_program.loaded_elf.entry_point = entry_point;

    // get dynamic or not
    unsigned long long is_dynamic = get_Lvalue(TYPE_SIZE, TYPE_OFFSET, elf);
    cout << "is_dynamic = " << is_dynamic <<endl;
    if(is_dynamic == 0x03){
        loaded_program.dynamic = 1;
        cout << "This program is using dynamic address!" <<endl;
    }
    else
        loaded_program.dynamic = 0;

    return 0;
}

int load_program(string program_name){
    if(state != STATE_NOT_LOADED){
        printf("** state must be NOT LOADED\n");
        return 0;
    }
    if(get_elf_info(program_name)!=0){
        return 0;
    }
    
    state = STATE_LOADED;
    loaded_program.program_name = program_name; 
    printf("** program '%s' loaded. entry point 0x%-6llx\n", program_name.c_str(),loaded_program.loaded_elf.entry_point);
    return 0;
}