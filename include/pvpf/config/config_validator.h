//
// Created by Icay on 21/06/2018.
//

#ifndef CONFIG_VALIDATOR_H
#define CONFIG_VALIDATOR_H
#pragma once

#include <rapidjson/document.h>
#include <vector>
#include <string>
#include "pvpf/pvpf.h"

PVPF_NAMESPACE_BEGIN

namespace config
{
    struct validation_result {
        int type; // type: int(0: pass, 1: warning, 2: error)
        std::string message;
        inline validation_result(int t, std::string m) : type(t), message(m) {};
        inline validation_result(const validation_result &vr){
            type = vr.type;
            message = vr.message;
        }
    };

    class config_validator {
    public:
        const validation_result test(rapidjson::Document &d){
            const validation_result vr(2, "invalid JSON file");
            return vr;
        }
        std::vector<const validation_result> validate(rapidjson::Document &conf);
    };

}

PVPF_NAMESPACE_END
#endif //CONFIG_VALIDATOR_H
