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
        perror("** fopen");
        return RETURN_EXIT;
    }
    unsigned char elf[0x40];
    read(fd, elf, 0x40);
    // get entry point
    unsigned long long entry_point = get_Lvalue(ENTRY_POINT_SIZE,ENTRY_POINT_OFFSET,elf);
    loaded_program.loaded_elf.entry_point = entry_point;

    //get sh information
    unsigned long long shoff = get_Lvalue(E_SHOFF_SIZE,E_SHOFF_OFFSET,elf);
    unsigned long long shentsize = get_Lvalue(E_SHENTSIZE_SIZE, E_SHENTSIZE_OFFSET, elf);
    unsigned long long shnum = get_Lvalue(E_SHNUM_SIZE, E_SHNUM_OFFSET, elf);
    unsigned long long shstrndx = get_Lvalue(E_SHSTRNDX_SIZE,E_SHSTRNDX_OFFSET,elf);

    unsigned char section_data[shentsize * shnum];
    unsigned char * sec_ptr = section_data;
    lseek(fd, shoff, SEEK_SET);
    read(fd, section_data, shentsize * shnum);

    unsigned char * name_str_ptr = section_data + shstrndx * shentsize;
    unsigned long long sh_name_offset = get_Lvalue(SH_OFFSET_SIZE, SH_OFFSET_OFFSET, name_str_ptr);
    unsigned long long sh_name_size = get_Lvalue(SH_SIZE_SIZE, SH_SIZE_OFFSET, name_str_ptr);

    char name[sh_name_size];
    lseek(fd, sh_name_offset, SEEK_SET);
    read(fd, name, sh_name_size);

    for(unsigned long long i = 0 ; i < shnum ; i++){
        unsigned long long sh_name = get_Lvalue(SH_NAME_SIZE, SH_NAME_OFFSET, sec_ptr);
        
        if(strncmp(name + sh_name,".text",5) == 0){
            unsigned long long sh_offset = get_Lvalue(SH_OFFSET_SIZE, SH_OFFSET_OFFSET, sec_ptr);
            unsigned long long sh_size = get_Lvalue(SH_SIZE_SIZE, SH_SIZE_OFFSET, sec_ptr);
            loaded_program.loaded_elf.offset = sh_offset;
            loaded_program.loaded_elf.size = sh_size;
            loaded_program.asm_address = get_Lvalue(SH_ADDRESS_SIZE, SH_ADDRESS_OFFSET, sec_ptr);
            lseek(fd, sh_offset, SEEK_SET);
            char* code = (char*)malloc(sh_size);
            loaded_program.asm_code = code;
            read(fd, loaded_program.asm_code, sh_size);
        }
        sec_ptr+= shentsize;
    }
    close(fd);
    return 0;
}

int load_program(string program_name){
    if(state != STATE_NOT_LOADED){
        printf("** state must be NOT LOADED\n");
        return 0;
    }
    if(get_elf_info(program_name)!=0){
        return RETURN_EXIT;
    }
    init_program();
    state = STATE_LOADED;
    loaded_program.program_name = program_name; 
    printf("** program '%s' loaded. entry point 0x%-6llx\n", program_name.c_str(),loaded_program.loaded_elf.entry_point);
    return 0;
}