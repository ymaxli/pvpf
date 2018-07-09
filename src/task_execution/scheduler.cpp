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

#define BUFFER_SIZE 10

using namespace tbb;
using namespace std;
using namespace rapidjson;


PVPF_NAMESPACE_BEGIN
    namespace task_execution {
        void scheduler::build_graph(rapidjson::Document &conf) {

            unordered_map<string, unique_ptr<flow::graph_node>> node_map;

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

        void scheduler::source_node_list(unordered_map<string, flow::graph_node> &nodes,
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

        void scheduler::graph_node_list(unordered_map<string, flow::graph_node> &nodes,
                                        flow::graph &graph, const Value &conf) {

        }

        void scheduler::sink_node_list(unordered_map<std::string, flow::graph_node> &nodes,
                                       flow::graph &graph, const Value &conf) {
            //generate sink node
            for (Value::ConstValueIterator it = conf.Begin(); it != conf.End(); it++) {
                auto pair = pvpf::data_io::create_sink(BUFFER_SIZE, true);

                // TODO 1. generate source/sink threads calling the source/sink library and pass io pipe to the library.
            }
        }

        unique_ptr<flow::source_node<data_bucket>>
        scheduler::generate_source_node(flow::graph &graph, const Value &conf, context &context) {
            return unique_ptr<flow::source_node<data_bucket>>();
        }

        unique_ptr<flow::function_node<data_bucket>>
        scheduler::generate_graph_node(flow::graph &graph, const Value &conf, context &context) {
            return unique_ptr<flow::function_node<data_bucket>>();
        }

        unique_ptr<flow::function_node<data_bucket>>
        scheduler::generate_sink_node(flow::graph &graph, const Value &conf, context &context) {
            //2. generate func_node

            //add context of the source node to the system

            //find the algorithm to build up the executable

            //construct body use context and executable

            return unique_ptr<flow::function_node<data_bucket>>();
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

//            cout<<"pre"<<endl;
            //add to pre
            const Value &pre_list = obj["input"]["pre"];
            vector<string> pre;
            for (Value::ConstValueIterator pre_it = pre_list.Begin(); pre_it != pre_list.End(); pre_it++) {
                pre.push_back(move(pre_it->GetString()));
            }

            //add to input mapping
            unordered_map<string, vector<pair<int, string>>> input;
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

//            cout<<"output"<<endl;
            //add to output
            unordered_map<string, string> output;
            const Value &data_list = obj["output"]["mapping"];
            for (Value::ConstMemberIterator data = data_list.MemberBegin(); data != data_list.MemberEnd(); data++) {
                output[data->name.GetString()] = data->value.GetString();
            }

            auto cont = make_shared<context>(id, pre, successors, input, output);

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


    }

PVPF_NAMESPACE_END