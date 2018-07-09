//
// Created by Icay on 20/06/2018.
//

#include "pvpf/config/config_validator.hpp"
#include "pvpf/config/validation_rule.hpp"
#include <iostream>

using namespace rapidjson;
using namespace std;

PVPF_NAMESPACE_BEGIN
    namespace config {

        vector<validation_result> config_validator::validate(Document const &conf) {
            vector<validation_result> res;

            vector<validator> rules;
            rules.push_back(concrete_rule_format);
            rules.push_back(concrete_rule_source);
            rules.push_back(concrete_rule_graph);
            rules.push_back(concrete_rule_sink);
            rules.push_back(concrete_rule_duplicate_id);
            rules.push_back(concrete_rule_predecessor_check);
            rules.push_back(concrete_rule_successor_check);


            for (int i = 0; i < rules.size(); ++i) {
                auto result = rules[i](conf);
                res.push_back(result);
                if (result.get_type() == 2) {
                    break;
                }
            }

            return res;
        }
    }

PVPF_NAMESPACE_END







