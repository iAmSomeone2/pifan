//
// Created by bdavidson on 10/3/19.
//

#ifndef PIFAN_DATAACCESS_H
#define PIFAN_DATAACCESS_H

#include <string>

class DataAccess {
private:
    const std::string currentTempFile = "/sys/class/thermal/thermal_zone0/temp";
public:
    DataAccess();
    int getTargetTemp();
    int getCurrentTemp();
};


#endif //PIFAN_DATAACCESS_H
