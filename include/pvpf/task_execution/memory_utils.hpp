//
// Created by Icay on 13/07/2018.
//

#ifndef POINT_VISION_PIPELINE_FRAMEWORK_MEMORY_UTILS_HPP
#define POINT_VISION_PIPELINE_FRAMEWORK_MEMORY_UTILS_HPP

#include "pvpf/pvpf.hpp"
#include "pvpf/task_execution/body.hpp"
#include <rapidjson/document.h>
#include <tbb/flow_graph.h>
#include <unordered_map>
#include <string>
#include <vector>
#include "pvpf/utils/data_bucket.hpp"

PVPF_NAMESPACE_BEGIN

namespace task_execution {

    void copy_cpu_to_gpu(data_bucket& data);

    void copy_gpu_to_cpu(data_bucket& data);

    void copy_cpu(data_bucket& data);

    void copy_gpu(data_bucket& data);


}

PVPF_NAMESPACE_END


#endif //POINT_VISION_PIPELINE_FRAMEWORK_MEMORY_UTILS_HPP
