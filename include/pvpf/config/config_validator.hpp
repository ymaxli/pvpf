//
// Created by Icay on 21/06/2018.
//

#ifndef CONFIG_VALIDATOR_H
#define CONFIG_VALIDATOR_H
#pragma once

#include <rapidjson/document.h>
#include <vector>
#include <string>
#include "pvpf/pvpf.hpp"

PVPF_NAMESPACE_BEGIN

    namespace config {
        struct validation_result {
        public:
            validation_result(int t, std::string m) : type(t), message(m) {};

            validation_result(validation_result const &vr) {
                type = vr.type;
                message = vr.message;
            }

            int get_type() {
                return type;
            }

            std::string const &get_message() {
                return message;
            }

        private:
            int type; // type: int(0: pass, 1: warning, 2: error)
            std::string message;
        };


        class config_validator {
        public:
            std::vector<validation_result> validate(rapidjson::Document const &conf);
        };

    }

PVPF_NAMESPACE_END
#endif //CONFIG_VALIDATOR_H
