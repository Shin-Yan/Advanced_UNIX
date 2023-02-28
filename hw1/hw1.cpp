#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <pwd.h>
#include <fstream>
#include <string>
#include <regex.h>
#include <sstream>
#include <map>

// #define USER_LENGTH 100
// #define PATH_LENGTH 100
bool cmd_flag = false;
bool type_flag = false;
bool file_flag = false;
std::string cmd_arg,type_arg,file_arg;

struct proc_inf{
    std::string pid;
    std::string user;
    std::string path;
    std::string cmd;
};

bool is_digit(char* str){
    for(int i = 0; i < int(strlen(str)); i++)
        if(!isdigit(str[i]))
            return false;
    return true;
}
void get_user(struct proc_inf *inf){
    struct stat pid_stat;
    struct passwd *pwd;
    
    if(!stat(inf->path.c_str(), &pid_stat)){
        pwd = getpwuid(pid_stat.st_uid);
        if(pwd){
            inf->user = inf->user+pwd->pw_name;          
        }
    }
}
void get_cmd(struct proc_inf * inf){
    std::string path;
    std::string line;
    path = inf->path + "comm";
    std::ifstream comm_file(path);
    if(comm_file.is_open()){
        getline(comm_file,line);
    }
    inf->cmd = line;
    comm_file.close();
}
ino_t get_inode(std::string path){
    struct stat s;
    if(stat(path.c_str(), &s) != 0)
        return 0;
    return s.st_ino;
}

int is_dir(std::string path){
    struct stat s;
    if(stat(path.c_str(), &s) != 0)
        return 0;
    return S_ISDIR(s.st_mode);
}

int is_chr(std::string path){
    struct stat s;
    if(stat(path.c_str(), &s) != 0)
        return 0;
    return S_ISCHR(s.st_mode);
}
int is_fifo(std::string path){
    struct stat s;
    if(stat(path.c_str(), &s) != 0)
        return 0;
    return S_ISFIFO(s.st_mode);
}
int is_sock(std::string path){
    struct stat s;
    if(stat(path.c_str(), &s) != 0)
        return 0;
    return S_ISSOCK(s.st_mode);
}
int is_reg(std::string path){
    struct stat s;
    if(stat(path.c_str(), &s) != 0)
        return 0;
    return S_ISREG(s.st_mode);
}

std::string get_type(std::string path){
    if(is_dir(path))
        return "DIR";
    else if(is_reg(path))
        return "REG";
    else if(is_chr(path))
        return "CHR";
    else if(is_fifo(path))
        return "FIFO";
    else if(is_sock(path))
        return "SOCK";
    else 
        return "unknown";
}

std::string get_fd(std::string path, std::string file_name){
    struct stat s;
    if(lstat(path.c_str(), &s) == -1)
        return " ";
    if((s.st_mode & S_IREAD) && (s.st_mode & S_IWRITE))
        return file_name + "u";
    else if(s.st_mode & S_IRUSR)
        return file_name + "r";
    else if(s.st_mode & S_IWUSR)
        return file_name + "w";
    else
        return " ";
}
bool is_regex(const char* expression, const char* string ){
    regex_t compiled_exp;
    regcomp(&compiled_exp,expression,0);
    if(regexec(&compiled_exp,string,0,NULL,0) == 0)
        return true;
    else
        return false;

}

bool check_del(std::string name){
    if(is_regex("deleted", name.c_str())){
         return true;
    }
    return false;
}
void print_crt(std::string FD,proc_inf* inf){
    // std::cout<<type<<std::endl;
    std::string path = inf->path + FD;
    char link[PATH_MAX];
    ssize_t link_size;
    std::string name, type, inode, FD_type;
    if((link_size = readlink(path.c_str(),link,PATH_MAX-1))<0){
        name = path +" (Permission denied)";
        type = "unknown";
        inode = " ";
    }
    else{
		link[link_size] ='\0';
        name = std::string(link);
        type = get_type(path);
        inode = std::to_string(get_inode(path));
    }
    if(check_del(name))
        name = name.substr(0,name.length()-10); 
    if(FD == "root")
        FD_type = "rtd";
    else if (FD == "exe")
        FD_type = "txt";
    else
        FD_type = FD;
    if(type_flag == true){
        if(type != type_arg)
            return;
    }
                
    if(file_flag == true){
        if(!is_regex(file_arg.c_str(), name.c_str()))
            return;
    }

    std::cout<<inf->cmd<<" "<<inf->pid<<" "<<inf->user<<" "<<FD_type<<" "<<type<<" "<<inode<<" "<<name<<std::endl;
}

