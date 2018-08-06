//
// Created by Max on 2018/8/3.
//

#include <pvpf/task_execution/scheduler.logical_node.hpp>
#include <pvpf/task_execution/body.hpp>
#include "body.cpp"
#include <pvpf/utils/exception.hpp>
#include <tbb/flow_graph.h>

using namespace std;
using namespace tbb::flow;

PVPF_NAMESPACE_BEGIN
namespace task_execution
{

unique_ptr<logical_graph_node>
generate_logical_graph_node(graph &g, size_t join_size, shared_ptr<context> cont,
                            std::shared_ptr<executable> exec,
                            std::shared_ptr<data_bucket> params)
{

    auto node = make_unique<logical_graph_node>();
    node->join_size = join_size;

    switch (join_size)
    {
    case 1:
    {
        auto func = std::make_unique<function_node<array<data_bucket, 1>, data_bucket>>(g, unlimited,
                                                                                        node_body<1>(cont,
                                                                                                     exec,
                                                                                                     params));
        auto join = std::make_unique<join_node<array<data_bucket, 1>>>(g);
        node->wrap.size_1 = {std::move(cont), std::move(func), std::move(join)};
        break;
    }
    case 2:
    {
        auto func = std::make_unique<function_node<array<data_bucket, 2>, data_bucket>>(g, unlimited,
                                                                                        node_body<2>(cont,
                                                                                                     exec,
                                                                                                     params));
        auto join = std::make_unique<join_node<array<data_bucket, 2>>>(g);
        node->wrap.size_2 = {std::move(cont), std::move(func), std::move(join)};
        break;
    }
    case 3:
    {
        auto func = std::make_unique<function_node<array<data_bucket, 3>, data_bucket>>(g, unlimited,
                                                                                        node_body<3>(cont,
                                                                                                     exec,
                                                                                                     params));
        auto join = std::make_unique<join_node<array<data_bucket, 3>>>(g);
        node->wrap.size_3 = {std::move(cont), std::move(func), std::move(join)};
        break;
    }
    default:
        throw utils::pvpf_exception(
            (std::string("illegal number of join nodes: ") + std::to_string(join_size)).c_str());
    }

    return std::move(node);
}

std::unique_ptr<logical_graph_node>
generate_logical_sink_node(graph &g, size_t join_size, shared_ptr<context> cont,
                           unique_ptr<data_io::io_pipe_for_sink_node> io_pipe)
{

    auto node = make_unique<logical_graph_node>();
    node->join_size = join_size;

    switch (join_size)
    {
    case 1:
    {
        auto func = std::make_unique<function_node<array<data_bucket, 1>, data_bucket>>(g, unlimited,
                                                                                        sink_body<1>(
                                                                                            cont,
                                                                                            std::move(io_pipe)));
        auto join = std::make_unique<join_node<array<data_bucket, 1>>>(g);
        node->wrap.size_1 = {std::move(cont), std::move(func), std::move(join)};
        break;
    }
    case 2:
    {
        auto func = std::make_unique<function_node<array<data_bucket, 2>, data_bucket>>(g, unlimited,
                                                                                        sink_body<2>(
                                                                                            cont,
                                                                                            std::move(io_pipe)));
        auto join = std::make_unique<join_node<array<data_bucket, 2>>>(g);
        node->wrap.size_2 = {std::move(cont), std::move(func), std::move(join)};
        break;
    }
    case 3:
    {
        auto func = std::make_unique<function_node<array<data_bucket, 3>, data_bucket>>(g, unlimited,
                                                                                        sink_body<3>(
                                                                                            cont,
                                                                                            std::move(io_pipe)));
        auto join = std::make_unique<join_node<array<data_bucket, 3>>>(g);
        node->wrap.size_3 = {std::move(cont), std::move(func), std::move(join)};

        break;
    }
    default:
        throw utils::pvpf_exception(
            (std::string("illegal number of join nodes: ") + std::to_string(join_size)).c_str());
    }

    return std::move(node);
}

void connect_two_logical_graph_node(logical_graph_node const &first, logical_graph_node const &second, int second_port)
{
    auto first_size = first.join_size;
    auto second_size = second.join_size;

    if (first_size == 1)
    {
        auto &first_func = first.wrap.size_1.func_node;

        if (second_size == 1)
        {
            auto &second_join = second.wrap.size_1.j_node;
            make_edge(*first_func, input_port<0>(*second_join));
        }
        else if (second_size == 2)
        {
            auto &second_join = second.wrap.size_2.j_node;
            switch (second_port)
            {
            case 0:
                make_edge(*first_func, input_port<0>(*second_join));
                break;
            case 1:
                make_edge(*first_func, input_port<1>(*second_join));
                break;
            }
        }
        else if (second_size == 3)
        {
            auto &second_join = second.wrap.size_3.j_node;
            switch (second_port)
            {
            case 0:
                make_edge(*first_func, input_port<0>(*second_join));
                break;
            case 1:
                make_edge(*first_func, input_port<1>(*second_join));
                break;
            case 2:
                make_edge(*first_func, input_port<2>(*second_join));
                break;
            }
        }
    }
    else if (first_size == 2)
    {
        auto &first_func = first.wrap.size_2.func_node;

        if (second_size == 1)
        {
            auto &second_join = second.wrap.size_1.j_node;
            make_edge(*first_func, input_port<0>(*second_join));
        }
        else if (second_size == 2)
        {
            auto &second_join = second.wrap.size_2.j_node;
            switch (second_port)
            {
            case 0:
                make_edge(*first_func, input_port<0>(*second_join));
                break;
            case 1:
                make_edge(*first_func, input_port<1>(*second_join));
                break;
            }
        }
        else if (second_size == 3)
        {
            auto &second_join = second.wrap.size_3.j_node;
            switch (second_port)
            {
            case 0:
                make_edge(*first_func, input_port<0>(*second_join));
                break;
            case 1:
                make_edge(*first_func, input_port<1>(*second_join));
                break;
            case 2:
                make_edge(*first_func, input_port<2>(*second_join));
                break;
            }
        }
    }
    else if (first_size == 3)
    {
        auto &first_func = first.wrap.size_3.func_node;

        if (second_size == 1)
        {
            auto &second_join = second.wrap.size_1.j_node;
            make_edge(*first_func, input_port<0>(*second_join));
        }
        else if (second_size == 2)
        {
            auto &second_join = second.wrap.size_2.j_node;
            switch (second_port)
            {
            case 0:
                make_edge(*first_func, input_port<0>(*second_join));
                break;
            case 1:
                make_edge(*first_func, input_port<1>(*second_join));
                break;
            }
        }
        else if (second_size == 3)
        {
            auto &second_join = second.wrap.size_3.j_node;
            switch (second_port)
            {
            case 0:
                make_edge(*first_func, input_port<0>(*second_join));
                break;
            case 1:
                make_edge(*first_func, input_port<1>(*second_join));
                break;
            case 2:
                make_edge(*first_func, input_port<2>(*second_join));
                break;
            }
        }
    }
}

void connect_logical_source_node_with_logical_graph_node(logical_source_node const &first, logical_graph_node const &second, int second_port)
{
    auto second_size = second.join_size;
    auto &first_source = first.source;

    if (second_size == 1)
    {
        auto &second_join = second.wrap.size_1.j_node;
        make_edge(*first_source, input_port<0>(*second_join));
    }
    else if (second_size == 2)
    {
        auto &second_join = second.wrap.size_2.j_node;
        switch (second_port)
        {
        case 0:
            make_edge(*first_source, input_port<0>(*second_join));
            break;
        case 1:
            make_edge(*first_source, input_port<1>(*second_join));
            break;
        }
    }
    else if (second_size == 3)
    {
        auto &second_join = second.wrap.size_3.j_node;
        switch (second_port)
        {
        case 0:
            make_edge(*first_source, input_port<0>(*second_join));
            break;
        case 1:
            make_edge(*first_source, input_port<1>(*second_join));
            break;
        case 2:
            make_edge(*first_source, input_port<2>(*second_join));
            break;
        }
    }
}

context *
get_context_of_logical_graph_node(logical_graph_node const &node)
{
    switch (node.join_size)
    {
    case 1:
        return node.wrap.size_1.cont.get();
    case 2:
        return node.wrap.size_2.cont.get();
    case 3:
        return node.wrap.size_3.cont.get();
    }
}

} // namespace task_execution
PVPF_NAMESPACE_END