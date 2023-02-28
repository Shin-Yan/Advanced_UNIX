#include "print_log.h"
/*#define BUFFER_SIZE 80000*/

void get_filename(const char* path, char* buf) {
	realpath(path, buf);
}

void get_file_pointer(FILE* fptr, char* buf) {
    int f = fileno(fptr);
    get_fd(buf, f);
}

void get_fd(char* buf, int fd) {
    char path[500], buf2[500];
    memset(buf2, '\0', 500);
    memset(buf, '\0', 500);
    memset(path, '\0', 500);
    snprintf(path, sizeof(path),"/proc/%d/fd/%d", getpid(), fd);
    readlink(path, buf2, 500);
    snprintf(buf, 500, "%s", buf2);
    buf[500] = '\0';
}

std::string get_checked_string(char* str,int count) {
    std::string ret="";
    if(count>32)
        count = 32;
    for(int i = 0; i < count; i++){
        if(!isprint(str[i])){
            ret += ".";
        }
        else{
            ret += str[i];
        } 
    }
    return ret;
}

void print_chmod(const char* path, mode_t mode, int ret) {
    char buf[500];
    get_filename(path,buf);
    dprintf(output_file_fd, "[logger] chmod(\"%s\", %o) = %d\n", path, mode, ret);
}

void print_chown(const char* path, uid_t o, gid_t g, int ret){
    char buf[500];
    get_filename(path,buf);
    dprintf(output_file_fd, "[logger] chown(\"%s\", %o, %o) = %d\n", path, o, g, ret);
}

void print_close(char* buf, int re) {
    dprintf(output_file_fd, "[logger] close(\"%s\") = %d\n", buf, re);
}

void print_creat(const char* path, mode_t mode, int re) {
    char buf[500];
    get_filename(path,buf);
    dprintf(output_file_fd, "[logger] creat(\"%s\", %o) = %d\n", buf, mode, re);
}

void print_fclose(char* buf, int re) {
    dprintf(output_file_fd, "[logger] fclose(\"%s\") = %d\n", buf, re);
}

void print_fopen(const char* path, const char* mode, FILE* ret) {
    char buf[500];
    get_filename(path,buf);
    dprintf(output_file_fd, "[logger] fopen(\"%s\", \"%s\") = %p\n", path, mode, ret);
}

void print_fread(char* buf, size_t s, size_t n, FILE* fp, size_t re) {
    char buf2[500];
    get_file_pointer(fp, buf2);
    
    std::string path_ptr;
    path_ptr = get_checked_string(buf,strlen(buf));
    dprintf(output_file_fd, "[logger] fread(\"%s\", %lu, %lu, \"%s\") = %lu\n", path_ptr.c_str(),s,n,buf2,re);
}

void print_fwrite(char* buf, size_t s, size_t n, FILE* fp, size_t re) {
    char buf2[500];
    get_file_pointer(fp, buf2);
    
    std::string path_ptr;
    path_ptr = get_checked_string((char*)buf, n);
    dprintf(output_file_fd, "[logger] fwrite(\"%s\", %lu, %lu, \"%s\") = %lu\n", path_ptr.c_str(),s,n,buf2,re);
}

void print_open(const char* path, int flag, mode_t mode, int re){
    char buf[500];
    get_filename(path,buf);
    dprintf(output_file_fd, "[logger] open(\"%s\", %o, %o) = %d\n", buf, flag, mode, re);
}

void print_read(int fd, void* buf, size_t count, ssize_t re) {
    char buf2[500];
    std::string readInfo;
    get_fd(buf2, fd);
    readInfo = get_checked_string((char*)buf,strlen((char*)buf));
    
    dprintf(output_file_fd, "[logger] read(\"%s\", \"%s\", %lu) = %ld\n", buf2,readInfo.c_str(),count,re);
}

void print_remove(const char* buf, int re) {
    dprintf(output_file_fd, "[logger] remove(\"%s\") = %d\n", buf, re);
}

void print_rename(const char *old, const char *n, int re) {
    dprintf(output_file_fd, "[logger] rename(\"%s\", \"%s\") = %d\n", old, n, re);
}

void print_tmpfile(FILE* re) {
    dprintf(output_file_fd, "[logger] tmpfile() = %p\n", re);    
}

void print_write(int fd, const void* buf, size_t count, ssize_t re) {
    char buf2[500];
    std::string writeInfo;
    get_fd(buf2, fd);
 
    /*fprintf(output_file, "[logger] read(\%s\", \"%s\", %lu) = %ld\n", buf2, (char*)buf, count, re);*/
    writeInfo = get_checked_string((char*)buf, strlen((char*)buf));
    dprintf(output_file_fd, "[logger] write(\"%s\", \"%s\", %lu) = %ld\n", buf2, writeInfo.c_str(),count,re);
}