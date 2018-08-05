//
// Created by Max on 2018/8/3.
//

#include <pvpf/task_execution/scheduler.logical_node.hpp>
#include <pvpf/task_execution/body.hpp>
#include "body.cpp"
#include <pvpf/utils/exception.hpp>

using namespace std;

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
        node->wrap.size_1 = {std::move(cont), std::move(func)};
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
        node->wrap.size_1 = {std::move(cont), std::move(func)};
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
} // namespace task_execution
PVPF_NAMESPACE_END