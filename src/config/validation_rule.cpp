//
// Created by Icay on 20/06/2018.
//

#include "pvpf/config/validation_rule.h"
#include <iostream>
#include <unordered_set>
#include <unordered_map>

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
            return validation_result(2, "Error: lack \"source\" field");
        }
        else if (!conf["source"].IsArray()) {
            return validation_result(2, "Error: the content of \"source\" field is not an array");
        }
        // rule 1.3: check source field is empty or not
        else if(conf["source"].Empty()){
            return validation_result(2, "Error: \"source\" field has to have at least one child");
        }
        else {
            const Value& source = conf["source"];
            for (rapidjson::SizeType i = 0; i < source.Size(); i++) {
                if(source[i].ObjectEmpty()) {
                    return validation_result(2, "Error: “source” field has to have at least one child");
                }
                // rule 1.8: check node id
                if (!source[i].HasMember("id")) {
                    return validation_result(2, "Error: node has to have an id");
                }
                else if(!source[i]["id"].IsString() || (source[i]["id"].GetStringLength() == 0)) {
                    return validation_result(2, "Error: source node id should be nonempty string");
                }

                // rule 1.10: check task field of source node
                if (!source[i].HasMember("task")) {
                    return validation_result(2, "Error: source node has to have a task field");
                }
                else if(!source[i]["task"].IsObject()) {
                    return validation_result(2, "Error: source node task should be an object");
                }
                else if(!source[i]["task"].HasMember("dylib")){
                    return validation_result(2, "Error: source node task should have a dylib field");
                }
                else if(!source[i]["task"]["dylib"].HasMember("location") || !source[i]["task"]["dylib"].HasMember("func")) {
                    return validation_result(2, "Error: source node task dylib should have location and func");
                }
                else if(!source[i]["task"]["dylib"]["location"].IsString() || source[i]["task"]["dylib"]["location"].GetStringLength() == 0
                        || !source[i]["task"]["dylib"]["func"].IsString() || source[i]["task"]["dylib"]["func"].GetStringLength() == 0) {
                    return validation_result(2, "Error: source node id should be nonempty string");
                }

                // rule 1.12: check output field of source node
                if (!source[i].HasMember("output")) {
                    return validation_result(2, "Error: source node has to have an output field");
                }
                else if(!source[i]["output"].IsObject()) {
                    return validation_result(2, "Error: source node output should be an object");
                }
                else if(!source[i]["output"].HasMember("data")) {
                    return validation_result(2, "Error: source node output should have a data field");
                }
            }

            return validation_result(0, "Pass: source field check");

        }

    }

    validation_result concrete_rule_graph::validate(Document &conf) {
        // rule 1.4: check graph field
        if(!conf.HasMember("graph")){
            return validation_result(2, "Error: lack \"graph\" field");
        }
        else if (! conf["graph"].IsArray()){
            return validation_result(2, "Error: the content of \"graph\" field is not an array");
        }
        // rule 1.5: check graph field is empty or not
        else if(conf["graph"].Empty()){
            return validation_result(2, "Error: \"graph\" field has to have at least one child");
        }
        else {
            const Value &graph = conf["graph"];
            for (rapidjson::SizeType i = 0; i < graph.Size(); i++) {
                if(graph[i].ObjectEmpty()) {
                    return validation_result(2, "Error: “graph” field has to have at least one child");
                }
                // rule 1.8: check node id
                if (!graph[i].HasMember("id")) {
                    return validation_result(2, "Error: node has to have an id");
                }
                else if(!graph[i]["id"].IsString() || (graph[i]["id"].GetStringLength() == 0)) {
                    return validation_result(2, "Error: graph node id should be nonempty string");
                }
                // rule 1.14: check input field of graph node
                if (!graph[i].HasMember("input")) {
                    return validation_result(2, "Error: graph node has to have an input field");
                }
                else if(!graph[i]["input"].IsObject()) {
                    return validation_result(2, "Error: graph node input should be an object");
                }
                else if(!graph[i]["input"].HasMember("pre")) {
                    return validation_result(2, "Error: graph node input should have a pre field");
                }
                else if(!(graph[i]["input"]["pre"].IsString() && graph[i]["input"]["pre"].GetStringLength() != 0)
                        && !graph[i]["input"]["pre"].IsArray()) {
                    return validation_result(2, "Error: graph node input pre should be nonempty string or string list");
                }
                else if(graph[i]["input"]["pre"].IsArray()) {
                    const Value &pre = graph[i]["input"]["pre"];
                    for (rapidjson::SizeType j = 0; j < pre.Size(); j++) {
                        if(!pre[j].IsString() || pre[j].GetStringLength() == 0)
                            return validation_result(2, "Error: graph node input pre should be nonempty string or string list");
                    }
                }

                // rule 1.15: : check task field of graph node
                if (!graph[i].HasMember("task")) {
                    return validation_result(2, "Error: graph node has to have a task field");
                }
                else if(!graph[i]["task"].IsObject()) {
                    return validation_result(2, "Error: graph node task should be an object");
                }
                else if(!graph[i]["task"].HasMember("algorithm")) {
                    return validation_result(2, "Error: graph node task should have an algorithm field");
                }
                else if(!graph[i]["task"]["algorithm"].IsString() || graph[i]["task"]["algorithm"].GetStringLength() == 0) {
                    return validation_result(2, "Error: graph node task algorithm should be nonempty string");
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
            return validation_result(2, "Error: lack \"sink\" field");
        }
        else if (!conf["sink"].IsArray()) {
            return validation_result(2, "Error: the content of \"sink\" field is not an array");
        }
        // rule 1.7: check sink field is empty or not
        else if(conf["sink"].Empty()) {
            return validation_result(2, "Error: \"sink\" field has to have at least one child");
        }
        else {
            const Value& sink = conf["sink"];
            for (rapidjson::SizeType i = 0; i < sink.Size(); i++) {
                if(sink[i].ObjectEmpty()) {
                    return validation_result(2, "Error: “sink” field has to have at least one child");
                }
                // rule 1.8: check node id
                if (!sink[i].HasMember("id")) {
                    return validation_result(2, "Error: node has to have an id");
                }
                else if(!sink[i]["id"].IsString()  || (sink[i]["id"].GetStringLength() == 0)) {
                    return validation_result(2, "Error: sink node id should be nonempty string");
                }

                // rule 11: check task field of sink node
                if (!sink[i].HasMember("task")) {
                    return validation_result(2, "Error: sink node has to have a task field");
                }
                else if(!sink[i]["task"].IsObject()) {
                    return validation_result(2, "Error: sink node task should be an object");
                }
                else if(!sink[i]["task"].HasMember("dylib")){
                    return validation_result(2, "Error: sink node task should have a dylib field");
                }
                else if(!sink[i]["task"]["dylib"].HasMember("location") || !sink[i]["task"]["dylib"].HasMember("func")) {
                    return validation_result(2, "Error: sink node task dylib should have location and func");
                }
                else if(!sink[i]["task"]["dylib"]["location"].IsString() || sink[i]["task"]["dylib"]["location"].GetStringLength() == 0
                        || !sink[i]["task"]["dylib"]["func"].IsString() || sink[i]["task"]["dylib"]["func"].GetStringLength() == 0) {
                    return validation_result(2, "Error: sink node id should be nonempty string");
                }

                // rule 13: check input field of sink node
                if (!sink[i].HasMember("input")) {
                    return validation_result(2, "Error: sink node has to have an input field");
                }
                else if(!sink[i]["input"].IsObject()) {
                    return validation_result(2, "Error: sink node input should be an object");
                }
                else if(!sink[i]["input"].HasMember("pre")) {
                    return validation_result(2, "Error: sink node input should have a pre field");
                }
                else if(!(sink[i]["input"]["pre"].IsString() && sink[i]["input"]["pre"].GetStringLength() != 0)
                        && !sink[i]["input"]["pre"].IsArray()) {
                    return validation_result(2, "Error: sink node input pre should be nonempty string or string list");
                }
                else if(sink[i]["input"]["pre"].IsArray()) {
                    const Value &pre = sink[i]["input"]["pre"];
                    for (rapidjson::SizeType j = 0; j < pre.Size(); j++) {
                        if(!pre[j].IsString() || pre[j].GetStringLength() == 0)
                            return validation_result(2, "Error: sink node input pre should be nonempty string or string list");
                    }
                }
            }
            return validation_result(0, "Pass: sink field check");
        }
    }

    // rule 9: check duplicate node
    // after check the other rules

    validation_result concrete_rule_duplicate_id::validate(Document &conf) {
        unordered_set<string> id_set;
        //get id in sink
        const Value& sink = conf["sink"];
        for (rapidjson::SizeType i = 0; i < sink.Size(); i++) {
            string id = sink[i]["id"].GetString();
            if(id_set.find(id) != id_set.end()) {
                return validation_result(2, "Error: detect duplicate node id \""  + id +"\"");
            }
            id_set.insert(id);
        }
        //get id in source
        const Value& source = conf["source"];
        for (rapidjson::SizeType i = 0; i < source.Size(); i++) {
            string id = source[i]["id"].GetString();
            if(id_set.find(id) != id_set.end()) {
                return validation_result(2, "Error: detect duplicate node id \""  + id +"\"");
            }
            id_set.insert(id);
        }
        //get id in graph
        const Value& graph = conf["graph"];
        for (rapidjson::SizeType i = 0; i < graph.Size(); i++) {
            string id = graph[i]["id"].GetString();
            if(id_set.find(id) != id_set.end()) {
                return validation_result(2, "Error: detect duplicate node id \""  + id +"\"");
            }
            id_set.insert(id);
        }

        return validation_result(0, "Pass: no duplicate node id");
    }

//    validation_result concrete_rule_library_search::validate(Document &conf) {
//        const Value& sink = conf["sink"];
//        for (rapidjson::SizeType i = 0; i < sink.Size(); i++) {
//            string task = sink[i]["task"].GetString();
////            if(!boost::filesystem::exists(task))
////                return validation_result(2, "Error: dylib");
//            //TODO
//
//        }
//    }


    validation_result concrete_rule_predecessor_check::validate(Document &conf) {

        // collect sink id
        unordered_set<string> sink_id;
        const Value& sink = conf["sink"];
        for (rapidjson::SizeType i = 0; i < sink.Size(); i++) {
            string id= sink[i]["id"].GetString();
            sink_id.insert(id);
        }

        // collect graph id
        unordered_set<string> graph_id;
        const Value& graph = conf["graph"];
        for (rapidjson::SizeType i = 0; i < graph.Size(); i++) {
            string id= graph[i]["id"].GetString();
            graph_id.insert(id);
        }

        // collect source id
        unordered_set<string> source_id;
        const Value& source = conf["source"];
        for (rapidjson::SizeType i = 0; i < source.Size(); i++) {
            string id= source[i]["id"].GetString();
            source_id.insert(id);
        }

        // check predecessor for graph
        for (rapidjson::SizeType i = 0; i < graph.Size(); i++) {
            const Value & pre =  graph[i]["input"]["pre"];
            if(!pre.IsArray()) {
                string pre_id = pre.GetString();
                if(source_id.find(pre_id) == source_id.end() && graph_id.find(pre_id) == graph_id.end()){
                    return validation_result(2, "Error: predecessor \"" + pre_id +"\" does not exist");
                }
            }
            else {
                for (rapidjson::SizeType j = 0; j < pre.Size(); j++) {
                    string pre_id = pre[j].GetString();
                    if(source_id.find(pre_id) == source_id.end() && graph_id.find(pre_id) == graph_id.end()){
                        return validation_result(2, "Error: predecessor \"" + pre_id +"\" does not exist");
                    }
                }
            }
        }

        // check predecessor for sink
        for (rapidjson::SizeType i = 0; i < sink.Size(); i++) {
            const Value & pre =  sink[i]["input"]["pre"];
            if(!pre.IsArray()) {
                string pre_id = pre.GetString();
                if(source_id.find(pre_id) == source_id.end() && graph_id.find(pre_id) == graph_id.end()){
                    return validation_result(2, "Error: predecessor \"" + pre_id +"\" does not exist");
                }
            }
            else {
                for (rapidjson::SizeType j = 0; j < pre.Size(); j++) {
                    string pre_id = pre[j].GetString();
                    if(source_id.find(pre_id) == source_id.end() && graph_id.find(pre_id) == graph_id.end()){
                        return validation_result(2, "Error: predecessor \"" + pre_id +"\" does not exist");
                    }
                }
            }

        }

        return validation_result(0, "Pass: predecessors exist");


    }


    validation_result concrete_rule_successor_check::validate(Document &conf) {
        unordered_map<string, int> pre_map;
        const Value& sink = conf["sink"];
        const Value& graph = conf["graph"];

        // check predecessors in graph
        for (rapidjson::SizeType i = 0; i < graph.Size(); i++) {
            const Value & pre =  graph[i]["input"]["pre"];
            if(!pre.IsArray()) {
                string pre_id = pre.GetString();
                pre_map[pre_id]++;
            }
            else {
                for (rapidjson::SizeType j = 0; j < pre.Size(); j++) {
                    string pre_id = pre[j].GetString();
                    pre_map[pre_id]++;
                }
            }
        }

        // check predecessors in sink
        for (rapidjson::SizeType i = 0; i < sink.Size(); i++) {
            const Value & pre =  sink[i]["input"]["pre"];
            if(!pre.IsArray()) {
                string pre_id = pre.GetString();
                pre_map[pre_id]++;
            }
            else {
                for (rapidjson::SizeType j = 0; j < pre.Size(); j++) {
                    string pre_id = pre[j].GetString();
                    pre_map[pre_id]++;
                }
            }
        }

        // count successors
        for(auto & it : pre_map) {
            if(it.second > MAX_SUCCESSOR) {
                return validation_result(2, "Error: number of successors of one node exceeds maximum value \"" + it.first + "\"");
            }
        }

        return validation_result(0, "Pass: successors number check");
    }



}



PVPF_NAMESPACE_END







