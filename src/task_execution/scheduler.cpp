//
// Created by jiabei on 2018/7/5.
//
#include "pvpf/task_execution/scheduler.hpp"
#include <pvpf/task_execution/body.hpp>
#include <string>
#include <boost/algorithm/string.hpp>
#include <pvpf/data_io/factory.hpp>
#include <tbb/flow_graph.h>

#define BUFFER_SIZE 10

using namespace tbb;
using namespace std;
using namespace rapidjson;


PVPF_NAMESPACE_BEGIN
    namespace scheduler {
        void scheduler::build_graph(rapidjson::Document &conf) {

            unordered_map<string, unique_ptr<flow::graph_node>> node_map;

            vector<flow::source_node> sources;
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
            for (Value::ConstValueIterator it = conf.Begin(); it != conf.End(); it++) {
                auto pair;
                if (it->HasMember("control") && it["control"]->HasMember("block") &&
                    (*it)["control"]["block"].GetBool() == false) {
                    pair = pvpf::data_io::create_source(BUFFER_SIZE, false);
                } else {
                    pair = pvpf::data_io::create_source(BUFFER_SIZE, true);
                }


                body body(context, exec);
                //3. generate sink node, add the sink_node to the map
            }

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

        unique_ptr<flow::source_node>
        scheduler::generate_source_node(flow::graph &graph, const Value &conf, context &context) {
            return unique_ptr<flow::source_node>();
        }

        unique_ptr<flow::function_node>
        scheduler::generate_graph_node(flow::graph &graph, const Value &conf, context &context) {
            return unique_ptr<flow::function_node>();
        }

        unique_ptr<flow::function_node>
        scheduler::generate_sink_node(flow::graph &graph, const Value &conf, context &context) {
            //2. generate func_node

            //add context of the source node to the system

            //find the algorithm to build up the executable

            //construct body use context and executable

            return unique_ptr<flow::function_node>();
        }

        shared_ptr<context>
        scheduler::create_context(const rapidjson::Value &obj, rapidjson::Document &conf) {

            string id = obj["id"].GetString();

            const Value &graph_json_list = conf["graph"];
            const Value &sink_json_list = conf["sink"];

            //add to successors
            vector<string> successors;
            for (Value::ConstValueIterator node = graph_json_list.Begin(); node != graph_json_list.End(); node++) {
                const Value &pre_list = (*node)["input"]["pre"];
                for (Value::ConstValueIterator pre_it = pre_list.Begin(); pre_it != pre_list.End(); pre_it++) {
                    if (pre_it->GetString() == id) {
                        successors.push_back((*node)["id"].GetString());
                        break;
                    }
                }
            }

            for (Value::ConstValueIterator node = sink_json_list.Begin(); node != sink_json_list.End(); node++) {
                const Value &pre_list = (*node)["input"]["pre"];
                for (Value::ConstValueIterator pre_it = pre_list.Begin(); pre_it != pre_list.End(); pre_it++) {
                    if (pre_it->GetString() == id) {
                        successors.push_back((*node)["id"].GetString());
                        break;
                    }
                }
            }

            //add to pre
            const Value &pre_list = obj["input"]["pre"];
            vector<string> pre;
            for (Value::ConstValueIterator pre_it = pre_list.Begin(); pre_it != pre_list.End(); pre_it++) {
                pre.push_back(pre_it->GetString());
            }


            unordered_map<string, vector<pair<int, string>>> input;
            unordered_map<string, string> output;
            //TODO mapping between input and output

            const Value &data_list = obj["output"]["data"];
            for (Value::ConstMemberIterator data = data_list.MemberBegin(); data != data_list.MemberEnd(); data++) {
                output[data->name.GetString()] = data->value.GetString();
            }

            shared_ptr<context> context = make_shared<context>(id, pre, successors, input, output);

            return context;

        }

    }

PVPF_NAMESPACE_END