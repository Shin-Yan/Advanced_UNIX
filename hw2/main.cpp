#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/wait.h>

char *exe_arg[50];
int main(int argc,char* argv[]){
    // for(int i=0;i<argc;i++){
    //     std::cout<<"!"<<argv[i]<<std::endl;
    // }
    std::string output_file ="", ld_preload = "./logger.so";
    
    int exe_index=0 , output_file_flag = 0;
    if(argc<2){
        std::cout<< "no command given"<<std::endl;
        return 0;
    }
    char opt;
    while((opt = getopt(argc,argv,"o:p:")) != -1){
        switch(opt){
            case 'o':
                output_file = std::string(optarg);
                output_file_flag = 1;
                // std::cout<<"output file = "<<output_file<<std::endl;
                break;
            case 'p':
                ld_preload = std::string(optarg);
                break;
            default:
                std::cout<<"usage: ./logger [-o file] [-p sopath] [--] cmd [cmd args ...]"<<std::endl;
                std::cout<<"-p: set the path to logger.so, default = ./logger.so"<<std::endl;
                std::cout<<"-o: print output to file, print to \"stderr\" if no file specified"<<std::endl;
                std::cout<<"--: separate the arguments for logger and for the command"<<std::endl;
                return 0;
                break;
        }
    }
    // for(int i=0;i<argc+1;i++){
    //     std::cout<<argv[i]<<std::endl;
    // }
    for(int i = optind; i < argc; i++) {
        exe_arg[exe_index++] = argv[i];
        // printf("%s\n",exe_arg[exe_index-1]);
    }
    // printf("%s\n",argv[optind]);
    pid_t pid = fork();
    if(pid == 0) {
        char t[10];
        if(output_file_flag == 1) {
            int fd = open(output_file.c_str(), O_RDWR | O_CREAT | O_TRUNC, S_IRWXU);
            snprintf(t, 10, "%d", fd);
            setenv("output_file", t, 1);
        }
        setenv("LD_PRELOAD", ld_preload.c_str(), 1);
        if(execvp(argv[optind], exe_arg) == -1) {
            perror("exec error!\n");
            exit(0);
        }
    }else if(pid > 0) {
        while(waitpid(pid, NULL, WNOHANG) != -1);
    } 
}