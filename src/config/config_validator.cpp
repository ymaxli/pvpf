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

    vector<validation_result const> config_validator::validate(Document &conf)
    {
        vector<validation_result const> res;

        vector<validation_rule*> rules;

        rules.push_back(new concrete_rule_format());
        rules.push_back(new concrete_rule_source());
        rules.push_back(new concrete_rule_graph());
        rules.push_back(new concrete_rule_sink());
        rules.push_back(new concrete_rule_duplicate_id());
        rules.push_back(new concrete_rule_predecessor_check());


        for(int i = 0; i < rules.size(); ++i) {
            const validation_result result = rules[i]->validate(conf);
            res.push_back(result);
            if(result.type == 2) {
                break;
            }
        }

        return res;

    }
}

PVPF_NAMESPACE_END







