//
// Created by Icay on 13/07/2018.
//

#ifndef POINT_VISION_PIPELINE_FRAMEWORK_MEMORY_MANAGER_HPP
#define POINT_VISION_PIPELINE_FRAMEWORK_MEMORY_MANAGER_HPP

#include "pvpf/pvpf.hpp"
#include "pvpf/task_execution/body.hpp"
#include <rapidjson/document.h>
#include <tbb/flow_graph.h>
#include <unordered_map>
#include <string>
#include <vector>
#include <pvpf/utils/data_bucket.hpp>

PVPF_NAMESPACE_BEGIN
    namespace task_execution {

        class memory_manager {
        public:
            void pre_process(std::vector<data_bucket> & data, context const & context);
            void post_process(data_bucket & data, context const & context);
            std::vector<data_bucket> generate_output_buckets(data_bucket & original);

        };


    }

PVPF_NAMESPACE_END

#endif //POINT_VISION_PIPELINE_FRAMEWORK_MEMORY_MANAGER_HPP
