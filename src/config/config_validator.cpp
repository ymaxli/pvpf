//
// Created by Icay on 20/06/2018.
//

#include "pvpf/config/config_validator.h"
#include "pvpf/config/validation_rule.h"
#include <iostream>

using namespace rapidjson;
using namespace std;

PVPF_NAMESPACE_BEGIN

namespace config
{
    vector<validation_result const> config_validator::validate(Document &conf) {
        vector<validation_result const> res;

        // check json format
        concrete_rule_format rule_format = concrete_rule_format();
        const validation_result format = rule_format.validate(conf);
        res.push_back(format);
        //if invalid format, return immediately
        if(format.type == 2) {
            return res;
        }

        //check source field
        concrete_rule_source rule_source = concrete_rule_source();
        const validation_result source = rule_source.validate(conf);
        res.push_back(source);




        return res;


    }

}

PVPF_NAMESPACE_END







