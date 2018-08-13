//
// Created by jiabei on 2018/7/5.
//
#include <pvpf/task_execution/scheduler.hpp>
#include <pvpf/task_execution/scheduler.logical_node.hpp>
#include <pvpf/task_execution/body.hpp>
#include <string>
#include <boost/algorithm/string.hpp>
#include <pvpf/data_io/factory.hpp>
#include <tbb/flow_graph.h>
#include <iostream>
#include <pvpf/config/config_reader.hpp>
#include <pvpf/task_execution/context_generator.hpp>
#include <utility>

#define BUFFER_SIZE 10

using namespace tbb::flow;
using namespace std;
using namespace rapidjson;
using namespace pvpf::os_agnostic;
using namespace pvpf::config;
using namespace pvpf::data_io;

PVPF_NAMESPACE_BEGIN
namespace task_execution
{
void scheduler::build_graph(rapidjson::Document &conf, unordered_map<string, Document> _algorithm_map)
{
    fill_json_obj_map(conf);

    algorithm_map = std::move(_algorithm_map);

    const Value &source_json_list = conf["source"];
    const Value &graph_json_list = conf["graph"];
    const Value &sink_json_list = conf["sink"];

    sink_node_list(sink_json_list);
    graph_node_map(graph_json_list);
    source_node_list(source_json_list);

    connect_nodes();
}

void scheduler::run()
{
    start_sink_functions();
    start_source_functions();
    activate_source_nodes();

    graph.wait_for_all();

    stop_io_threads();
}

void scheduler::start_source_functions()
{
    for (auto &it : source_pipe_map)
    {
        thread t(run_source_func, it.first, move(it.second));
        cout << "start source thread" << endl;
        thread_vector.push_back(std::move(t));
    }
}

void run_source_func(int id, std::unique_ptr<pvpf::data_io::source_io_pipe> pipe)
{
    os_agnostic::dynamic_lib_func_manager &manager = os_agnostic::dynamic_lib_func_manager::get_instance();
    manager.invoke_io_func(id, *(pipe.get()));
};

void scheduler::start_sink_functions()
{
    for (auto &it : sink_pipe_map)
    {
        thread t(run_sink_func, it.first, move(it.second));
        cout << "start sink thread" << endl;
        thread_vector.push_back(std::move(t));
    }
}

void run_sink_func(int id, std::unique_ptr<pvpf::data_io::sink_io_pipe> pipe)
{
    os_agnostic::dynamic_lib_func_manager &manager = os_agnostic::dynamic_lib_func_manager::get_instance();
    manager.invoke_io_func(id, *(pipe.get()));
};

void scheduler::stop_io_threads()
{
    for (int i = 0; i < thread_vector.size(); i++)
    {
        thread_vector[i].join();
    }
}

void scheduler::activate_source_nodes()
{
    for (auto it = source_node_map.begin(); it != source_node_map.end(); it++)
    {
        it->second->source->activate();
    }
}

void scheduler::source_node_list(const Value &conf)
{
    for (auto it = conf.Begin(); it != conf.End(); it++)
    {

        auto c = create_context(*it, this->json_object_map);

        bool flag = !(*it).HasMember("control") || !(*it)["control"].HasMember("block") ||
                    (*it)["control"]["block"].GetBool();
        auto pair = pvpf::data_io::create_source(BUFFER_SIZE, flag);

        save_source_node(*it, c, std::move(get<1>(pair)));
        save_source_pipe(*it, std::move(get<0>(pair)));
    }
}

void scheduler::graph_node_map(Value const &conf)
{
    for (auto it = conf.Begin(); it != conf.End(); it++)
    {
        auto c = create_context(*it, this->json_object_map);

        save_graph_node(*it, c);
    }
}

void scheduler::sink_node_list(Value const &conf)
{
    for (Value::ConstValueIterator it = conf.Begin(); it != conf.End(); it++)
    {
        auto c = create_context(*(it), this->json_object_map);
        auto pair = pvpf::data_io::create_sink(BUFFER_SIZE, true);

        save_sink_node(*it, c, std::move(get<1>(pair)));
        save_sink_pipe(*it, std::move(get<0>(pair)));
    }
}

void scheduler::save_source_node(Value const &obj, shared_ptr<context> cont,
                                 unique_ptr<io_pipe_for_source_node> io_pipe)
{
    string id = obj["id"].GetString();

    source_body ib(cont, std::move(io_pipe));
    auto node = std::make_unique<source_node<data_bucket>>(graph, ib, false);

    source_node_map[id] = std::make_unique<logical_source_node>(std::move(node), cont);
}

void scheduler::save_source_pipe(rapidjson::Value const &obj, std::unique_ptr<source_io_pipe> io_pipe)
{
    auto &manager = dynamic_lib_func_manager::get_instance();

    path location(obj["task"]["dylib"]["location"].GetString());
    string name = obj["task"]["dylib"]["func"].GetString();

    int func_id = manager.load_input_func(location, name);

    source_pipe_map[func_id] = std::move(io_pipe);
}

void scheduler::save_graph_node(Value const &obj, shared_ptr<context> cont)
{
    string id = obj["id"].GetString();

    auto exec = generate_executable(obj, algorithm_map);

    auto params_bucket = make_shared<data_bucket>();
    if (obj.HasMember("task") && obj["task"].HasMember("params"))
    {
        auto const &params = obj["task"]["params"];
        for (auto it = params.MemberBegin(); it != params.MemberEnd(); it++)
        {
            if (it->value.IsString())
            {
                params_bucket->put(string(it->name.GetString()), string(it->value.GetString()));
            }
        }
    }

    node_map[id] = task_execution::generate_logical_graph_node(graph, cont->pre.size(), cont, std::move(exec),
                                                               params_bucket);
}

void scheduler::save_sink_node(Value const &obj, shared_ptr<context> cont,
                               unique_ptr<io_pipe_for_sink_node> io_pipe)
{
    string id = obj["id"].GetString();

    node_map[id] = task_execution::generate_logical_sink_node(graph, cont->pre.size(), cont, std::move(io_pipe));
}

void scheduler::save_sink_pipe(Value const &obj, unique_ptr<sink_io_pipe> io_pipe)
{
    auto &manager = dynamic_lib_func_manager::get_instance();

    path location(obj["task"]["dylib"]["location"].GetString());
    string name = obj["task"]["dylib"]["func"].GetString();

    int func_id = manager.load_output_func(location, name);

    sink_pipe_map[func_id] = std::move(io_pipe);
}

std::unique_ptr<executable>
scheduler::generate_executable(Value const &obj, unordered_map<string, Document> const &map)
{
    string algorithm_name = obj["task"]["algorithm"].GetString();
    auto const &algorithm = map.at(algorithm_name);
    auto result = executable_helper(algorithm, map);
    return result;
}

std::unique_ptr<executable>
scheduler::executable_helper(Value const &obj, unordered_map<string, Document> const &map)
{
    os_agnostic::dynamic_lib_func_manager &manager = os_agnostic::dynamic_lib_func_manager::get_instance();
    unique_ptr<abstract_algorithm> result;
    if (obj.HasMember("meta") && obj["meta"].HasMember("loop"))
    {
        unordered_map<string, string> loop_key_map;
        for (Value::ConstMemberIterator it = obj["meta"]["loop"].MemberBegin();
             it != obj["meta"]["loop"].MemberEnd(); it++)
        {
            loop_key_map[it->name.GetString()] = it->value.GetString();
        }
        result = make_unique<loop_algorithm>(loop_key_map);
    }
    else
    {
        result = make_unique<normal_algorithm>();
    }

    for (Value::ConstValueIterator it = obj["body"].Begin(); it != obj["body"].End(); it++)
    {
        string type = (*it)["type"].GetString();
        if (type == "dylib")
        {
            string location = (*it)["location"].GetString();
            string func = (*it)["func"].GetString();
            dylib_func_ptr ptr = manager.load_algorithm(location, func);
            unique_ptr<executable> temp = make_unique<dynamic_library_func>(ptr);
            result->add_executable(std::move(temp));
        }
        else if (type == "algorithm")
        {
            string algorithm_name = (*it)["algorithm"].GetString();
            auto const &d = map.at(algorithm_name);
            unique_ptr<executable> temp = executable_helper(d, map);
            result->add_executable(std::move(temp));
        }
    }
    return result;
}

void scheduler::connect_nodes()
{
    for (auto it = node_map.begin(); it != node_map.end(); it++)
    {
        auto &current_node = it->second;

        // connect join_node & func_node within one logical node
        switch (current_node->join_size)
        {
        case 1:
            make_edge(*(current_node->wrap.size_1.j_node), *(current_node->wrap.size_1.func_node));
            break;
        case 2:
            make_edge(*(current_node->wrap.size_2.j_node), *(current_node->wrap.size_2.func_node));
            break;
        case 3:
            make_edge(*(current_node->wrap.size_3.j_node), *(current_node->wrap.size_3.func_node));
            break;
        case 4:
            make_edge(*(current_node->wrap.size_4.j_node), *(current_node->wrap.size_4.func_node));
            break;
        }

        // connect current logical node to its predecessors
        context *current_context = get_context_of_logical_graph_node(*current_node);
        for (int i = 0; i < current_context->pre.size(); i++)
        {
            auto &pre = current_context->pre[i];
            if (node_map.count(pre) != 0)
            {
                connect_two_logical_graph_node(*(node_map[pre]), *current_node, i);
            }
            else if (source_node_map.count(pre) != 0)
            {
                connect_logical_source_node_with_logical_graph_node(*(source_node_map[pre]), *current_node, i);
            }
        }
    }
}

void scheduler::fill_json_obj_map(Document &conf)
{
    auto &source_json = conf["source"];
    for (auto node = source_json.Begin(); node != source_json.End(); node++)
    {
        auto const &id = (*node)["id"].GetString();
        Value copy(*node, conf.GetAllocator());
        json_object_map[id] = copy;
    }

    auto &graph_json = conf["graph"];
    for (auto node = graph_json.Begin(); node != graph_json.End(); node++)
    {
        auto const &id = (*node)["id"].GetString();
        Value copy(*node, conf.GetAllocator());
        json_object_map[id] = copy;
    }

    auto &sink_json = conf["sink"];
    for (auto node = sink_json.Begin(); node != sink_json.End(); node++)
    {
        auto const &id = (*node)["id"].GetString();
        Value copy(*node, conf.GetAllocator());
        json_object_map[id] = copy;
    }
}

} // namespace task_execution

PVPF_NAMESPACE_END