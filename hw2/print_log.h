#ifndef _PRINT_LOG_H
#define _PRINT_LOG_H

#include <stdio.h>
#include <sys/types.h>
#include <limits.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <string.h>
#include <string>
#include <iostream>

void get_filename(const char*,char* buf);
void get_file_pointer(FILE*, char*);
void get_fd(char*, int);

std::string get_checked_string(char* str,int);

void print_chmod(const char*, mode_t, int);
void print_chown(const char*,  uid_t, gid_t, int);
void print_close(char*, int);
void print_creat(const char*, mode_t , int);
void print_fclose(char* , int);
void print_fopen(const char*, const char*, FILE*);
void print_fread(char*, size_t, size_t, FILE*, size_t);
void print_fwrite(char*, size_t, size_t, FILE* , size_t);
void print_open(const char*, int, mode_t, int);
void print_read(int, void*, size_t, ssize_t);
void print_remove(const char*, int);
void print_rename(const char*, const char*, int);
void print_tmpfile(FILE*);
void print_write(int, const void*, size_t, ssize_t);

extern FILE* output_file;
extern int output_file_fd;
// extern std::string real_path;
#endif