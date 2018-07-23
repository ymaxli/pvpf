//
// Created by Jiabei He on 2018/7/21.
//

#ifndef POINT_VISION_PIPELINE_FRAMEWORK_CONTEXT_GENERATOR_HPP
#define POINT_VISION_PIPELINE_FRAMEWORK_CONTEXT_GENERATOR_HPP

#include <pvpf/pvpf.hpp>
#include "pvpf/task_execution/body.hpp"
#include <rapidjson/document.h>
#include <pvpf/task_execution/scheduler.hpp>

PVPF_NAMESPACE_BEGIN
namespace task_execution {

    std::vector<std::pair<int, std::string>> analyze_mapping_value(std::string value, int size);

    bool is_cpu(std::string algorithm_name);

    std::shared_ptr<context> create_context(rapidjson::Value const &obj, scheduler& sch);

    void figure_out_json_object(rapidjson::Document &conf, scheduler& sch);

}

PVPF_NAMESPACE_END
#endif //POINT_VISION_PIPELINE_FRAMEWORK_CONTEXT_GENERATOR_HPP

