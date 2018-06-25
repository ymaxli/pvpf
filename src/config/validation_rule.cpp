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
    validation_result concrete_rule_format::validate(rapidjson::Document &conf) {
        if(!conf.IsObject()) {
            return validation_result(2, "invalid JSON file");
        }
        else {
            return validation_result(0, "valid JSON file");

        }
    }

    validation_result concrete_rule_source::validate(Document &conf) {
        if(!conf.HasMember("source")){
            return validation_result(2, "Error: lack “source” field");
        }
        else if(conf["source"].Empty()){
            return validation_result(2, "Error: “source” field has to have at least one child");
        }
        else {
            return validation_result(0, "Pass: source field check");
        }
    }

    validation_result concrete_rule_graph::validate(Document &conf) {
        if(!conf.HasMember("graph")){
            return validation_result(2, "Error: lack “graph” field");
        }
        else if(conf["source"].Empty()){
            return validation_result(2, "Error: “graph” field has to have at least one child");
        }
        else {
            return validation_result(0, "Pass: graph field check");
        }
    }

    validation_result concrete_rule_sink::validate(Document &conf) {
        if(!conf.HasMember("sink")){
            return validation_result(2, "Error: lack “sink” field");
        }
        else if(conf["source"].Empty()){
            return validation_result(2, "Error: “sink” field has to have at least one child");
        }
        else {
            return validation_result(0, "Pass: sink field check");
        }
    }
}

PVPF_NAMESPACE_END







