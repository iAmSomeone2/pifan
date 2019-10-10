//
// Created by Brenden Davidson on 10/9/19.
//

#ifndef PIFAN_CONFIGURE_H
#define PIFAN_CONFIGURE_H

#include <json/json.h>

namespace PiFan {
    class Configure {
    private:
        std::string m_configFilePath;
        Json::Value m_configRoot;

        void loadConfig();

    public:
        Configure(std::string configFilePath);

    };
}

#endif //PIFAN_CONFIGURE_H
