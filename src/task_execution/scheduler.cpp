//
// Created by jiabei on 2018/7/5.
//
#include <pvpf/task_execution/scheduler.hpp>
#include <pvpf/task_execution/body.hpp>
#include <string>
#include <boost/algorithm/string.hpp>
#include <pvpf/data_io/factory.hpp>
#include <tbb/flow_graph.h>
#include <iostream>
#include <pvpf/config/config_reader.hpp>

#define BUFFER_SIZE 10
#define JOIN_SIZE 5

using namespace tbb;
using namespace std;
using namespace rapidjson;


PVPF_NAMESPACE_BEGIN
    namespace task_execution {
        void scheduler::build_graph(rapidjson::Document &conf) {

            unordered_map<string, unique_ptr<logical_node>> node_map;

            vector<flow::source_node<data_bucket>> sources;
            const Value &source_json_list = conf["source"];
            const Value &graph_json_list = conf["graph"];
            const Value &sink_json_list = conf["sink"];




            //generate source node
            /* construct the rules
             * check if it is multi pre, if so, add a join node
             * traversal the list to generate a list of nodes that predecessor is this node if the size of the list is greater than 1
             * add a new split node
             * */


        }

        void scheduler::source_node_list(unordered_map<string, logical_node> &nodes,
                                         flow::graph &graph, const Value &conf) {
            // TODO 1. generate source/sink threads calling the source/sink library and pass io pipe to the library.
//            for (Value::ConstValueIterator it = conf.Begin(); it != conf.End(); it++) {
//                auto pair;
//                if (it->HasMember("control") && it["control"]->HasMember("block") &&
//                    (*it)["control"]["block"].GetBool() == false) {
//                    pair = pvpf::data_io::create_source(BUFFER_SIZE, false);
//                } else {
//                    pair = pvpf::data_io::create_source(BUFFER_SIZE, true);
//                }
//
//
//                body body(context, exec);
//                //3. generate sink node, add the sink_node to the map
//            }

        }

        void scheduler::graph_node_list(unordered_map<string, logical_node> &nodes,
                                        flow::graph &graph, const Value &conf) {

        }

        void scheduler::sink_node_list(unordered_map<std::string, logical_node> &nodes,
                                       flow::graph &graph, const Value &conf) {
            //generate sink node
            for (Value::ConstValueIterator it = conf.Begin(); it != conf.End(); it++) {
                auto pair = pvpf::data_io::create_sink(BUFFER_SIZE, true);

                // TODO 1. generate source/sink threads calling the source/sink library and pass io pipe to the library.
            }
        }

        unique_ptr<logical_node>
        scheduler::generate_source_node(flow::graph &graph, const Value &conf, shared_ptr<context> context) {

            return unique_ptr<logical_node>();
        }

        unique_ptr<logical_node>
        scheduler::generate_graph_node(flow::graph &graph, const Value &conf, shared_ptr<context> context) {

            unique_ptr<executable> exec(new normal_algorithm);

            body b(context, std::move(exec));

            unique_ptr<flow::function_node<data_bucket>> temp(
                    new flow::function_node<data_bucket>(graph, flow::unlimited, b));

            unique_ptr<logical_node> result(new logical_node(temp.get(), context));

            int join_size = context.get()->succ.size();

            result->join_size = join_size;

            if (join_size > 1 && join_size < JOIN_SIZE) {
                unique_ptr<flow::join_node<array<data_bucket, 1>>> join = create_join_node(graph, join_size);

                result->join = std::move(join);

            }

            int split_size = context.get()->pre.size();

            result->split_size = split_size;

            if (split_size > 1) {
                unique_ptr<flow::split_node<array<data_bucket, 1>>> split(
                        new flow::split_node<array<data_bucket, 1>>(graph));
            }





//            flow::split_node

//            result -> source()

            return result;
        }

        unique_ptr<logical_node>
        scheduler::generate_sink_node(flow::graph &graph, const Value &conf, shared_ptr<context> context) {
            //2. generate func_node

            //add context of the source node to the system

            //find the algorithm to build up the executable

            //construct body use context and executable

            return unique_ptr<logical_node>();
        }

        void scheduler::figure_out_json_object(Document &conf) {
            for (Value::ConstValueIterator source = conf["source"].Begin();
                 source != conf["source"].End(); source++) {
                json_object_map[(*source)["id"].GetString()] = (*source);
            }

            for (Value::ConstValueIterator node = conf["graph"].Begin();
                 node != conf["graph"].End(); node++) {
                json_object_map[(*node)["id"].GetString()] = (*node);
            }

            for (Value::ConstValueIterator sink = conf["sink"].Begin(); sink != conf["sink"].End(); sink++) {
                json_object_map[(*sink)["id"].GetString()] = (*sink);
            }
        }

        shared_ptr<context>
        scheduler::create_context(const rapidjson::Value &obj, rapidjson::Document &conf) {


            string id = obj["id"].GetString();

            const Value &graph_json_list = conf["graph"];
            const Value &sink_json_list = conf["sink"];

//            cout<<"succesors"<<endl;
            //add to successors
            vector<string> successors;
            for (Value::ConstValueIterator node = graph_json_list.Begin(); node != graph_json_list.End(); node++) {
                const Value &pre_list = (*node)["input"]["pre"];
                for (Value::ConstValueIterator pre_it = pre_list.Begin(); pre_it != pre_list.End(); pre_it++) {
                    if (pre_it->GetString() == id) {
                        successors.push_back(move((*node)["id"].GetString()));
                        break;
                    }
                }
            }

            for (Value::ConstValueIterator node = sink_json_list.Begin(); node != sink_json_list.End(); node++) {
                const Value &pre_list = (*node)["input"]["pre"];
                for (Value::ConstValueIterator pre_it = pre_list.Begin(); pre_it != pre_list.End(); pre_it++) {
                    if (pre_it->GetString() == id) {
                        successors.push_back(move((*node)["id"].GetString()));
                        break;
                    }
                }
            }

            //add is_cpu to context;
            bool this_is_cpu = false;
            if (obj["task"].HasMember("algorithm")) {
                this_is_cpu = is_cpu(obj["task"]["algorithm"].GetString());
            }


//          cout<<"pre"<<endl;
//          add to pre{}
            //add pre list and configure for pre.

            vector<string> pre;
            vector<bool> pre_is_cpu;
            if (obj.HasMember("input")) {
                const Value &pre_list = obj["input"]["pre"];
                for (Value::ConstValueIterator pre_it = pre_list.Begin(); pre_it != pre_list.End(); pre_it++) {
                    string id = pre_it->GetString();
                    pre.push_back(move(id));
                    if (json_object_map[id]["task"].HasMember("algorithm")) {
                        pre_is_cpu.push_back(is_cpu(json_object_map[id]["task"]["algorithm"].GetString()));
                    } else {
                        pre_is_cpu.push_back(true);
                    }
                }
            }

            vector<bool> read_only;
            if (obj["input"].HasMember("pre_conf")) {
                for (Value::ConstValueIterator conf_it = obj["input"]["pre_conf"].Begin();
                     conf_it != obj["input"]["pre_conf"].End(); conf_it++) {
                    read_only.push_back((*conf_it)["readonly"].GetBool());
                }
            } else {
                for (int i = 0; i < pre.size(); i++) {
                    read_only.push_back(true);
                }
            }







            //add to input mapping
            unordered_map<string, vector<pair<int, string>>> input;
            if (obj["input"].HasMember("mapping")) {
                const Value &input_list = obj["input"]["mapping"];
                for (Value::ConstMemberIterator input_it = input_list.MemberBegin();
                     input_it != input_list.MemberEnd(); input_it++) {
                    vector<pair<int, string>> value_list;
                    if (input_it->value.IsArray()) {
                        const Value &key_list = input_it->value;
                        for (Value::ConstValueIterator key_it = key_list.Begin(); key_it != key_list.End(); key_it++) {
                            vector<std::pair<int, std::string>> result;
                            result = analyze_mapping_value((*key_it).GetString(), pre.size());
                            value_list.insert(value_list.end(), result.begin(), result.end());
                        }
                    } else {
                        value_list = analyze_mapping_value(input_it->value.GetString(), pre.size());
                    }

                    input[input_it->name.GetString()] = move(value_list);
                }
            }


//            cout<<"output"<<endl;
            //add to output
            unordered_map<string, string> output;
            unordered_map<string, string> data;
            if (obj.HasMember("output")) {
                if (obj["output"].HasMember("mapping")) {
                    const Value &output_list = obj["output"]["mapping"];
                    for (Value::ConstMemberIterator keys = output_list.MemberBegin();
                         keys != output_list.MemberEnd(); keys++) {
                        output[keys->name.GetString()] = keys->value.GetString();
                    }
                }

                if (obj["output"].HasMember("data")) {
                    const Value &data_list = obj["output"]["mapping"];
                    for (Value::ConstMemberIterator data_it = data_list.MemberBegin();
                         data_it != data_list.MemberEnd(); data_it++) {
                        data[data_it->name.GetString()] = data_it->value.GetString();
                    }
                }
            }


            auto cont = make_shared<context>(id, pre, successors, input, output, data, read_only, pre_is_cpu,
                                             this_is_cpu);

//            cout<<"finish"<<endl;
            return cont;
        }

        std::vector<std::pair<int, std::string>> scheduler::analyze_mapping_value(std::string value, int size) {
            vector<std::pair<int, std::string>> result;
            vector<string> split_result;
            string key = value;
            boost::algorithm::split(split_result, key, boost::is_any_of("."));
            if (split_result[1].compare("all")) {
                for (int i = 0; i < size; i++) {
                    pair<int, string> p(i, split_result[0]);
                    result.push_back(move(p));
                }
            } else {
                pair<int, string> p(stoi(split_result[1]), split_result[0]);
                result.push_back(move(p));
            }
            return result;
        }

        std::unique_ptr<tbb::flow::join_node<std::array<data_bucket, 1>>>
        scheduler::create_join_node(tbb::flow::graph &graph, int size) {
//            switch (size) {
//                case 1:
//                    return unique_ptr<flow::join_node<std::array<data_bucket, 1>>>(
//                            new flow::join_node<std::array<data_bucket, 1>>(graph));
//                case 2:
//                    return unique_ptr<flow::join_node<std::array<data_bucket, 1>>>(
//                            new flow::join_node<std::array<data_bucket, 2>>(graph));
//                case 3:
//                    return unique_ptr<flow::join_node<std::array<data_bucket, 1>>>(
//                            new flow::join_node<std::array<data_bucket, 3>>(graph));
//                case 4:
//                    return unique_ptr<flow::join_node<std::array<data_bucket, 1>>>(
//                            new flow::join_node<std::array<data_bucket, 4>>(graph));
//                case 5:
//                    return unique_ptr<flow::join_node<std::array<data_bucket, 1>>>(
//                            new flow::join_node<std::array<data_bucket, 5>>(graph));
//            }
        }

        bool scheduler::is_cpu(std::string algorithm_name) {
            config::config_reader cr;
            Document d = cr.load_json_conf("algorithm_name");
            //TODO change algorithm name to real name;
            const Value &meta = d["meta"];
            if (meta.HasMember("gpu")) {
                return !meta["gpu"].GetBool();
            }
            return true;
        }
    }

PVPF_NAMESPACE_END