//
// Created by Icay on 20/06/2018.
//

#ifndef CONFIG_READER_H
#define CONFIG_READER_H
#pragma once

#include <rapidjson/document.h>
#include <unordered_map>
#include <string>
#include "pvpf/pvpf.hpp"
#include <memory>

PVPF_NAMESPACE_BEGIN

    namespace config {
        class config_reader {
        public:
            rapidjson::Document load_json_conf(const std::string &file);

            std::shared_ptr<std::unordered_map<std::string, rapidjson::Document>> load_algorithm(rapidjson::Document const &d);
        private:
            void config_algorithm(std::unordered_map<std::string, rapidjson::Document> &map, std::string algorithm_name);
        };


    }

PVPF_NAMESPACE_END

#endif //CONFIG_READER_H
