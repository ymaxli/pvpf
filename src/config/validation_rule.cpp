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
            return validation_result(2, "Error: invalid JSON file");
        }
        else {
            return validation_result(0, "Pass: valid JSON file");
        }
    }

    validation_result concrete_rule_source::validate(Document &conf) {
        // rule 2: check source field
        if(!conf.HasMember("source")){
            return validation_result(2, "Error: lack “source” field");
        }
        // rule 3: check source field is empty or not
        else if(conf["source"].Empty()){
            return validation_result(2, "Error: “source” field has to have at least one child");
        }
        else if (!conf["source"].IsArray()) {
            return validation_result(2, "Error: the content of “source” field is not an array");
        }
        else if (conf["source"].IsArray()) {
            const Value& source = conf["source"];
            for (SizeType i = 0; i < source.Size; i ++) {
                // rule 8: check node id
                if (!source[i].HasMember("id")) {
                    return validation_result(2, "Error: node has to have an id");
                }
                else {
                    (this->node_ids).push_back(source[i]["id"]);
                }
                // rule 10: check task field of source node
                if (!source[i].HasMember("task")) {
                    return validation_result(2, "Error: source node has to have a task field");
                }
                // rule 12: check output field of source node
                if (!source[i].HasMember("output")) {
                    return validation_result(2, "Error:  source node has to have an output field");
                }
            }

        }
        else {
            return validation_result(0, "Pass: source field check");
        }
    }

    validation_result concrete_rule_graph::validate(Document &conf) {
        // rule 4: check graph field
        if(!conf.HasMember("graph")){
            return validation_result(2, "Error: lack “graph” field");
        }
        // rule 5: check graph field is empty or not
        else if(conf["graph"].Empty()){
            return validation_result(2, "Error: “graph” field has to have at least one child");
        }
        else if (!conf["graph"].IsArray()) {
            return validation_result(2, "Error: the content of “graph” field is not an array");
        }
        else if (conf["graph"].IsArray())
        {
            const Value &graph = conf["graph"];
            for (SizeType i = 0; i < graph.Size(); i ++)
            {
                // rule 8: check node id
                if (! graph[i].HasMember("id")) {
                    return validation_result(2, "Error: node has to have an id");
                }
                else {
                    (this->node_ids).push_back(graph[i]["id"]);
                }
                // rule 14: check input field of graph node
                if (! graph[i].HasMember("input")) {
                    return validation_result(2, "Error: graph node has to have an input field");
                }
                // rule 15: : check task field of graph node
                if (! graph[i].HasMember("task")) {
                    return validation_result(2, "Error: graph node has to have an task field");
                }
                    // rule 16: check algorithm of graph node
                else {
                    if (graph[i]["task"].HasMember("algorithm")) {
                        return validation_result(2, "Error: algorithm not found");
                    }
                }
            }
        }
        else {
            return validation_result(0, "Pass: graph field check");
        }
    }

    validation_result concrete_rule_sink::validate(Document &conf) {
        // rule 6: check sink field
        if(!conf.HasMember("sink")){
            return validation_result(2, "Error: lack “sink” field");
        }
        // rule 7: check sink field is empty or not
        else if(conf["sink"].Empty()) {
            return validation_result(2, "Error: “sink” field has to have at least one child");
        }
        else if (!conf["sink"].isArray()) {
            return validation_result(2, "Error: the content of “sink” field is not an array");
        }
        else if (conf["sink"].isArray) {
            const Value& sink = conf["sink"];
            for (SizeType i = 0; i < sink.Size(); i ++) {
                // rule 8: check node id
                if (!sink[i].HasMember("id")) {
                    return validation_result(2, "Error: node has to have an id");
                }
                else {
                    (this->node_ids).push_back(sink[i]["id"]);
                }
                // rule 11: check task field of sink node
                if (!sink[i].HasMember("task")) {
                    return validation_result(2, "Error: A sink node missing the task field");
                }
                // rule 13: check input field of sink node
                if (!sink[i].HasMember("input")) {
                    return validation_result(2, "Error: sink node has to have an input field");
                }
            }
        }
        else {
            return validation_result(0, "Pass: sink field check");
        }
    }

    // rule 9: check duplicate node
    validation_result duplicate_node_rule::validate(Document &conf) {
        std::set<int> id_set(this->node_ids.begin(), this->node_ids.end());
        if (id_set.size() < this->node_ids.size())
            return validation_result(2, "Error: detect duplicate node id");
        return validation_result(0, "Pass: no duplicate node id");
    }
}



PVPF_NAMESPACE_END







