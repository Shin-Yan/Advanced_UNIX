#include "help.h"

int help(string cmd){
    for(auto &x:Commands) {
        cout<< x.help_msg << endl;
    }
    return 0;
}