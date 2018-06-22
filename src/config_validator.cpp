//
// Created by Icay on 20/06/2018.
//

#include "pvpf/config_validator.h"
#include "pvpf/validation_rule.h"

using namespace rapidjson;
using namespace std;

PVPF_NAMESPACE_BEGIN

namespace config
{
    vector<validation_result const> validate(Document &conf) {
        vector<validation_result const> res;

        validation_rule rule = concrete_rule_sink();
        const validation_result sink = rule.validate(conf);

        //TODO
    }

}

PVPF_NAMESPACE_END







