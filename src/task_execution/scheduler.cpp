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
using namespace config;


PVPF_NAMESPACE_BEGIN
    namespace task_execution {
        void scheduler::build_graph(rapidjson::Document &conf) {

            const Value &source_json_list = conf["source"];
            const Value &graph_json_list = conf["graph"];
            const Value &sink_json_list = conf["sink"];

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
                thread t(run_source_func, it.first, move(it.second));
                cout << "start one thread" << endl;
                thread_vector.push_back(std::move(t));
            }
        }

        void run_source_func(int id, std::unique_ptr<pvpf::data_io::source_io_pipe> pipe) {
            os_agnostic::dynamic_lib_func_manager &manager = os_agnostic::dynamic_lib_func_manager::get_instance();
            manager.invoke_io_func(id, *(pipe.get()));
        };

        void scheduler::start_sink_functions() {
            for (auto &it : sink_pipe_map) {
                thread t(run_sink_func, it.first, move(it.second));
                thread_vector.push_back(std::move(t));
            }
        }

        void run_sink_func(int id, std::unique_ptr<pvpf::data_io::sink_io_pipe> pipe) {
            os_agnostic::dynamic_lib_func_manager &manager = os_agnostic::dynamic_lib_func_manager::get_instance();
            manager.invoke_io_func(id, *(pipe.get()));
        };

        std::unique_ptr<executable> scheduler::generate_executable(rapidjson::Value const &obj) {
            os_agnostic::dynamic_lib_func_manager &manager = os_agnostic::dynamic_lib_func_manager::get_instance();
            config_reader cr;
            unique_ptr<abstract_algorithm> result;
            if (obj.HasMember("meta") && obj["meta"].HasMember("loop")) {
                cout<<"it is a loop"<<endl;
                unordered_map<string, string> loop_key_map;
                for (Value::ConstMemberIterator it = obj["meta"]["loop"].MemberBegin();
                     it != obj["meta"]["loop"].MemberEnd(); it++){
                    loop_key_map[it->name.GetString()] = it->value.GetString();
                }
                result = make_unique<loop_algorithm>(loop_key_map);
            } else {
                cout<<"it is a normal library"<<endl;
                result = make_unique<normal_algorithm>();
            }
            cout<<"main executable is ready"<<endl;
            for (Value::ConstValueIterator it = obj["body"].Begin(); it != obj["body"].End(); it++) {
                string type = (*it)["type"].GetString();
                if (type == "dylib") {
                    cout<<"it is dylib"<<endl;
                    string location = (*it)["location"].GetString();
                    string func = (*it)["func"].GetString();
                    dylib_func_ptr ptr = manager.load_algorithm(location, func);
                    unique_ptr<executable> temp = make_unique<dynamic_library_func>(ptr);
                    (*result.get()).add_executable(std::move(temp));
                } else if (type == "algorithm") {
                    cout<<"it is an algorithm"<<endl;
                    string path = "";
                    path = path + "./pvpf_algorithm/" + (*it)["algorithm"].GetString() + ".json";
                    Document d = cr.load_json_conf(path);
                    unique_ptr<executable> temp = generate_executable(d);
                    cout<<"algorithm generated"<<endl;
                    (*result.get()).add_executable(std::move(temp));
                    cout<<"move finished"<<endl;
                } else {
                    cout << "wrong type" << endl;
                }
            }
            return result;
        }

//        unique_ptr<executable> scheduler::graph_node_list(unordered_map<string, logical_node> &nodes,
//                                                          flow::graph &graph, const Value &conf) {
//
//        }
//
        void scheduler::sink_node_list(unordered_map<std::string, logical_node> &nodes,
                                       flow::graph &graph, const Value &conf) {
            for (Value::ConstValueIterator it = conf.Begin(); it != conf.End(); it++) {
//                auto pair;
                shared_ptr<context> c = create_context(*(it), *this);
                string id = (*it)["id"].GetString();

                unique_ptr<logical_node> logi_sink = generate_sink_node(graph, (*it), c);

                cout << logi_sink.get() << endl;

                node_map[id] = std::move(logi_sink);

                cout << "logical_node is in the map now" << endl;
            }
            cout << "sink node generate finish" << endl;
        }

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

        void scheduler::stop_io_threads() {
            for (int i = 0; i < thread_vector.size(); i++) {
                thread_vector[i].join();
            }
        }

        std::unique_ptr<pvpf::task_execution::logical_node>
        scheduler::generate_sink_node(tbb::flow::graph &graph, rapidjson::Value const &obj,
                                      std::shared_ptr<context> cont) {
            os_agnostic::dynamic_lib_func_manager &manager = os_agnostic::dynamic_lib_func_manager::get_instance();

            cout << "generate pipe" << endl;

            tuple<unique_ptr<data_io::sink_io_pipe>, unique_ptr<data_io::io_pipe_for_sink_node>> pair = pvpf::data_io::create_sink(
                    BUFFER_SIZE, true);

            cout << "generate body" << endl;
            sink_body sb(cont, std::move(get<1>(pair)));

            cout << "generate node" << endl;

            unique_ptr<flow::function_node<pvpf::data_bucket>> node(
                    new flow::function_node<pvpf::data_bucket>(graph, flow::unlimited, sb));

            path location(obj["task"]["dylib"]["location"].GetString());

            cout << "location is " << location << endl;

            string name = (obj["task"]["dylib"]["func"].GetString());

            cout << "function name is " << name << endl;

            int func_id = manager.load_input_func(location, name);

            cout << "function load finish, id is " << func_id << endl;

            sink_pipe_map[func_id] = std::move(get<0>(pair));

            cout << "generate logical_source_node" << endl;

            logical_node ln = {};

            switch ((int) cont.get()->pre.size()) {

                case 1: {
                    auto func1 = std::make_unique<function_node<data_bucket>>(graph, flow::unlimited,
                                                                              [](const data_bucket &t) {

                                                                              });
                    ln.wrap.size_1 = {
                            .cont = std::move(cont),
                            .func_node = std::move(func1),
                    };
                    break;

                }

                case 2: {
                    auto func2 = std::make_unique<function_node<array<data_bucket, 2>>>(graph, flow::unlimited,
                                                                                        [](const array<data_bucket, 2> &t) {

                                                                                        });
                    auto join2 = std::make_unique<join_node<array<data_bucket, 2>>>(graph);
                    ln.wrap.size_2 = {
                            .cont = std::move(cont),
                            .func_node = std::move(func2),
                            .join = std::move(join2)

                    };
                    break;
                }
                case 3: {
                    auto func3 = std::make_unique<function_node<array<data_bucket, 3>>>(graph, flow::unlimited,
                                                                                        [](const array<data_bucket, 3> &t) {

                                                                                        });
                    auto join3 = std::make_unique<join_node<array<data_bucket, 3>>>(graph);
                    ln.wrap.size_3 = {
                            .cont = std::move(cont),
                            .func_node = std::move(func3),
                            .join = std::move(join3)

                    };
                    break;
                }
                default: {
                    auto func4 = std::make_unique<function_node<data_bucket>>(graph, flow::unlimited,
                                                                              [](const data_bucket &t) {

                                                                              });
                    ln.wrap.size_1 = {
                            .cont = std::move(cont),
                            .func_node = std::move(func4),
                    };
                }
            }


            cout << "generate node is good" << endl;

            return unique_ptr<logical_node>();
        }


    }

PVPF_NAMESPACE_END