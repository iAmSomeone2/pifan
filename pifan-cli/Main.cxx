#include <iostream>

#include <PiFanConfig.h>
#include "Configure/Configure.h"

using namespace PiFan;


int main() {
    std::cout << "Pifan config file: " << CONFIG_FILE_PATH << std::endl;
    Configure config = Configure(CONFIG_FILE_PATH);
    return 0;
}
