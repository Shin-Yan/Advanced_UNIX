#include "vmmap.h"

#define VMMAP_WRITE_FORMAT "%016llx-%016llx %s %010llx %s\n"
#define VMMAP_READ_FORMAT "%llx-%llx %s %*s %*s %llx"
int vmmap(string cmd){
    if(state != STATE_RUNNING){
        cout << "** state must be RUNNING" <<endl;
        return 0;
    }
    else{
        // cout << "** state is RUNNING" <<endl;
        string path = "/proc/" + to_string(loaded_program.pid) + "/maps";
        cout << "pid = " << loaded_program.pid <<endl;
        FILE* f = fopen(path.c_str(), "r");
        if (f == NULL){
            perror("fopen");
            return 0;
        }
        unsigned long long   begin_addr , end_addr  , inode;
        char permission[4] , file_path[100];
        int ret_val;
        // int i=0;
        while(true){
            ret_val = fscanf(f,VMMAP_READ_FORMAT,&begin_addr,&end_addr,permission,&inode);
            permission[3] = '\0';
            if(ret_val == 0 || ret_val == EOF){
                break;
            }
            if(inode != 0){ //file must exist
                fscanf(f,"%s", file_path);
            }
            else { //file may not exist
                char line_buffer[100];           
                file_path[0]= '\0';             //by default, file not exist
                fgets(line_buffer, 100, f);
                sscanf(line_buffer, "%s", file_path);
            }
            printf(VMMAP_WRITE_FORMAT,begin_addr,end_addr,permission,inode,file_path);
        }
        fclose(f);
        return 0;
    }
}