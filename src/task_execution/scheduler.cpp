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
#include <pvpf/task_execution/context_generator.hpp>

#define BUFFER_SIZE 10
#define JOIN_SIZE 5

using namespace tbb;
using namespace std;
using namespace rapidjson;
using namespace os_agnostic;


PVPF_NAMESPACE_BEGIN
    namespace task_execution {
        void scheduler::build_graph(rapidjson::Document &conf) {

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

        void scheduler::source_node_list(flow::graph &graph, const Value &conf) {

            for (Value::ConstValueIterator it = conf.Begin(); it != conf.End(); it++) {
//                auto pair;
                shared_ptr<context> c = create_context(*(it), *this);
                string id = (*it)["id"].GetString();

                unique_ptr<logical_source_node> logi_source = generate_source_node((*it), c);

                cout << logi_source.get() << endl;

                source_node_map[id] = std::move(logi_source);

                cout << "logical_node is in the map now" << endl;
            }
            cout << "source node generate finish" << endl;
        }

        void scheduler::start_source_functions() {
            for (auto &it : source_pipe_map) {
                thread t(run_io_func, it.first, move(it.second));
                thread_vector.push_back(std::move(t));
            }
        }

        void run_io_func(int id, std::unique_ptr<pvpf::data_io::source_io_pipe> pipe) {
            os_agnostic::dynamic_lib_func_manager &manager = os_agnostic::dynamic_lib_func_manager::get_instance();
            manager.invoke_io_func(id, *(pipe.get()));
//            std::terminate();
        };

        std::unique_ptr<executable> scheduler::generate_executable(rapidjson::Value const &obj) {

        }

//        unique_ptr<executable> scheduler::graph_node_list(unordered_map<string, logical_node> &nodes,
//                                                          flow::graph &graph, const Value &conf) {
//
//        }
//
//        void scheduler::sink_node_list(unordered_map<std::string, logical_node> &nodes,
//                                       flow::graph &graph, const Value &conf) {
//            //generate sink node
//            for (Value::ConstValueIterator it = conf.Begin(); it != conf.End(); it++) {
//                auto pair = pvpf::data_io::create_sink(BUFFER_SIZE, true);
//
//                // TODO 1. generate source/sink threads calling the source/sink library and pass io pipe to the library.
//            }
//        }

        unique_ptr<pvpf::task_execution::logical_source_node>
        scheduler::generate_source_node(const Value &obj, shared_ptr<context> cont) {
            os_agnostic::dynamic_lib_func_manager &manager = os_agnostic::dynamic_lib_func_manager::get_instance();
            cout << "generate pipe" << endl;
            bool flag;
            if (obj.HasMember("control") && obj["control"].HasMember("block") &&
                obj["control"]["block"].GetBool() == false) {
                flag = false;
            } else {
                flag = true;
            }

            tuple<unique_ptr<data_io::source_io_pipe>, unique_ptr<data_io::io_pipe_for_source_node>> pair = pvpf::data_io::create_source(
                    BUFFER_SIZE, flag);

            cout << "generate body" << endl;
            io_body ib(cont, std::move(get<1>(pair)));

            cout << "generate node" << endl;

            unique_ptr<flow::source_node<pvpf::data_bucket>> node(
                    new flow::source_node<pvpf::data_bucket>(graph, ib, false));

            cout << "generate logical_source_node" << endl;

            auto logi_source = std::make_unique<logical_source_node>(move(node), cont);

            path location(obj["task"]["dylib"]["location"].GetString());

            cout << "location is " << location << endl;

            string name = (obj["task"]["dylib"]["func"].GetString());

            cout << "function name is " << name << endl;

            int func_id = manager.load_input_func(location, name);

            cout << "function load finish, id is " << func_id << endl;

            source_pipe_map[func_id] = std::move(get<0>(pair));

            cout << "generate node is good" << endl;
            //3. generate sink node, add the sink_node to the map

            return logi_source;
        }

//        unique_ptr<logical_node>
//        scheduler::generate_graph_node(flow::graph &graph, const Value &conf, shared_ptr<context> context) {
//
//            unique_ptr<executable> exec(new normal_algorithm);
//
//            body b(context, std::move(exec));
//
//            unique_ptr<flow::function_node<data_bucket>> temp(
//                    new flow::function_node<data_bucket>(graph, flow::unlimited, b));
//
//            unique_ptr<logical_node> result(new logical_node(temp.get(), context));
//
//            int join_size = context.get()->succ.size();
//
//            result->join_size = join_size;
//
//            if (join_size > 1 && join_size < JOIN_SIZE) {
//                unique_ptr<flow::join_node<array<data_bucket, 1>>> join = create_join_node(graph, join_size);
//
//                result->join = std::move(join);
//
//            }
//
//            int split_size = context.get()->pre.size();
//
//            result->split_size = split_size;
//
//            if (split_size > 1) {
//                unique_ptr<flow::split_node<array<data_bucket, 1>>> split(
//                        new flow::split_node<array<data_bucket, 1>>(graph));
//            }
//
//
//
//
//
////            flow::split_node
//
////            result -> source()
//
//            return result;
//        }

//        unique_ptr<logical_node>
//        scheduler::generate_sink_node(flow::graph &graph, const Value &conf, shared_ptr<context> context) {
//            //2. generate func_node
//
//            //add context of the source node to the system
//
//            //find the algorithm to build up the executable
//
//            //construct body use context and executable
//
//            return unique_ptr<logical_node>();
//        }


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

        void scheduler::stop_source_functions() {
            for (int i = 0; i < thread_vector.size(); i++) {
                thread_vector[i].join();
            }
        }


    }

PVPF_NAMESPACE_END