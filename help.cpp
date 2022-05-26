#include "help.h"

int help(string){
    vector<Command>::iterator it = Commands.begin();
    for(; it != Commands.end() ; it++) {
        cout<< it -> help_msg << endl;
    }
    return 0;
}