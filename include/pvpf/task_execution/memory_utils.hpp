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

    std::shared_ptr<core::any> copy_cpu_to_gpu(const std::shared_ptr<core::any> &cpu_ptr);
    void copy_cpu_to_gpu(data_bucket& data);

    std::shared_ptr<core::any> copy_gpu_to_cpu(const std::shared_ptr<core::any> &gpu_ptr);
    void copy_gpu_to_cpu(data_bucket& data);

    std::shared_ptr<core::any> copy_cpu(const std::shared_ptr<core::any> &cpu_ptr);
    void copy_cpu(data_bucket& data);

    std::shared_ptr<core::any> copy_gpu(const std::shared_ptr<core::any> &gpu_ptr);
    void copy_gpu(data_bucket& data);

    void free_cpu(std::shared_ptr<core::any> cpu_ptr);


    void free_gpu(std::shared_ptr<core::any> gpu_ptr);

}

PVPF_NAMESPACE_END


#endif //POINT_VISION_PIPELINE_FRAMEWORK_MEMORY_UTILS_HPP
