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
    namespace task_execution {
        struct logical_node {
            int join_size = 0;

            union union_wrap {
                ~union_wrap() {
                    placeholder = 0;
                }

                int placeholder = 0;
                struct ssize_1 {
                    std::shared_ptr<context> cont;
                    std::unique_ptr<function_node<data_bucket>> func_node;
                } size_1;
                struct ssize_2 {
                    std::shared_ptr<context> cont;
                    std::unique_ptr<join_node<std::array<data_bucket, 2>>> join_node;
                    std::unique_ptr<function_node<std::array<data_bucket, 2>>> func_node;
                } size_2;
                struct ssize_3 {
                    std::shared_ptr<context> cont;
                    std::unique_ptr<join_node<std::array<data_bucket, 3>>> join_node;
                    std::unique_ptr<function_node<std::array<data_bucket, 3>>> func_node;
                } size_3;
            } wrap;
        };
    }
PVPF_NAMESPACE_END

#endif //POINT_VISION_PIPELINE_FRAMEWORK_SCHEDULER_LOGICAL_NODE_HPP
