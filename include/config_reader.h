//
// Created by Icay on 20/06/2018.
//

#ifndef CONFIG_READER_H
#define CONFIG_READER_H
#pragma once

#include <rapidjson/document.h>

namespace config
{
    class config_reader{
    public:
//        rapidjson::Document load_json_conf(std::string file);
        int print(){
            std::cout << "print" << std::endl;
        }
    };
}


#endif //CONFIG_READER_H
