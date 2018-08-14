//
// Created by Max on 2018/7/22.
//

#ifndef POINT_VISION_PIPELINE_FRAMEWORK_SCHEDULER_LOGICAL_NODE_HPP
#define POINT_VISION_PIPELINE_FRAMEWORK_SCHEDULER_LOGICAL_NODE_HPP

#ifdef DEV_MODE
#include <iostream>
#endif

#include "pvpf/pvpf.hpp"
#include "pvpf/task_execution/body.hpp"
#include <tbb/flow_graph.h>
#include <pvpf/utils/data_bucket.hpp>

using namespace tbb::flow;

PVPF_NAMESPACE_BEGIN
namespace task_execution
{
/**
 * logical source node struct representing source nodes in project configuration files
 */
struct logical_source_node
{
    std::unique_ptr<tbb::flow::source_node<data_bucket>> source;
    std::shared_ptr<context> cont;

    logical_source_node(std::unique_ptr<tbb::flow::source_node<data_bucket>> source,
                        std::shared_ptr<context> cont) : source(std::move(source)), cont(std::move(cont)){};
};

/**
 * logical graph node struct representing normal nodes & sink nodes in project configuration files
 * notice that this struct uses a union to store different types of TBB nodes
 */
struct logical_graph_node
{
    int join_size = 0;

    union union_wrap {
        union_wrap() {}
        ~union_wrap()
        {
            placeholder = 0;
        }

        int placeholder = 0;

        struct ssize_1
        {
            std::shared_ptr<context> cont;
            std::unique_ptr<function_node<std::array<data_bucket, 1>, data_bucket>> func_node;
            std::unique_ptr<join_node<std::array<data_bucket, 1>>> j_node;

            ssize_1(std::shared_ptr<context> cont,
                    std::unique_ptr<function_node<std::array<data_bucket, 1>, data_bucket>> func_node,
                    std::unique_ptr<join_node<std::array<data_bucket, 1>>> j_node) : cont(std::move(cont)), func_node(std::move(func_node)), j_node(std::move(j_node)) {}
        } size_1;

        struct ssize_2
        {
            std::shared_ptr<context> cont;
            std::unique_ptr<function_node<std::array<data_bucket, 2>, data_bucket>> func_node;
            std::unique_ptr<join_node<std::array<data_bucket, 2>>> j_node;

            ssize_2(std::shared_ptr<context> cont,
                    std::unique_ptr<function_node<std::array<data_bucket, 2>, data_bucket>> func_node,
                    std::unique_ptr<join_node<std::array<data_bucket, 2>>> j_node) : cont(std::move(cont)), func_node(std::move(func_node)), j_node(std::move(j_node)) {}
        } size_2;

        struct ssize_3
        {
            std::shared_ptr<context> cont;
            std::unique_ptr<function_node<std::array<data_bucket, 3>, data_bucket>> func_node;
            std::unique_ptr<join_node<std::array<data_bucket, 3>>> j_node;

            ssize_3(std::shared_ptr<context> cont,
                    std::unique_ptr<function_node<std::array<data_bucket, 3>, data_bucket>> func_node,
                    std::unique_ptr<join_node<std::array<data_bucket, 3>>> j_node) : cont(std::move(cont)), func_node(std::move(func_node)), j_node(std::move(j_node)) {}
        } size_3;

        struct ssize_4
        {
            std::shared_ptr<context> cont;
            std::unique_ptr<function_node<std::array<data_bucket, 4>, data_bucket>> func_node;
            std::unique_ptr<join_node<std::array<data_bucket, 4>>> j_node;

            ssize_4(std::shared_ptr<context> cont,
                    std::unique_ptr<function_node<std::array<data_bucket, 4>, data_bucket>> func_node,
                    std::unique_ptr<join_node<std::array<data_bucket, 4>>> j_node) : cont(std::move(cont)), func_node(std::move(func_node)), j_node(std::move(j_node)) {}
        } size_4;
    } wrap;
};

std::unique_ptr<logical_graph_node>
generate_logical_graph_node(graph &g, size_t join_size, std::shared_ptr<context> cont,
                            std::shared_ptr<executable> exec,
                            std::shared_ptr<data_bucket> params);

std::unique_ptr<logical_graph_node>
generate_logical_sink_node(graph &g, size_t join_size, std::shared_ptr<context> cont,
                           std::unique_ptr<data_io::io_pipe_for_sink_node> io_pipe);

void connect_two_logical_graph_node(logical_graph_node const &first, logical_graph_node const &second, int second_port);
void connect_logical_source_node_with_logical_graph_node(logical_source_node const &first, logical_graph_node const &second, int second_port);

context *
get_context_of_logical_graph_node(logical_graph_node const &node);

} // namespace task_execution
PVPF_NAMESPACE_END

#endif //POINT_VISION_PIPELINE_FRAMEWORK_SCHEDULER_LOGICAL_NODE_HPP