void print_mD(proc_inf* inf){
    std::string maps_file = inf->path + "maps";
    std::ifstream maps_stream(maps_file);
    std::string a,line,node,name,deleted,fd,type;
    int line_count = 0;
	std::map <std::string, int> inode_map;
    if(maps_stream.is_open()){
        while(getline(maps_stream,line)){
            std::istringstream istr(line);
            istr>>a>>a>>a>>a>>node>>name>>deleted;
			if(inode_map.find(node) != inode_map.end() || atoi(node.c_str()) == 0)
                continue;
            inode_map[node] = 1; 
			if(deleted == "(deleted)"){
            	fd = "DEL";
				type = get_type(name);
			}
			else{
				fd = "mem";
				type = get_type(name);
			}
            if(type_flag == true){
                if(type != type_arg)
                    continue;
            }
                
            if(file_flag == true){
                if(!is_regex(file_arg.c_str(), name.c_str()))
                    continue;
            }
            line_count++;
            if(line_count != 1)
			    std::cout<<inf->cmd<<" "<<inf->pid<<" "<<inf->user<<" "<<fd<<" "<<type<<" "<<node<<" "<<name<<std::endl;
        }
        maps_stream.close();
    }
}
void print_FD(proc_inf* inf){
    std::string fd_path = inf->path + "fd";
    std::string name,fd,type,node,tmp_path;
    char link[100];
    ssize_t link_size;
    DIR* dir = opendir(fd_path.c_str());
    if(!dir){
        name = fd_path + " (Permission denied)";
        fd = "NOFD";
    }
    else{
        struct dirent* fd_entry;
        while(fd_entry = readdir(dir)){
            if(!strcmp(fd_entry->d_name, ".") || !strcmp(fd_entry->d_name, ".."))
                continue;
            
            tmp_path =  fd_path+"/"+fd_entry->d_name;
            if((link_size = readlink(tmp_path.c_str(),link,sizeof(link)-1))<0){
                name = tmp_path + " (Permission denied)";
                type = "unknown";
            }
            else{
                link[link_size] = '\0';
                fd = get_fd(tmp_path,std::string(fd_entry->d_name));
                type = get_type(tmp_path);
                node = std::to_string(get_inode(tmp_path));
                name = std::string(link);
            }
            if(check_del(name)){
                name = name.substr(0,name.length()-10);
            }
            if(type_flag == true){
                if(type != type_arg)
                    continue;
            }
                
            if(file_flag == true){
                if(!is_regex(file_arg.c_str(), name.c_str()))
                    continue;
            }
            std::cout<<inf->cmd<<" "<<inf->pid<<" "<<inf->user<<" "<<fd<<" "<<type<<" "<<node<<" "<<name<<" "<<std::endl;
        }
        closedir(dir);
    }
}
void print_information(std::string pid){
    //TODO: print the information of each process with pid
    // std::cout<<pid<<std::endl;
    struct proc_inf inf;
    inf.path = "/proc/" + pid +"/";
    inf.pid = pid;
    
    get_user(&inf);
    // std::cout<<inf.user<<" ";
    // std::cout<<inf.path<<" ";
    get_cmd(&inf);
    // std::cout<<inf.cmd<<" ";
    if(cmd_flag){
        if(!is_regex(cmd_arg.c_str(),inf.cmd.c_str()))
            return;
    }
    print_crt("cwd",&inf);
    print_crt("root",&inf);
    print_crt("exe",&inf);
    print_mD(&inf);
    print_FD(&inf);
}



int main(int argc, char** argv){
    int option;
    if(argc > 1){
        while((option =getopt(argc, argv, "c:t:f:")) != -1){
            switch (option){
                case 'c':
                // TODO set c option
                    cmd_flag = true;
                    cmd_arg = std::string(optarg);
                    break;
                case 't':
                // TODO set t option
                    if(!strcmp(optarg, "REG") || !strcmp(optarg, "CHR") || 
                       !strcmp(optarg, "DIR") || !strcmp(optarg, "FIFO") ||
                       !strcmp(optarg, "SOCK") || !strcmp(optarg, "unknown")){
                        type_flag = true;
                        if(optarg)
                            type_arg = std::string(optarg);
                        else
                            return 1;
                           
                        }
                        else{
                            std::cout<<"Invalid TYPE option."<<std::endl;
                            return 0;
                        }
                    break;
                case 'f':
                // TODO set f option
                    file_flag = true;
                    file_arg = std::string(optarg);
                    break;
            }
        }
    }
    // std::cout<<cmd_arg<<" "<<type_arg<<" "<<file_arg<<std::endl;
    DIR *dir = opendir("/proc");
    struct dirent* proc_entry;
    //TODO: print the first line(COMMAND,PID,...)
    std::cout<<"COMMAND PID USER FD TYPE NODE NAME"<<std::endl;
    while(proc_entry = readdir(dir)){
        // strcmp return 0 if equal, return none 0 if not equal
        if(strcmp(proc_entry->d_name,".") && strcmp(proc_entry->d_name,"..") && is_digit(proc_entry->d_name))
            print_information(std::string(proc_entry->d_name));
    }
    closedir(dir);
}