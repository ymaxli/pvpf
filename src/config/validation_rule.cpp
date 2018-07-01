//
// Created by Icay on 20/06/2018.
//

#include "pvpf/config/validation_rule.h"
#include <iostream>
#include <unordered_set>

using namespace rapidjson;
using namespace std;

PVPF_NAMESPACE_BEGIN

namespace config
{
    validation_result concrete_rule_format::validate(rapidjson::Document &conf) {
        // rule 1.1: invalid JSON format
        if(!conf.IsObject()) {
            return validation_result(2, "Error: invalid JSON file");
        }
        else {
            return validation_result(0, "Pass: valid JSON file");
        }
    }

    validation_result concrete_rule_source::validate(Document &conf) {
        // rule 1.2: check source field
        if(!conf.HasMember("source")){
            return validation_result(2, "Error: lack “source” field");
        }
        else if (!conf["source"].IsArray()) {
            return validation_result(2, "Error: the content of “source” field is not an array");
        }
        // rule 1.3: check source field is empty or not
        else if(conf["source"].Empty()){
            return validation_result(2, "Error: “source” field has to have at least one child");
        }
        else {
            const Value& source = conf["source"];
            for (Value::ConstValueIterator itr = source.Begin(); itr != source.End(); ++itr) {
                // rule 1.8: check node id
                if (!itr->HasMember("id")) {
                    return validation_result(2, "Error: node has to have an id");
                }
                // rule 1.10: check task field of source node
                if (!itr->HasMember("task")) {
                    return validation_result(2, "Error: source node has to have a task field");
                }
                // rule 1.12: check output field of source node
                if (!itr->HasMember("output")) {
                    return validation_result(2, "Error: source node has to have an output field");
                }
            }

            return validation_result(0, "Pass: source field check");

        }

    }

    validation_result concrete_rule_graph::validate(Document &conf) {
        // rule 1.4: check graph field
        if(!conf.HasMember("graph")){
            return validation_result(2, "Error: lack “graph” field");
        }
        else if (! conf["graph"].IsArray()){
            return validation_result(2, "Error: the content of “graph” field is not an array");
        }
        // rule 1.5: check graph field is empty or not
        else if(conf["graph"].Empty()){
            return validation_result(2, "Error: “graph” field has to have at least one child");
        }
        else {
            const Value &graph = conf["graph"];
            for (Value::ConstValueIterator itr = graph.Begin(); itr != graph.End(); ++itr) {
                // rule 1.8: check node id
                if (!itr->HasMember("id")) {
                    return validation_result(2, "Error: node has to have an id");
                }
                // rule 1.14: check input field of graph node
                if (!itr->HasMember("input")) {
                    return validation_result(2, "Error: graph node has to have an input field");
                }
                // rule 1.15: : check task field of graph node
                if (!itr->HasMember("task")) {
                    return validation_result(2, "Error: graph node has to have a task field");
                }
                    // rule 1.16: check algorithm of graph node
//                else {
//                    if (graph[i]["task"].HasMember("algorithm")) {
//                        return validation_result(2, "Error: algorithm not found");
//                    }
//                }
            }
            return validation_result(0, "Pass: graph field check");
        }
    }

    validation_result concrete_rule_sink::validate(Document &conf) {
        // rule 1.6: check sink field
        if(!conf.HasMember("sink")){
            return validation_result(2, "Error: lack “sink” field");
        }
        else if (!conf["sink"].IsArray()) {
            return validation_result(2, "Error: the content of “sink” field is not an array");
        }
        // rule 1.7: check sink field is empty or not
        else if(conf["sink"].Empty()) {
            return validation_result(2, "Error: “sink” field has to have at least one child");
        }
        else {
            const Value& sink = conf["sink"];
            for (Value::ConstValueIterator itr = sink.Begin(); itr != sink.End(); ++itr){
                // rule 1.8: check node id
                if (!itr->HasMember("id")) {
                    return validation_result(2, "Error: node has to have an id");
                }
                // rule 11: check task field of sink node
                if (!itr->HasMember("task")) {
                    return validation_result(2, "Error: sink node has to have a task field");
                }
                // rule 13: check input field of sink node
                if (!itr->HasMember("input")) {
                    return validation_result(2, "Error: sink node has to have an input field");
                }
            }
            return validation_result(0, "Pass: sink field check");
        }
    }

    // rule 9: check duplicate node
    // after check the other rules

    validation_result concrete_rule_duplicate_id::validate(Document &conf) {
        unordered_set<string> id_set;
        cout << 1 << endl;
        //get id in sink
        const Value& sink = conf["sink"];
        for (rapidjson::SizeType i = 0; i < sink.Size(); i++) {
            string id = sink[i]["id"].GetString();
            if(id_set.find(id) != id_set.end()) {
                return validation_result(2, "Error: detect duplicate node id");
            }
            id_set.insert(id);
        }
        cout << 2 << endl;
        //get id in source
        const Value& source = conf["source"];
        for (rapidjson::SizeType i = 0; i < source.Size(); i++) {
            string id = source[i]["id"].GetString();
            if(id_set.find(id) != id_set.end()) {
                return validation_result(2, "Error: detect duplicate node id");
            }
            id_set.insert(id);
        }
        cout << 3 << endl;
        //get id in graph
        const Value& graph = conf["graph"];
        for (rapidjson::SizeType i = 0; i < graph.Size(); i++) {
            string id = graph[i]["id"].GetString();
            if(id_set.find(id) != id_set.end()) {
                return validation_result(2, "Error: detect duplicate node id");
            }
            id_set.insert(id);
        }

        return validation_result(0, "Pass: no duplicate node id");
    }
}



PVPF_NAMESPACE_END







