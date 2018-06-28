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

        // check json format
        concrete_rule_format rule_format = concrete_rule_format();
        const validation_result format = rule_format.validate(conf);
        res.push_back(format);
        //if invalid format, return immediately
        if(format.type == 2) {
            return res;
        }

        vector<validation_rule*> rules;

        rules.push_back(new concrete_rule_source);
        rules.push_back(new concrete_rule_graph);
        rules.push_back(new concrete_rule_sink);

        for(int i = 0; i < rules.size(); ++i) {
            const validation_result result = rules[i]->validate(conf);
            res.push_back(result);
        }

//        //check source field
//        concrete_rule_source rule_source = concrete_rule_source();
//        const validation_result source = rule_source.
//        res.push_back(source);
//
//        //check graph field
//        concrete_rule_graph rule_graph = concrete_rule_graph();
//        const validation_result graph = rule_graph.validate(conf);
//        res.push_back(graph);
//
//        //check sink field
//        concrete_rule_sink rule_sink = concrete_rule_sink();
//        const validation_result sink = rule_sink.validate(conf);
//        res.push_back(sink);

        // check node duplicate
//        duplicate_node_rule rule_duplicate = duplicate_node_rule();
//        const validation_result duplicate = rule_duplicate.validate(conf);
//        res.push_back(duplicate);

        return res;

    }
}

PVPF_NAMESPACE_END







