//
// Created by jiabei on 2018/7/5.
//
#include "pvpf/task_execution/scheduler.hpp"
#include <pvpf/data_io>
#include <pvpf/task_execution/body.hpp>
#define BUFFER_SIZE 10

using namespace tbb;
using namespace std;
using namespace rapidjson;


PVPF_NAMESPACE_BEGIN
    namespace scheduler{
        void scheduler::build_graph(rapidjson::Document& conf){
            //TODO add a map to map between node and node id

            vector<flow::source_node> sources;
            const Value& source_json_list = conf["source"];
            const Value& graph_json_list = conf["graph"];
            const Value& sink_json_list = conf["sink"];

            //generate source node
            for (Value::ConstValueIterator it = source_json_list.Begin(); it != source_json_list.End(); it ++){
                auto pair;
                if (it -> HasMember("control") && it["control"] -> HasMember("block") && (*it)["control"]["block"].GetBool() == false){
                    pair = pvpf::data_io::create_source(BUFFER_SIZE, false);
                } else {
                    pair = pvpf::data_io::create_source(BUFFER_SIZE, true);
                }
                // TODO 1. generate source/sink threads calling the source/sink library and pass io pipe to the library.

                // 2. add information needed by context
                string id = (*it)["id"];
                vector<string> successors;
                for (Value::ConstValueIterator node = graph_json_list.Begin(); node != graph_json_list.End(); node ++){
                    const Value& pre_list =  (*node)["input"]["pre"];
                    for (Value::ConstValueIterator pre_it = pre_list.Begin(); pre_it != pre_list.End(); pre_it ++){
                        if (pre_it -> GetString() == id) {
                            successors.push_back((*node)["id"]);
                            break;
                        }
                    }
                }
                string pre[0];
                string succ[successors.size()];
                unordered_map<string, tuple<int, string>> input;
                unordered_map<string, string> output;
                if (successors.size() > 0){
                    //TODO add a split node
                }
                const Value& data_list = (*it)["output"]["data"];
                for (Value::ConstValueIterator data = data_list.MemberBegin(); data != data_list.MemberEnd(); data ++){
                    output[data -> name.GetString()] = data -> value.GetString();
                }
                context context(id, pre, succ, input, output);
                body body(context, exec);
                //3. generate source node, add the source_node to the map
            }


            //generate sink node
            for (Value::ConstValueIterator it = sink_json_list.Begin(); it != sink_json_list.End(); it ++){
                auto pair = pvpf::data_io::create_sink(BUFFER_SIZE, true);

                // TODO 1. generate source/sink threads calling the source/sink library and pass io pipe to the library.

                // 2. add information needed by context
                string id = (*it)["id"];
                vector<string> successors;
                const Value& pre_list = (*it)["input"]["pre"];
                string pre[pre_list.size()];
                int index = 0;
                for (Value::ConstValueIterator pre_it = pre_list.Begin(); pre_it != pre_list.End(); pre_it ++){
                    pre[index++] = pre_it -> GetString();
                }
                if (pre_list.size() > 0){
                    //TODO add a join node and add edge with them
                }
                string succ[0];
                unordered_map<string, tuple<int, string>> input;// some problem
                unordered_map<string, string> output;
                const Value& input_list = (*it)["input"]["mapping"];
                for (Value::ConstValueIterator input_it = input_list.MemberBegin(); input_it != input_list.MemberEnd(); input_it ++){
                    if (! input_it -> isArray()){
                        if ()
                        input[input_it -> name.GetString()] =
                    } else {

                    }

                }


                context context(id, pre, succ, input, output);
                body body(context, exec);
                //3. generate sink node, add the sink_node to the map
            }

            /* construct the rules
             * check if it is multi pre, if so, add a join node
             * traversal the list to generate a list of nodes that predecessor is this node if the size of the list is greater than 1
             * add a new split node
             * */

            //2. generate func_node

            //add context of the source node to the system

            //find the algorithm to build up the executable

            //construct body use context and executable

        }
    };
PVPF_NAMESPACE_END