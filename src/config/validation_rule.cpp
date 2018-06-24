//
// Created by Icay on 20/06/2018.
//

#include "pvpf/config/validation_rule.h"
#include <iostream>
using namespace rapidjson;
using namespace std;

PVPF_NAMESPACE_BEGIN

namespace config
{
    validation_result concrete_rule_source::validate(Document &conf) {
        if(!conf.HasMember("source")){
            return validation_result(2, "lack “source” field");
        }
        //TODO
    }
}

PVPF_NAMESPACE_END







