#include <iostream>
#include <json/json.h>

#include <PiFanConfig.h>
#include "Configure/Configure.h"



int main(int argc, char **argv) {
    std::cout << "Pifan config file: " << CONFIG_FILE_PATH << std::endl;
    std::cout << "Number of args: " << argc << std::endl;
    std::cout << "Provided args:" << std::endl;
    for (int i=0; i < argc; i++) {
        std::cout << "\t* " << argv[i] << std::endl;
    }
    return 0;
}
