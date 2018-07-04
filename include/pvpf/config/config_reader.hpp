//
// Created by Icay on 20/06/2018.
//

#ifndef CONFIG_READER_H
#define CONFIG_READER_H
#pragma once

#include <rapidjson/document.h>
#include "pvpf/pvpf.hpp"

PVPF_NAMESPACE_BEGIN

namespace config
{
    class config_reader{
    public:
        rapidjson::Document load_json_conf(const std::string &file);
    };
}

PVPF_NAMESPACE_END

#endif //CONFIG_READER_H
