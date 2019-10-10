//
// Created by bdavidson on 10/3/19.
//

#ifndef PIFAN_DATAACCESS_H
#define PIFAN_DATAACCESS_H

#include <string>
#include <PiFanConfig.h>

namespace PiFan{
    class DataAccess {
    private:
        const std::string m_temperatureFilePath = THERMAL_FILE_PATH;
        const std::string m_configFilePath = CONFIG_FILE_PATH;
        const int m_defaultTargetTemp = 50;

        int m_targetTemp = m_defaultTargetTemp;
        bool m_monitorEnabled = true;

        void parseConfigFile();
    public:
        int m_upperTempBuffer{};
        int m_lowerTempBuffer{};

        DataAccess();
        int getTargetTemp();
        bool monitoringEnabled();
        int getCurrentTemp();
    };
}


#endif //PIFAN_DATAACCESS_H
