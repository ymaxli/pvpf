//
// Created by Icay on 20/06/2018.
//

#include "pvpf/config/validation_rule.hpp"
#include <iostream>
#include <unordered_set>
#include <unordered_map>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/filesystem.hpp>

using namespace rapidjson;
using namespace std;

PVPF_NAMESPACE_BEGIN

    namespace config {
        validation_result concrete_rule_format(rapidjson::Document const &conf) {
            // rule 1.1: invalid JSON format
            if (!conf.IsObject()) {
                return validation_result(2, "Error: invalid JSON file");
            } else {
                return validation_result(0, "Pass: valid JSON file");
            }
        }

        validation_result concrete_rule_source(rapidjson::Document const &conf) {
            // rule 1.2: check source field
            if (!conf.HasMember("source")) {
                return validation_result(2, "Error: lack \"source\" field");
            } else if (!conf["source"].IsArray()) {
                return validation_result(2, "Error: the content of \"source\" field is not an array");
            }
                // rule 1.3: check source field is empty or not
            else if (conf["source"].Empty()) {
                return validation_result(2, "Error: \"source\" field has to have at least one child");
            } else {
                const Value &source = conf["source"];
                for (rapidjson::SizeType i = 0; i < source.Size(); i++) {
                    if (!source[i].IsObject() || source[i].ObjectEmpty()) {
                        return validation_result(2, "Error: \"source\" field has to have at least one child");
                    }
                    // rule 1.8: check node id
                    if (!source[i].HasMember("id")) {
                        return validation_result(2, "Error: node has to have an id");
                    } else if (!source[i]["id"].IsString() || (source[i]["id"].GetStringLength() == 0)) {
                        return validation_result(2, "Error: source node id should be nonempty string");
                    }

                    // rule 1.10: check task field of source node
                    if (!source[i].HasMember("task")) {
                        return validation_result(2, "Error: source node has to have a task field");
                    } else if (!source[i]["task"].IsObject()) {
                        return validation_result(2, "Error: source node task should be an object");
                    } else if (!source[i]["task"].HasMember("dylib")) {
                        return validation_result(2, "Error: source node task should have a dylib field");
                    } else if (!source[i]["task"]["dylib"].HasMember("location") ||
                               !source[i]["task"]["dylib"].HasMember("func")) {
                        return validation_result(2, "Error: source node task dylib should have location and func");
                    } else if (!source[i]["task"]["dylib"]["location"].IsString() ||
                               source[i]["task"]["dylib"]["location"].GetStringLength() == 0
                               || !source[i]["task"]["dylib"]["func"].IsString() ||
                               source[i]["task"]["dylib"]["func"].GetStringLength() == 0) {
                        return validation_result(2, "Error: source node task dylib and func should be nonempty string");
                    }

                    // rule 1.12: check output field of source node
                    if (!source[i].HasMember("output")) {
                        return validation_result(2, "Error: source node has to have an output field");
                    } else if (!source[i]["output"].IsObject()) {
                        return validation_result(2, "Error: source node output should be an object");
                    } else if (!source[i]["output"].HasMember("data")) {
                        return validation_result(2, "Error: source node output should have a data field");
                    } else if (!source[i]["output"]["data"].IsObject()) {
                        return validation_result(2, "Error: source node output data should be an object");
                    } else if (source[i]["output"]["data"].ObjectEmpty()) {
                        return validation_result(2, "Error: source node output data cannot be empty");
                    }

                }

                return validation_result(0, "Pass: source field check");

            }

        }

        validation_result concrete_rule_graph(rapidjson::Document const &conf) {
            // rule 1.4: check graph field
            if (!conf.HasMember("graph")) {
                return validation_result(2, "Error: lack \"graph\" field");
            } else if (!conf["graph"].IsArray()) {
                return validation_result(2, "Error: the content of \"graph\" field is not an array");
            }
                // rule 1.5: check graph field is empty or not
            else if (conf["graph"].Empty()) {
                return validation_result(2, "Error: \"graph\" field has to have at least one child");
            } else {
                const Value &graph = conf["graph"];
                for (rapidjson::SizeType i = 0; i < graph.Size(); i++) {
                    if (!graph[i].IsObject() || graph[i].ObjectEmpty()) {
                        return validation_result(2, "Error: \"graph\" field has to have at least one child");
                    }
                    // rule 1.8: check node id
                    if (!graph[i].HasMember("id")) {
                        return validation_result(2, "Error: node has to have an id");
                    } else if (!graph[i]["id"].IsString() || (graph[i]["id"].GetStringLength() == 0)) {
                        return validation_result(2, "Error: graph node id should be nonempty string");
                    }
                    // rule 1.14: check input field of graph node
                    if (!graph[i].HasMember("input")) {
                        return validation_result(2, "Error: graph node has to have an input field");
                    } else if (!graph[i]["input"].IsObject()) {
                        return validation_result(2, "Error: graph node input should be an object");
                    } else if (!graph[i]["input"].HasMember("pre")) {
                        return validation_result(2, "Error: graph node input should have a pre field");
                    } else if (!(graph[i]["input"]["pre"].IsString() && graph[i]["input"]["pre"].GetStringLength() != 0)
                               && !graph[i]["input"]["pre"].IsArray()) {
                        return validation_result(2,
                                                 "Error: graph node input pre should be nonempty string or string list");
                    } else if (graph[i]["input"]["pre"].IsArray()) {
                        const Value &pre = graph[i]["input"]["pre"];
                        for (rapidjson::SizeType j = 0; j < pre.Size(); j++) {
                            if (!pre[j].IsString() || pre[j].GetStringLength() == 0)
                                return validation_result(2,
                                                         "Error: graph node input pre should be nonempty string or string list");
                        }
                    }

                    // rule 1.15: : check task field of graph node
                    if (!graph[i].HasMember("task")) {
                        return validation_result(2, "Error: graph node has to have a task field");
                    } else if (!graph[i]["task"].IsObject()) {
                        return validation_result(2, "Error: graph node task should be an object");
                    } else if (!graph[i]["task"].HasMember("algorithm")) {
                        return validation_result(2, "Error: graph node task should have an algorithm field");
                    } else if (!graph[i]["task"]["algorithm"].IsString() ||
                               graph[i]["task"]["algorithm"].GetStringLength() == 0) {
                        return validation_result(2, "Error: graph node task algorithm should be nonempty string");
                    }
                }
                return validation_result(0, "Pass: graph field check");
            }
        }

        validation_result concrete_rule_sink(rapidjson::Document const &conf) {
            // rule 1.6: check sink field
            if (!conf.HasMember("sink")) {
                return validation_result(2, "Error: lack \"sink\" field");
            } else if (!conf["sink"].IsArray()) {
                return validation_result(2, "Error: the content of \"sink\" field is not an array");
            }
                // rule 1.7: check sink field is empty or not
            else if (conf["sink"].Empty()) {
                return validation_result(2, "Error: \"sink\" field has to have at least one child");
            } else {
                const Value &sink = conf["sink"];
                for (rapidjson::SizeType i = 0; i < sink.Size(); i++) {
                    if (!sink[i].IsObject() || sink[i].ObjectEmpty()) {
                        return validation_result(2, "Error: \"sink\" field has to have at least one child");
                    }
                    // rule 1.8: check node id
                    if (!sink[i].HasMember("id")) {
                        return validation_result(2, "Error: node has to have an id");
                    } else if (!sink[i]["id"].IsString() || (sink[i]["id"].GetStringLength() == 0)) {
                        return validation_result(2, "Error: sink node id should be nonempty string");
                    }

                    // rule 11: check task field of sink node
                    if (!sink[i].HasMember("task")) {
                        return validation_result(2, "Error: sink node has to have a task field");
                    } else if (!sink[i]["task"].IsObject()) {
                        return validation_result(2, "Error: sink node task should be an object");
                    } else if (!sink[i]["task"].HasMember("dylib")) {
                        return validation_result(2, "Error: sink node task should have a dylib field");
                    } else if (!sink[i]["task"]["dylib"].HasMember("location") ||
                               !sink[i]["task"]["dylib"].HasMember("func")) {
                        return validation_result(2, "Error: sink node task dylib should have location and func");
                    } else if (!sink[i]["task"]["dylib"]["location"].IsString() ||
                               sink[i]["task"]["dylib"]["location"].GetStringLength() == 0
                               || !sink[i]["task"]["dylib"]["func"].IsString() ||
                               sink[i]["task"]["dylib"]["func"].GetStringLength() == 0) {
                        return validation_result(2, "Error: sink node task dylib and func should be nonempty string");
                    }

                    // rule 13: check input field of sink node
                    if (!sink[i].HasMember("input")) {
                        return validation_result(2, "Error: sink node has to have an input field");
                    } else if (!sink[i]["input"].IsObject()) {
                        return validation_result(2, "Error: sink node input should be an object");
                    } else if (!sink[i]["input"].HasMember("pre")) {
                        return validation_result(2, "Error: sink node input should have a pre field");
                    } else if (!(sink[i]["input"]["pre"].IsString() && sink[i]["input"]["pre"].GetStringLength() != 0)
                               && !sink[i]["input"]["pre"].IsArray()) {
                        return validation_result(2,
                                                 "Error: sink node input pre should be nonempty string or string list");
                    } else if (sink[i]["input"]["pre"].IsArray()) {
                        const Value &pre = sink[i]["input"]["pre"];
                        for (rapidjson::SizeType j = 0; j < pre.Size(); j++) {
                            if (!pre[j].IsString() || pre[j].GetStringLength() == 0)
                                return validation_result(2,
                                                         "Error: sink node input pre should be nonempty string or string list");
                        }
                    }
                }
                return validation_result(0, "Pass: sink field check");
            }
        }

        validation_result concrete_rule_duplicate_id(rapidjson::Document const &conf) {
            unordered_set<string> id_set;
            //get id in sink
            const Value &sink = conf["sink"];
            for (rapidjson::SizeType i = 0; i < sink.Size(); i++) {
                string id = sink[i]["id"].GetString();
                if (id_set.find(id) != id_set.end()) {
                    return validation_result(2, "Error: detect duplicate node id \"" + id + "\"");
                }
                id_set.insert(id);
            }
            //get id in source
            const Value &source = conf["source"];
            for (rapidjson::SizeType i = 0; i < source.Size(); i++) {
                string id = source[i]["id"].GetString();
                if (id_set.find(id) != id_set.end()) {
                    return validation_result(2, "Error: detect duplicate node id \"" + id + "\"");
                }
                id_set.insert(id);
            }
            //get id in graph
            const Value &graph = conf["graph"];
            for (rapidjson::SizeType i = 0; i < graph.Size(); i++) {
                string id = graph[i]["id"].GetString();
                if (id_set.find(id) != id_set.end()) {
                    return validation_result(2, "Error: detect duplicate node id \"" + id + "\"");
                }
                id_set.insert(id);
            }

            return validation_result(0, "Pass: no duplicate node id");
        }

        validation_result concrete_rule_predecessor_check(rapidjson::Document const &conf) {

            // collect sink id
            unordered_set<string> sink_id;
            const Value &sink = conf["sink"];
            for (rapidjson::SizeType i = 0; i < sink.Size(); i++) {
                string id = sink[i]["id"].GetString();
                sink_id.insert(id);
            }

            // collect graph id
            unordered_set<string> graph_id;
            const Value &graph = conf["graph"];
            for (rapidjson::SizeType i = 0; i < graph.Size(); i++) {
                string id = graph[i]["id"].GetString();
                graph_id.insert(id);
            }

            // collect source id
            unordered_set<string> source_id;
            const Value &source = conf["source"];
            for (rapidjson::SizeType i = 0; i < source.Size(); i++) {
                string id = source[i]["id"].GetString();
                source_id.insert(id);
            }

            // check predecessor for graph
            for (rapidjson::SizeType i = 0; i < graph.Size(); i++) {
                const Value &pre = graph[i]["input"]["pre"];
                if (!pre.IsArray()) {
                    string pre_id = pre.GetString();
                    if (source_id.find(pre_id) == source_id.end() && graph_id.find(pre_id) == graph_id.end()) {
                        return validation_result(2, "Error: predecessor \"" + pre_id + "\" does not exist");
                    }
                } else {
                    for (rapidjson::SizeType j = 0; j < pre.Size(); j++) {
                        string pre_id = pre[j].GetString();
                        if (source_id.find(pre_id) == source_id.end() && graph_id.find(pre_id) == graph_id.end()) {
                            return validation_result(2, "Error: predecessor \"" + pre_id + "\" does not exist");
                        }
                    }
                }
            }

            // check predecessor for sink
            for (rapidjson::SizeType i = 0; i < sink.Size(); i++) {
                const Value &pre = sink[i]["input"]["pre"];
                if (!pre.IsArray()) {
                    string pre_id = pre.GetString();
                    if (source_id.find(pre_id) == source_id.end() && graph_id.find(pre_id) == graph_id.end()) {
                        return validation_result(2, "Error: predecessor \"" + pre_id + "\" does not exist");
                    }
                } else {
                    for (rapidjson::SizeType j = 0; j < pre.Size(); j++) {
                        string pre_id = pre[j].GetString();
                        if (source_id.find(pre_id) == source_id.end() && graph_id.find(pre_id) == graph_id.end()) {
                            return validation_result(2, "Error: predecessor \"" + pre_id + "\" does not exist");
                        }
                    }
                }

            }

            return validation_result(0, "Pass: predecessors exist");


        }

        validation_result concrete_rule_successor_check(rapidjson::Document const &conf) {
            unordered_map<string, int> pre_map;
            const Value &source = conf["source"];
            const Value &sink = conf["sink"];
            const Value &graph = conf["graph"];

            //check max successor of source nodes
            for (rapidjson::SizeType i = 0; i < source.Size(); i++) {
                if(source[i]["output"].HasMember("conf")) {
                    if(!source[i]["output"]["conf"].IsObject()) {
                        return validation_result(2, "Error: source node output conf should be an object");
                    }
                    else if(source[i]["output"]["conf"].HasMember("output_port_num")) {
                        if(!source[i]["output"]["conf"]["output_port_num"].IsInt()) {
                            return validation_result(2, "Error: source node output_port_num should be integer");
                        }
                        else if(source[i]["output"]["conf"]["output_port_num"].GetInt() < 0) {
                            return validation_result(2, "Error: source node output_port_num should not be negative");
                        }
                        string id = source[i]["id"].GetString();
                        pre_map[id] = source[i]["output"]["conf"]["output_port_num"].GetInt();
                    }
                }
            }

            //check max successor of graph nodes
            for (rapidjson::SizeType i = 0; i < graph.Size(); i++) {
                if(graph[i]["output"].HasMember("conf")) {
                    if(!graph[i]["output"]["conf"].IsObject()) {
                        return validation_result(2, "Error: graph node output conf should be an object");
                    }
                    else if(graph[i]["output"]["conf"].HasMember("output_port_num")) {
                        if(!graph[i]["output"]["conf"]["output_port_num"].IsInt()) {
                            return validation_result(2, "Error: graph node output_port_num should be integer");
                        }
                        else if(graph[i]["output"]["conf"]["output_port_num"].GetInt() < 0) {
                            return validation_result(2, "Error: graph node output_port_num should not be negative");
                        }
                        string id = graph[i]["id"].GetString();
                        pre_map[id] = graph[i]["output"]["conf"]["output_port_num"].GetInt();
                    }
                }
            }

            // check predecessors in graph
            for (rapidjson::SizeType i = 0; i < graph.Size(); i++) {
                const Value &pre = graph[i]["input"]["pre"];
                if (!pre.IsArray()) {
                    string pre_id = pre.GetString();
                    if(pre_map.count(pre_id) != 0) {
                        pre_map[pre_id]--;
                        if(pre_map[pre_id] < 0)
                            return validation_result(2, "Error: number of successors of node \"" + pre_id + "\" exceeds maximum value");
                    }
                } else {
                    for (rapidjson::SizeType j = 0; j < pre.Size(); j++) {
                        string pre_id = pre[j].GetString();
                        if(pre_map.count(pre_id) != 0) {
                            pre_map[pre_id]--;
                            if(pre_map[pre_id] < 0)
                                return validation_result(2, "Error: number of successors of node \"" + pre_id + "\" exceeds maximum value");
                        }
                    }
                }
            }

            // check predecessors in sink
            for (rapidjson::SizeType i = 0; i < sink.Size(); i++) {
                const Value &pre = sink[i]["input"]["pre"];
                if (!pre.IsArray()) {
                    string pre_id = pre.GetString();
                    if(pre_map.count(pre_id) != 0) {
                        pre_map[pre_id]--;
                        if(pre_map[pre_id] < 0)
                            return validation_result(2, "Error: number of successors of node \"" + pre_id + "\" exceeds maximum value");
                    }
                } else {
                    for (rapidjson::SizeType j = 0; j < pre.Size(); j++) {
                        string pre_id = pre[j].GetString();
                        if(pre_map.count(pre_id) != 0) {
                            pre_map[pre_id]--;
                            if(pre_map[pre_id] < 0)
                                return validation_result(2, "Error: number of successors of node \"" + pre_id + "\" exceeds maximum value");
                        }
                    }
                }
            }

            return validation_result(0, "Pass: successors number check");
        }

        //TODO unittest
        validation_result concrete_rule_library_search(rapidjson::Document const &conf) {
            const Value &source = conf["source"];
            const Value &sink = conf["sink"];
            const Value &graph = conf["graph"];

            //check dylib in source nodes
            for (rapidjson::SizeType i = 0; i < source.Size(); i++) {
                string file = source[i]["task"]["dylib"]["location"].GetString();
                string id = source[i]["id"].GetString();

                boost::filesystem::path filePath(file);

                if (!boost::filesystem::exists(filePath)) {
                    return validation_result(2, "Error: dylib not found in source node \"" + id + "\"");
                }
                else if (!boost::filesystem::is_regular_file(filePath)) {
                    return validation_result(2, "Error: dylib not found in source node \"" + id + "\"");
                }
                else {
                    string extension = filePath.extension().string();
                    if (extension != ".dll" || extension != ".so" || extension != ".dylib") {
                        return validation_result(2, "Error: dylib format incorrect in source node \"" + id + "\"");
                    }
                }
            }

            //check dylib in sink nodes
            for (rapidjson::SizeType i = 0; i < sink.Size(); i++) {
                string file = sink[i]["task"]["dylib"]["location"].GetString();
                string id = sink[i]["id"].GetString();

                boost::filesystem::path filePath(file);

                if (!boost::filesystem::exists(filePath)) {
                    return validation_result(2, "Error: dylib not found in sink node \"" + id + " \"");
                }
                else if( !boost::filesystem::is_regular_file(filePath)) {
                    return validation_result(2, "Error: dylib not found in sink node \"" + id + "\"");
                }
                else {
                    string extension = filePath.extension().string();
                    if (extension != ".dll" || extension != ".so" || extension != ".dylib") {
                        return validation_result(2, "Error: dylib format incorrect in sink node \"" + id + " \"");
                    }
                }
            }

            //check algorithm in graph nodes
            for (rapidjson::SizeType i = 0; i < graph.Size(); i++) {
                string file = graph[i]["task"]["algorithm"].GetString();
                string id = graph[i]["id"].GetString();


                #ifdef _WIN32
                string defaultPath = ".\algorithms\";
                #else
                string defaultPath = "./algorithms/";
                #endif

                boost::filesystem::path filePath(file);
                boost::filesystem::path filePath2(defaultPath+file);

                if (!boost::filesystem::exists(filePath) || !boost::filesystem::exists(filePath2)) {
                    return validation_result(2, "Error: algorithm not found in sink node \"" + id + " \"");
                }
                else if (boost::filesystem::exists(filePath) && boost::filesystem::is_regular_file(filePath)) {
                    string extension = filePath.extension().string();
                    if (extension != ".dll" || extension != ".so" || extension != ".dylib") {
                        return validation_result(2, "Error: dylib format incorrect in sink node \"" + id + " \"");
                    }
                }
                else if (boost::filesystem::exists(filePath2) && boost::filesystem::is_regular_file(filePath2)) {
                    string extension = filePath.extension().string();
                    if (extension != ".dll" || extension != ".so" || extension != ".dylib") {
                        return validation_result(2, "Error: dylib format incorrect in sink node \"" + id + " \"");
                    }
                }
            }

            //TODO   func check

            return validation_result(0, "Pass: library location check");

        }

        validation_result concrete_rule_data_type(rapidjson::Document const & conf) {
            const unordered_set<string> types = {"any", "int", "string", "double"};
            const Value &source = conf["source"];

            //check dylib in source nodes
            for (rapidjson::SizeType i = 0; i < source.Size(); i++) {
                const Value &data = source[i]["output"]["data"];
                for (Value::ConstMemberIterator itr = data.MemberBegin(); itr != data.MemberEnd(); ++itr) {
                    string type = itr->value.GetString();
                    if(types.find(type) == types.end()) {
                        return validation_result(2, "Error: data type \"" +type + "\" unsupported");
                    }
                }
            }
            return validation_result(0, "Pass: data type check");
        }

        validation_result concrete_rule_mapping_check(rapidjson::Document const & conf) {
            const Value &sink = conf["sink"];
            const Value &source = conf["source"];
            const Value &graph = conf["graph"];

            unordered_map<string, vector<string>> output;

            //source output key
            for (rapidjson::SizeType i = 0; i < source.Size(); i++) {
                const Value &data = source[i]["output"]["data"];
                string id = source[i]["id"].GetString();
                for (Value::ConstMemberIterator itr = data.MemberBegin(); itr != data.MemberEnd(); ++itr) {
                    string key = itr->name.GetString();
                    output[id].push_back(key);
                }
            }

            //graph output key
            for (rapidjson::SizeType i = 0; i < graph.Size(); i++) {
                if(graph[i]["output"].HasMember("mapping")) {
                    string id = graph[i]["id"].GetString();
                    const Value &mapping = graph[i]["output"]["mapping"];
                    if(!mapping.IsObject()) {
                        return validation_result(2, "Error: mapping field should be an object \"" + id + "\"");
                    }

                    for (Value::ConstMemberIterator itr = mapping.MemberBegin(); itr != mapping.MemberEnd(); ++itr) {
                        if(!itr->value.IsString())
                            return validation_result(2, "Error: values in mapping field should be string \"" + id + "\"");
                        string key = itr->name.GetString();
                        output[id].push_back(key);
                    }
                }
            }


            //graph input key
            for (rapidjson::SizeType i = 0; i < graph.Size(); i++) {
                if(graph[i]["input"].HasMember("mapping")) {

                    string id = graph[i]["id"].GetString();
                    const Value &mapping = graph[i]["input"]["mapping"];
                    if(!mapping.IsObject()) {
                        return validation_result(2, "Error: mapping field should be an object \"" + id + "\"");
                    }

                    // check predecessor for graph
                    unordered_multiset<string> pre_keys;
                    const Value &pre = graph[i]["input"]["pre"];
                    if (!pre.IsArray()) {
                        for(auto mm : output[pre.GetString()])
                            pre_keys.insert(mm);
                    }
                    else {
                        for (rapidjson::SizeType j = 0; j < pre.Size(); j++) {
                            for(auto mm : output[pre[j].GetString()])
                                pre_keys.insert(mm);
                        }
                    }

                    for (Value::ConstMemberIterator itr = mapping.MemberBegin(); itr != mapping.MemberEnd(); ++itr) {
                        if(itr->value.IsArray()) {
                            for (rapidjson::SizeType j = 0; j < (itr->value).Size(); j++) {
                                if(!(itr->value)[j].IsString())
                                    return validation_result(2, "Error: values in mapping field should be string \"" + id + "\"");
                                string value = (itr->value)[j].GetString();
                                if(value.find_last_of(".") == string::npos) {
                                    return validation_result(2, "Error: mapping wrong format \"" + id + "\"");
                                }
                                size_t pos = value.find_last_of(".");
                                string cnt = value.substr(pos+1);
                                string pre_key = value.substr(0, pos);
                                if(!all_of(cnt.begin(), cnt.end(), ::isdigit) && cnt != "all") {
                                    return validation_result(2, "Error: mapping wrong format \"" + id + "\"");
                                }

                                if(cnt != "all" && (atoi(cnt.c_str()) > pre_keys.count(pre_key) || pre_keys.count(pre_key) == 0)) {
                                    return validation_result(2, "Error: mapping key unfit \"" + id + "\"");
                                }
                            }
                        }
                        else {
                            if(!itr->value.IsString())
                                return validation_result(2, "Error: values in mapping field should be string \"" + id + "\"");
                            string value = itr->value.GetString();
                            if(value.find_last_of(".") == string::npos) {
                                return validation_result(2, "Error: mapping wrong format \"" + id + "\"");
                            }
                            size_t pos = value.find_last_of(".");
                            string cnt = value.substr(pos+1);
                            string pre_key = value.substr(0, pos);
                            if(!all_of(cnt.begin(), cnt.end(), ::isdigit) && cnt != "all") {
                                return validation_result(2, "Error: mapping wrong format \"" + id + "\"");
                            }

                            if(cnt != "all" && (atoi(cnt.c_str()) > pre_keys.count(pre_key) || pre_keys.count(pre_key) == 0))  {
                                return validation_result(2, "Error: mapping key unfit \"" + id + "\"");
                            }
                        }

                    }
                }

            }

            //sink input key
            for (rapidjson::SizeType i = 0; i < sink.Size(); i++) {
                if(sink[i]["input"].HasMember("mapping")) {

                    string id = sink[i]["id"].GetString();
                    const Value &mapping = sink[i]["input"]["mapping"];
                    if(!mapping.IsObject()) {
                        return validation_result(2, "Error: mapping field should be an object \"" + id + "\"");
                    }

                    // check predecessor for sink
                    unordered_multiset<string> pre_keys;
                    const Value &pre = sink[i]["input"]["pre"];
                    if (!pre.IsArray()) {
                        for(auto mm : output[pre.GetString()])
                            pre_keys.insert(mm);
                    }
                    else {
                        for (rapidjson::SizeType j = 0; j < pre.Size(); j++) {
                            for(auto mm : output[pre[j].GetString()])
                                pre_keys.insert(mm);
                        }
                    }

                    for (Value::ConstMemberIterator itr = mapping.MemberBegin(); itr != mapping.MemberEnd(); ++itr) {
                        if((itr->value).IsArray()) {
                            for (rapidjson::SizeType j = 0; j < (itr->value).Size(); j++) {
                                if(!(itr->value)[j].IsString())
                                    return validation_result(2, "Error: values in mapping field should be string \"" + id + "\"");
                                string value = (itr->value)[j].GetString();
                                if(value.find_last_of(".") == string::npos) {
                                    return validation_result(2, "Error: mapping wrong format \"" + id + "\"");
                                }
                                size_t pos = value.find_last_of(".");
                                string cnt = value.substr(pos+1);
                                string pre_key = value.substr(0, pos);
                                if(!all_of(cnt.begin(), cnt.end(), ::isdigit) && cnt != "all") {
                                    return validation_result(2, "Error: mapping wrong format \"" + id + "\"");
                                }

                                if(cnt != "all" && (atoi(cnt.c_str()) > pre_keys.count(pre_key) || pre_keys.count(pre_key) == 0))  {
                                    return validation_result(2, "Error: mapping key unfit \"" + id + "\"");
                                }
                            }
                        }
                        else {

                            if(!(itr->value).IsString())
                                return validation_result(2, "Error: values in mapping field should be string \"" + id + "\"");
                            string value = itr->value.GetString();

                            if(value.find_last_of(".") == string::npos) {
                                return validation_result(2, "Error: mapping wrong format \"" + id + "\"");
                            }
                            size_t pos = value.find_last_of(".");
                            string cnt = value.substr(pos+1);
                            string pre_key = value.substr(0, pos);

                            if(!all_of(cnt.begin(), cnt.end(), ::isdigit) && cnt != "all") {
                                return validation_result(2, "Error: mapping wrong format \"" + id + "\"");
                            }

                            if(cnt != "all" && (atoi(cnt.c_str()) > pre_keys.count(pre_key) || pre_keys.count(pre_key) == 0))  {
                                return validation_result(2, "Error: mapping key unfit \"" + id + "\"");
                            }
                        }

                    }
                }

            }

            return validation_result(0, "Pass: mapping field check");
        }


    }



PVPF_NAMESPACE_END







